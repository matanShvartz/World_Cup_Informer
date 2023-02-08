package bgu.spl.net.srv;

import java.util.Collection;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
@SuppressWarnings("unchecked")
public class ConnectionsImp<T> implements Connections<T>{


    /*
     * Maps username to corresponding password.
     * key - username
     * value - Password
     */
    private Map<String, String> usernameToPass;

    /*
     * Maps connectionHandler to active username
     * Purpose - Checking if the client has sent CONNECT frame to the server
     * key - ConnectionHandler
     * value - username (Null if didn't connect)
     */

    private Map<ConnectionHandler<String>, String> clientToUsername;

     /*
      * Set of users (represented by username string) logged in to the servers.
      */
    private Set<String> loggedInUsers;

    /*
     * Maps topic to clients
     * key - topic
     * value - Map {  key - ConnectionHandler
     *                value - subscriptionID    }
     */
    private Map<String, Map<ConnectionHandler<T>, String>> topicToClients;
    
    /*
     * Maps connectionID to connectionHandler
     * key - connectionID
     * value - ConnectionHandler
     */
    private Map<Integer, ConnectionHandler<T>> IdToHandler;

    /*
     * Maps client to topics he is subscribed to.
     * key - ConnectionHandler
     * value - Map {  key - subscriptionID
     *                value - topic    }
     */
    private Map<ConnectionHandler<T>, Map<String, String>> clientToTopics;
    
    Integer messageId = 0;

    public ConnectionsImp() {
        usernameToPass = new ConcurrentHashMap<>();
        clientToUsername = new ConcurrentHashMap<>();
        loggedInUsers = new ConcurrentSkipListSet<>();
        topicToClients = new ConcurrentHashMap<>();
        clientToTopics = new ConcurrentHashMap<>();
        IdToHandler = new ConcurrentHashMap<>();
    }

    /*
     * Send msg to user identified by connectionId.
     * Assumes msg is already built as a frame.
     */
    public boolean send(int connectionId, T msg){
        ConnectionHandler<T> clientConnectionHandler = IdToHandler.get(connectionId);
        if (clientConnectionHandler == null)
            return false;
        clientConnectionHandler.send(msg);
        return true;
    }

    /*
     * Send msg to all useres subscribed to channel.
     * Assumes msg contains only the body. The message frame to send is being build using private function.
     */
    public void send(String channel, T msg){
        Map<ConnectionHandler<T>, String> registeredClientsToChannelMap = topicToClients.get(channel);
        for (ConnectionHandler<T> clientConnectionHandler : registeredClientsToChannelMap.keySet()) {
            String subscriptionId = registeredClientsToChannelMap.get(clientConnectionHandler);
            T response = (T)buildSendMessage(channel, subscriptionId, msg);
            clientConnectionHandler.send(response);
        }
    }

    public void disconnect(int connectionId){
        ConnectionHandler<T> clientConnectionHandler = IdToHandler.get(connectionId);
        StompMessagingProtocolImpl<String> clientProtocol = (StompMessagingProtocolImpl<String>)clientConnectionHandler.getProtocol();
        String userName = clientToUsername.get(clientConnectionHandler);
        // Take client out of data structures.
        // clientToTopics
        Map<String,String> clientTopicsMap = clientToTopics.get(clientConnectionHandler);
        Collection<String> clientTopics = new ConcurrentSkipListSet<>();
        if (clientTopicsMap != null) {
            clientTopics = clientTopicsMap.values();
            clientToTopics.remove(clientConnectionHandler);
        }
        // Topic to clients
        for (String topic : clientTopics) {
            topicToClients.get(topic).remove(clientConnectionHandler);
        }
        try {
            // Id to handler
            IdToHandler.remove(connectionId);
            // loggedInUsers
            loggedInUsers.remove(userName);
        } catch (NullPointerException ignored) {}
        
        clientProtocol.terminate();

        System.out.println("Client " + userName + " disconnected.");
       
    }

    public void connect(Integer connectionId, String userName, String password) {
        //Add user to logged in users.
        loggedInUsers.add(userName);
        //Put username and password usernamePassword hash.
        usernameToPass.put(userName, password);
        //Put client and username in clientToUsername hash.
        ConnectionHandler<String> clientHandler = (ConnectionHandler<String>)IdToHandler.get(connectionId);
        clientToUsername.put(clientHandler, userName);
    }

    private String buildSendMessage(String channel, String subscriptionId,  T message){
        String response = "MESSAGE\nsubscription :" + subscriptionId + "\nmessage - id :" + messageId.toString() + "\ndestination :" + channel + "\n\n" + message.toString() +"\u0000";
        return response;
    } 

