package bgu.spl.net.srv.Frames;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImp;
@SuppressWarnings("unchecked")
public class UnsubscribeFrame<T> implements Frame<T> {
    
    private Connections<T> connections;
    private int connectionId;
    private ConcurrentHashMap<String,String> headersToValue;

    public UnsubscribeFrame(Connections<T> connections, int connectionID, ConcurrentHashMap<String,String> headers) {
        this.connections = connections;
        this.connectionId = connectionID;
        this.headersToValue = headers;
    }


    /**
     * Checks if frame is valid
     * @return List of errors in the frame
     */
    public List<String> isLegalFrame() {
        List<String> errorList = new ArrayList<>();
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
            // Unsubscribe
            boolean success = ((ConnectionsImp<String>) connections).Unsubscribe(
                connectionId,
                headersToValue.get("id"));
            
            if (success) {
                // Send recipt frame if needed
                if (headersToValue.get("receipt") != null) {
                    ReceiptFrame receiptFrame = new ReceiptFrame(
                        headersToValue.get("receipt"));
                    connections.send(connectionId, (T)receiptFrame.toString());
                }
            }
            else {
                errorList.add("SubscriptionID does not represent a valid user subscription");
                sendErrorFrame(errorList);   
            }
        }
        else {
            // Send error frame and disconnect
            sendErrorFrame(errorList);
        }
    }
}
