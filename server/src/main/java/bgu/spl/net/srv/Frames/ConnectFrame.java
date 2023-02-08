package bgu.spl.net.srv.Frames;
import java.util.concurrent.ConcurrentHashMap;
import java.util.ArrayList;
import java.util.List;

import bgu.spl.net.srv.ConnectionsImp;
@SuppressWarnings("unchecked")
public class ConnectFrame<T> implements Frame<T> {
  
    private String accept_version;
    private String host;
    private String userName;
    private String passcode;
    private ConnectionsImp<T> connections;
    private int connectionId;
    private ConcurrentHashMap<String,String> headersToValue;

    public ConnectFrame(String message, ConnectionsImp<T> connections, int connectionId, ConcurrentHashMap<String,String> MessageHash){
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
            connections.connect(connectionId, userName, passcode);
            String ConnectedFrame = buildResponseFrame();
            connections.send(connectionId, (T)ConnectedFrame); 
            String receiptId = headersToValue.get("receipt-id");
            if (receiptId != null) {
                ReceiptFrame receiptFrame = new ReceiptFrame(receiptId);
                connections.send(connectionId, (T)receiptFrame.toString());
            }        
        }
    }

    private String buildResponseFrame () {
        String response = "CONNECTED\nversion: " + accept_version +"\n\n\u0000";
        return response;
    }

    /*
     * Set all object fields.
    */
    private void SetFieldsFromHash() {
        this.accept_version = headersToValue.get("accept - version");
        this.host = headersToValue.get("host");
        this.userName = headersToValue.get("login");
        this.passcode = headersToValue.get("passcode");
    }

    /*
     * Returns true if all the fields are legal.
     * Checks if all fields arent null and if version is legal.
     * Assumes legal passcode.
    */
    private List<String> isLegalFrame() {
        List<String> errorList = new ArrayList<>();
        //Malformed message arrived
        if (host == null || accept_version == null || userName == null || passcode == null) {
            errorList.add("Malformed message arrived.");
            if (accept_version == null || !accept_version.equals("1.2"))
                errorList.add("Message should include version header set as 1.2");
            if (host == null || !host.equals(" stomp . cs . bgu . ac . il"))
                errorList.add("Message should include host header as stomp.cs.bgu.ac.il");
            if (userName == null)
                errorList.add("Message should include username");
            if (passcode == null)
                errorList.add("Message should include password");
        } else {
            //Check if the client is alreadt connected
            if (connections.IsTheClientConnected(connections.convertIdToConnectionHandler(connectionId))){
                errorList.add("The client is already logged in, log out before trying again");
            }
            if (connections.IsloggedInUser(userName)){
                errorList.add("User already logged in.");
            }
            if (!connections.checkPasswordForUser(userName, passcode)){
                errorList.add("Wrong password");
            }
        }
        
        return errorList;
    }

}
