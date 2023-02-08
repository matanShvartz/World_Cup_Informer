package bgu.spl.net.srv;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Frames.ConnectFrame;
import bgu.spl.net.srv.Frames.DisconnectFrame;
import bgu.spl.net.srv.Frames.Frame;
import bgu.spl.net.srv.Frames.SendFrame;
import bgu.spl.net.srv.Frames.SubscribeFrame;
import bgu.spl.net.srv.Frames.UnsubscribeFrame;
import java.util.concurrent.ConcurrentHashMap;

public class StompMessagingProtocolImpl<T> implements StompMessagingProtocol<T> {

    private ConnectionsImp<T> connections;
    private int connectionId;
    private ConcurrentHashMap<String,String> headersToValue;
    boolean shouldTerminate = false;

    /**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/
    public void start(int connectionId, Connections<T> connections, ConnectionHandler<T> connectionHandler){
        this.connections = (ConnectionsImp<T>)connections;
        this.connectionId = connectionId;
        headersToValue = new ConcurrentHashMap<>();
    }
    
    public void process(T message){
        //Organize the hash map
        headersToValue = new ConcurrentHashMap<>();
        orderMsgInHash(message);
        String command = headersToValue.get("COMMAND");
        //Case for each command
        if (command.equals("CONNECT")){ 
            Frame<T> connectFrame = new ConnectFrame<T>(message.toString(), connections, connectionId, headersToValue);
            connectFrame.act();
        }
        if (command.equals("DISCONNECT")){ 
            Frame<T> connectFrame = new DisconnectFrame<T>(message.toString(), connections, connectionId, headersToValue);
            connectFrame.act();
        }
        if (command.equals("SEND")){ 
            Frame<T> sendFrame = new SendFrame<T>(message.toString(), connections, connectionId, headersToValue);
            sendFrame.act();
        }
        if (command.equals("UNSUBSCRIBE")){ 
            Frame<T> unsubscribeFrame = new UnsubscribeFrame<T>(connections, connectionId, headersToValue);
            unsubscribeFrame.act();
        }
        if (command.equals("SUBSCRIBE")){ 
            Frame<T> subscribeFrame = new SubscribeFrame<T>(connections, connectionId, headersToValue);
            subscribeFrame.act();
        }
    }

	/**
     * @return true if the connection should be terminated
     */
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    public void terminate() {
        shouldTerminate = true;
    }

    private void orderMsgInHash(T msg){
        //Insert COMMAND into hash
        String currMsg = msg.toString();
        int headerBeginIndex = currMsg.indexOf('\n');
        String header = "COMMAND";
        String value = currMsg.substring(0, headerBeginIndex);
        headersToValue.put(header, value);
        //BodyMsg
        int indexOfBodyBegin = currMsg.indexOf("\n\n");
        int indexOfBodyEnd = currMsg.indexOf("\u0000");
        String bodyMsg = currMsg.substring(indexOfBodyBegin + 2, indexOfBodyEnd);

        //Split headers and insert to hash table
        String headersMsg = currMsg.substring(headerBeginIndex + 1, indexOfBodyBegin);
        String[] lines = headersMsg.split("\n");
        for (String currHeader : lines){
            String[] headerAndValue = currHeader.split(" :");
            headersToValue.put(headerAndValue[0], headerAndValue[1]);
        }
        headersToValue.put("BODY", bodyMsg);
    }

}
