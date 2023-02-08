package bgu.spl.net.srv.Frames;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;


import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImp;
@SuppressWarnings("unchecked")
public class SubscribeFrame<T> implements Frame<T> {

    private Connections<T> connections;
    private int connectionId;
    private ConcurrentHashMap<String,String> headersToValue;

    public SubscribeFrame(Connections<T> connections, int connectionID, ConcurrentHashMap<String,String> headers) {
        this.connections = connections;
        this.connectionId = connectionID;
        this.headersToValue = headers;
    }

    /**
     * Checks if the frame is valid
     * @return List of Errors in the frame
     */
    private List<String> isLegalFrame() {
        List<String> errorList = new ArrayList<>();
        if (headersToValue.get("destination") == null)
            errorList.add("Message should contain a destination");
        if (headersToValue.get("id") == null)
            errorList.add("Message should contain subscription id");
        if (headersToValue.get("BODY") == null || headersToValue.get("BODY").length() != 0)
            errorList.add("Message body should be empty");
        return errorList;

    }

    /**
     * Sends error frame to the client
     * @param errorList List of errors in the frame
     */
    private void sendErrorFrame(List<String> errorList) {
        ErrorFrame errorFrame = new ErrorFrame(errorList, headersToValue);
        connections.send(connectionId, (T)errorFrame.toString());
        connections.disconnect(connectionId);
    }



    public void act() {
        List<String> errorList = isLegalFrame();
        // Check if frame is legal
        if (errorList.size() == 0) {
            // Subscribe
            boolean succcess = ((ConnectionsImp<String>) connections).Subscribe(
                headersToValue.get("destination"), 
                connectionId, //Should send connectionHandler
                headersToValue.get("id"));
            
            
            // Send recipt frame if needed
            if (succcess) {
                if (headersToValue.get("receipt") != null) {
                    ReceiptFrame receiptFrame = new ReceiptFrame(
                        headersToValue.get("receipt"));
                    connections.send(connectionId, (T)receiptFrame.toString());
                }
            }
            else {
                errorList.add("Cannot add subscription, already subscribed to topic");
                sendErrorFrame(errorList);
            }

        }
        // Send error frame and disconnect
        else{
            sendErrorFrame(errorList);
        }
            
        
    }
}