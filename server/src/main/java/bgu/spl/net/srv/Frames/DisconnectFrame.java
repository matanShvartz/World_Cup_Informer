package bgu.spl.net.srv.Frames;
import java.util.concurrent.ConcurrentHashMap;
import java.util.ArrayList;
import java.util.List;
import bgu.spl.net.srv.ConnectionsImp;
@SuppressWarnings("unchecked")
public class DisconnectFrame<T> implements Frame<T> {
     
    private String receiptId;
    private ConnectionsImp<T> connections;
    private int connectionId;
    private ConcurrentHashMap<String,String> headersToValue;

    public DisconnectFrame(String message, ConnectionsImp<T> connections, int connectionId, ConcurrentHashMap<String,String> MessageHash){
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
            connections.logOutUser(connections.idToUsername(connectionId));
            receiptId = headersToValue.get("receipt");
            ReceiptFrame receiptFrame = new ReceiptFrame(receiptId);
            connections.send(connectionId, (T)receiptFrame.toString()); 
            connections.disconnect(connectionId);  
                   
        }
    }

    /*
     * Set all object fields.
    */
    private void SetFieldsFromHash() {
        this.receiptId = headersToValue.get("receipt");
    }

    /*
     * Returns true if all the fields are legal.
     * Checks if all fields arent null and if version is legal.
     * Assumes legal passcode.
    */
    private List<String> isLegalFrame() {
        List<String> errorList = new ArrayList<>();
        if (receiptId == null) {
            errorList.add("Message is malformed. Disconnect message should include receipt id.");
        }
        return errorList;
    }
}
