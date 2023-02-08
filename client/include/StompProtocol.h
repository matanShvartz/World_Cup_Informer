#pragma once
#include "ConnectionHandler.h"
#include "Game.h"
#include <mutex>
#include <tr1/unordered_map>
using namespace std;

// TODO: implement the STOMP protocol
class StompProtocol
{
private:
    ConnectionHandler *handler;
    /**
     * Map of game events by user reported and game name
     * key - game_name, value - Map
     * key - username, value - game object
    */
    tr1::unordered_map<string, tr1::unordered_map<string, Game*>> gameToEvents;

    /**
     * Map of messages by receiptID
     * key - receiptID, value - message sent
     * possible values - {DISCONNECT, game_name}
    */
    tr1::unordered_map<string, string> receiptToMsg;

    /**
     * Map of subscription ID by game names
     * key - game name, value - subscriptionID
     * possible values - {DISCONNECT, game_name}
    */
    tr1::unordered_map<string, string> gameToSubID;

    /**
     * Wheter the client is waiting for a receipt for disconnect
    */
    bool waitingForDisconnect;

    /**
     * Wheter the client already sent a login request
    */
    bool sentLoginRequest;

    /**
     * Should Terminate bool
    */
    bool terminate;

    /**
        * SubscriptionID - unique per subscription
    */
    int subscriptionID;

    /**
        * Username currently logged in
    */
    string username;


    /**
     * mutex lock for read/write operations
    */
    std::mutex mtx;

    //set handler(host, port)
    void setHandler(short port, string host);
    //insert to gameToSubID at key gameName value *string(subscriptionID) 
    void addSubscription(string gameName);
    //insert to receiptToMsg at key receiptID value gameName
    void addMsg(string msg);
    //disconnect from socket, reset all data structures and variables
    void logout();
    //remove * char from beginning of the value of gameToSubID at key gameName
    void confirmSub(string gameName);
    //Unsubscribe from gameName
    void unsubscribe(string gameName);
    //Set sentLoginRequest to true
    void setLogin();
    //set waitingForDisconnect to true
    void setWaitingForDisconnect();
    //set username to name
    void setUsername(string name);
    //Connect to server using handler
    int connectToServer();
    //send frame to server using handler
    int sendFrame(string frame);
    //return if sentLoginRequest is true
    bool sentLogin();
    //return if value of gameToSubID at gameName has * char
    bool checkSub(string gameName);
    
    //returns subscriptionID as string
    string getSubId();
    //returns receiptID as string
    string getReceiptID();
    //returns value of gameToSubID at key gameName
    string getSubByGame(string gameName);
    //returns username
    string getUsername();

    /**
     * Builds valid UNSUBSCRIBE frame
     * @return UNSUBSCRIBE frame as string
    */
    string buildUnsubscribeFrame(string gameName);

    /**
     * Builds valid SUBSCRIBE frame
     * @return SUBSCRIBE frame as string
    */
    string buildSubscribeFrame(string gameName);

    /**
     * Builds valid SEND frame
     * @return SEND frames as string
    */
    vector<string> buildSendFrames(string filePath);

    /**
     * Builds valid DISCONNECT frame
     * @return DISCONNECT frame as string
    */
    string buildDisconnectFrame();

    /**
     * Builds valid CONNECT frame
     * @return CONNECT frame as string
    */
    string buildConnectFrame(string username, string password);

    /**
     * Builds string according to map of stats
     * @param updatesMap - map of stats
     * @return string of stats
    */
    string updatesToString(map<string, string> updatesMap);

    /**
     * Handles receipt frame
     * @param headers - headers of receipt frame
     * @return 0 if success, 1 if failure
    */
    int handleReceiptFrame(string headers);

    /**
     * Handles error frame
     * @param body - body of error frame
     * @return 0 if success, 1 if failure
    */
    int handleErrorFrame(string body);

    /**
     * Handles message frame
     * @param headers - headers of message frame
     * @param body - body of message frame
     * @return 0 if success, 1 if failure
    */
    int handleMessageFrame(string headers, string body);

    /**
     * Handles connected frame
     * @return 0 if success, 1 if failure
    */
    int handleConnectedFrame();


public:
    // Constructor
    StompProtocol(ConnectionHandler &handler);

    //Destructor
    ~StompProtocol();

    //Move constructor
    StompProtocol(StompProtocol &&other);

    //Copy constructor
    StompProtocol(const StompProtocol &other);

    //Copy assignment operator
    StompProtocol &operator=(const StompProtocol &other);

    //Move assignment operator
    StompProtocol &operator=(StompProtocol &&other);

    /**
     * Handles login request
     * @param parameters - vector of parameters
     * @param client - client object
     * @return 0 if success, 1 if failure
    */
    int handleLoginRequest(vector<string> parameters);

    /**
     * Handles join request
     * @param parameters - vector of parameters
     * @param client - client object
     * @return 0 if success, 1 if failure
    */
    int handleJoinRequest(vector<string> parameters);

    /**
     * Handles exit request
     * @param parameters - vector of parameters
     * @param client - client object
     * @return 0 if success, 1 if failure
    */
    int handleExitRequest(vector<string> parameters);

    /**
     * Handles report request
     * @param parameters - vector of parameters
     * @param client - client object
     * @return 0 if success, 1 if failure
    */
    int handleReportRequest(vector<string> parameters);

    /**
     * Handles summary request
     * @param parameters - vector of parameters
     * @param client - client object
     * @return 0 if success, 1 if failure
    */
    int handleSummaryRequest(vector<string> parameters);

    /**
     * Handles logout request
     * @param parameters - vector of parameters
     * @param client - client object
     * @return 0 if success, 1 if failure
    */
    int handleLogoutRequest();

    /**
     * Handles disconnect request
     * @param parameters - vector of parameters
     * @param client - client object
     * @return 0 if success, 1 if failure
    */
    int handleDisconnectRequest(vector<string> parameters);

    /**
     * process frame
     * @param frame - string of frame
    */
    void processFrame(string frame);

    //return if waitingForDisconnect is true
    bool isWaitingForDisconnect();

    bool shouldTerminate();

    void setTerminate();

    
};
