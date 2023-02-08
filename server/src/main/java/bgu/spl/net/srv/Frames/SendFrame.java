package bgu.spl.net.srv.Frames;
import java.util.concurrent.ConcurrentHashMap;
import java.util.ArrayList;
import java.util.List;

import bgu.spl.net.srv.ConnectionsImp;
@SuppressWarnings("unchecked")
public class SendFrame<T> implements Frame<T>{
    
    private String destination;
    private String body;
    private ConnectionsImp<T> connections;
    private int connectionId;
    private ConcurrentHashMap<String,String> headersToValue;

    public SendFrame(String message, ConnectionsImp<T> connections, int connectionId, ConcurrentHashMap<String,String> MessageHash){
        this.connections = connections;
        this.connectionId = connectionId;
        headersToValue = MessageHash;
    }

    public void act() {
        SetFieldsFromHash();
        List<String> errorList = isLegalFrame();
        if (errorList.size() > 0){
            ErrorFrame errorFrame = new ErrorFrame(errorList, headersToValue);
            connections.send(connectionId, (T)errorFrame.toString());
            connections.disconnect(connectionId);
        } else {
            connections.send(destination, (T)body); 
            String receiptId = headersToValue.get("receipt-id");
            if (receiptId != null) {
                ReceiptFrame receiptFrame = new ReceiptFrame(receiptId);
                connections.send(connectionId, (T)receiptFrame.toString());
            }        
        }
    }
    
    /*
     * Set all object fields.
    */
    private void SetFieldsFromHash() {
        this.body = headersToValue.get("BODY");
        this.destination = headersToValue.get("destination");
    }

    /*
     * Returns true if all the fields are legal.
     * Checks if all fields arent null and if version is legal.
     * Assumes legal passcode.
    */
    private List<String> isLegalFrame() {
        List<String> errorList = new ArrayList<>();
        if (destination == null) {
            errorList.add("Message is malformed. Message should include password.");
        } 
        // Check if user is subscribed to the channel
        else if (!connections.isUserSubscribedToChannel(connections.convertIdToConnectionHandler(connectionId), destination)){
            errorList.add("User is not subscribed to: " + destination);
        }
        return errorList;
    }
}
