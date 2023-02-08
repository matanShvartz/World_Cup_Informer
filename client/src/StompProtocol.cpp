#include "../include/StompProtocol.h"
using namespace std;

StompProtocol::StompProtocol(ConnectionHandler& connectionHandler) : 
 handler(&connectionHandler), gameToEvents(), receiptToMsg(), gameToSubID(), 
 waitingForDisconnect(false), sentLoginRequest(false), terminate(false),  subscriptionID(0), username(""),
 mtx()
{}

 StompProtocol::~StompProtocol() {
        // Required if handler is allocated on the heap instead of stack
        // if (handler != nullptr) {
        //     delete handler;
        // }
        // delete all the game pointers in gameToEvents map in range based for loop
        for (auto &game : gameToEvents) {
            for (auto &user : game.second) {
                if (user.second != nullptr)
                    delete user.second;
            }
            game.second.clear();
        }
        gameToEvents.clear();
        receiptToMsg.clear();
        gameToSubID.clear();
        mtx.unlock();
 }

void StompProtocol::setHandler(short port, string host) {
        handler->setConnection(host, port);
}

//insert to gameToSubID at key gameName value *string(subscriptionID) 
void StompProtocol::addSubscription(string gameName) {
    lock_guard<mutex> lock(mtx);
    gameToSubID.insert(pair<string, string>(gameName, "*" + to_string(subscriptionID)));
    subscriptionID++;
}
//insert to receiptToMsg at key receiptID value gameName
void StompProtocol::addMsg(string msg) {
    lock_guard<mutex> lock(mtx);
    string index = to_string(receiptToMsg.size());
    receiptToMsg.insert(pair<string, string>(index, msg));
}
//disconnect from socket, reset all data structures and variables
void StompProtocol::logout() {
    lock_guard<mutex> lock(mtx);
    handler->close();
    handler->reset();
    // delete all the game pointers in gameToEvents map in range based for loop
    for (auto &game : gameToEvents) {
        for (auto &user : game.second) {
            if (user.second != nullptr)
                delete user.second;
        }
        game.second.clear();
    }
    gameToEvents.clear();
    receiptToMsg.clear();
    gameToSubID.clear();
    waitingForDisconnect = false;
    sentLoginRequest = false;
    subscriptionID = 0;
    username = "";
    cout << "Logged out from server" << endl;
}
//remove * char from beginning of the value of gameToSubID at key gameName
void StompProtocol::confirmSub(string gameName) {
    lock_guard<mutex> lock(mtx);
    string subID = gameToSubID[gameName];
    subID.erase(0, 1);
    gameToSubID[gameName] = subID;
    // print to console joined channel gameName
    cout << "Joined channel " << gameName << endl;
}
//Unsubscribe from gameName
void StompProtocol::unsubscribe(string gameName) {
    lock_guard<mutex> lock(mtx);
    // remove from gameToSubID
    gameToSubID.erase(gameName);
    // print to console exited channel gameName
    cout << "Exited channel " << gameName << endl;
}
//Set sentLoginRequest to true
void StompProtocol::setLogin() {
    lock_guard<mutex> lock(mtx);
    sentLoginRequest = true;
    handler->connecting();
}

//Set waitingForDisconnect to true
void StompProtocol::setWaitingForDisconnect() {
    lock_guard<mutex> lock(mtx);
    waitingForDisconnect = true;
}
//Set username to user
void StompProtocol::setUsername(string user) {
    lock_guard<mutex> lock(mtx);
    username = user;
}
//Connect to server using handler
int StompProtocol::connectToServer() {
    if (handler->connect()) {return 0;};
    return 1;
}
//send frame to server using handler
int StompProtocol::sendFrame(string frame) {
    //print frame to console
    if (handler->sendFrameAscii(frame, '\u0000')) {return 0;}
    return 1;
}
//return if sentLoginRequest is true
bool StompProtocol::sentLogin() {
    return sentLoginRequest;
}
//return if value of gameToSubID at gameName exists, if so does it have * char
bool StompProtocol::checkSub(string gameName) {
    if (gameToSubID.find(gameName) != gameToSubID.end()) {
        if (gameToSubID[gameName][0] == '*') {
            return false;
        }
        return true;
    }
    return false;
}
//return if waitingForDisconnect is true
bool StompProtocol::isWaitingForDisconnect() {
    return waitingForDisconnect;
}
//returns subscriptionID as string
string StompProtocol::getSubId() {
    return to_string(subscriptionID);
}
//returns receiptID as string
string StompProtocol::getReceiptID() {
    return to_string(receiptToMsg.size());
}
//returns value of gameToSubID at key gameName
string StompProtocol::getSubByGame(string gameName) {
    if (gameToSubID.find(gameName) == gameToSubID.end()) {
        return "";
    }
    return gameToSubID[gameName];
}
//returns username
string StompProtocol::getUsername() {
    return username;
}

bool StompProtocol::shouldTerminate() {
    return terminate;
}

void StompProtocol::setTerminate() {
    terminate = true;
}