    public String idToUsername(Integer connectionId) {
        return clientToUsername.get(IdToHandler.get(connectionId));
    }

    /*
     * This function get connection id and subscription id and return the topic the user is registered to.
     * Return value is null if there isnt topic related to this specific subscription id.
     */
    public String connectionIdAndSubscriptionIdToTopic(Integer connectionId, String subscriptionId){
        ConnectionHandler<T> userHandler = IdToHandler.get(connectionId);
        return clientToTopics.get(userHandler).get(subscriptionId);
        
    }

    /* Return values:
     * True if password is correct
     * False if password is incorrect
     * If user doesnt exist, insert to hash and returns true.
    */ 

    public boolean checkPasswordForUser (String userName, String password){
        if (usernameToPass.get(userName) == null){
            usernameToPass.put(userName, password);
            return true;
        } else if (usernameToPass.get(userName).equals(password))
            return true;
        return false;
    }

    public boolean IsTheClientConnected (ConnectionHandler<T> connectionHandler) {
        if (clientToUsername.get(connectionHandler) == null)
            return false;
        return true;
    }

    public void logInUser(String userName){
        loggedInUsers.add(userName);
    }

    public void logOutUser(String userName){
        loggedInUsers.remove(userName);
    }

    public boolean IsloggedInUser(String userName){
        return loggedInUsers.contains(userName);
    }

    public void addConnection(Integer connectionID, ConnectionHandler<T> handler) {
        IdToHandler.put(connectionID, handler);
    }

    public void removeConnection(Integer connectionID) {
        IdToHandler.remove(connectionID);
    }

    public ConnectionHandler<T> convertIdToConnectionHandler(Integer connectionID) {
        return IdToHandler.get(connectionID);
    }

    public boolean isUserSubscribedToChannel(ConnectionHandler<T> clientHandler, String topic) {
        if(topicToClients.get(topic) == null) 
            return false;
        Map<ConnectionHandler<T>, String> topicMap = topicToClients.get(topic);
        if (topicMap.get(clientHandler) == null)
            return false;
        return true;
    }

    /*
     * Adds new subscription to releavnt topic.
     * Creates new topic if didn't exist.
     * Returns true if completed succesfully.
     * Returns false if client already subscribed to topic.
     */
    public boolean Subscribe(String topic, Integer connectionID, String subscriptionID) {
        ConnectionHandler<T> handler = IdToHandler.get(connectionID);
        if(topicToClients.get(topic) == null) 
            topicToClients.put(topic, new ConcurrentHashMap<>());
        Map<ConnectionHandler<T>, String> topicMap = topicToClients.get(topic);
        if (topicMap.get(handler) != null)
            return false;
        topicMap.put(handler, subscriptionID);
        
        // Add relevant info to clientToTopic
        if(clientToTopics.get(handler) == null)
            clientToTopics.put(handler, new ConcurrentHashMap<>());
        clientToTopics.get(handler).put(
            subscriptionID, topic);

        return true;
    }
    /*
     * Removes subscription from releavnt topic.
     * Returns true if completed succesfully.
     * Returns false if client isn't subscribed to topic or if subscriptionID doesn't match.
     */
    public boolean Unsubscribe(Integer connectionID, String subscriptionID) {
        ConnectionHandler<T> handler = IdToHandler.get(connectionID);
        String topic = getTopic(connectionID, subscriptionID);
        // There is no such topic
        if(topicToClients.get(topic) == null) 
            return false;
        Map<ConnectionHandler<T>, String> topicMap = topicToClients.get(topic);
        if (topicMap.get(handler) == null)
            return false;

        // Remove relevant info from clientToTopic 
        boolean removed =  clientToTopics.get(handler).remove(
        subscriptionID, topic);
        if (removed) {
            // Check if it was the last subscription - remove handler if so
            int mapSize = clientToTopics.get(handler).size();
            if (mapSize == 0)
                clientToTopics.remove(handler);
        }

        
        topicToClients.get(topic).remove(handler);

        return removed;
    }

    public String getTopic(Integer connectionID, String subscriptionID) {
        ConnectionHandler<T> handler = IdToHandler.get(connectionID);
        Map<String, String> subToTopic = clientToTopics.get(handler);
        if (subToTopic == null)
            return "";
        String answer = subToTopic.get(subscriptionID);
        if (answer == null)
            return "";
        return answer;
    }



}
