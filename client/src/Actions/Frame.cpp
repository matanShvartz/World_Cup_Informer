#include "../include/StompProtocol.h"
#include <map>
using namespace std;

int StompProtocol::handleErrorFrame(string body) {
    //find error message in body by last "-----"
    string errorMessage = body.substr(body.find_last_of("-----")); 
    errorMessage = errorMessage.substr(errorMessage.find("\n")); 
    //print error message
    std::cout << errorMessage << std::endl;
    //reset connection
    logout();
    return 0;
}

int StompProtocol::handleReceiptFrame(string header) {
    //find receipt id in header by "receipt - id :"
    string receiptId = header.substr(header.find("receipt - id :") + 14);
  
    //find receipt id in receiptIdMap
    string command = receiptToMsg[receiptId];
    
    //if command is "DISCONNECT"
    if (command == "DISCONNECT") {
        //reset connection
        logout();
    }
    //if command is "game_name"
    else {
        //find subID in gameToSubID map
        string subID = gameToSubID[command];
        //check if subID has * at the beginning
        if (subID[0] == '*') {
            //Confirm subscription
            confirmSub(command);
        }
        else {
            //Unsubscribe
            unsubscribe(command);
        }
    }
    return 0;
}

int StompProtocol::handleMessageFrame(string header, string body) {
    //find destination in header by "destination:"
    string destination = header.substr(header.find("destination :/") + 14);
    destination = destination.substr(0, destination.find("\n"));
    string user = body.substr(body.find("user: ") + 6);
    user = user.substr(0, user.find("\n"));
    string teamA = destination.substr(0, destination.find("_"));
    string teamB = destination.substr(destination.find("_") + 1);
    string gameName = teamA + "_" + teamB;
    Game *current = nullptr;
    //check if game exists in gameToEvents map at gameToEvents[gameName][user]
    if (gameToEvents.find(gameName) != gameToEvents.end()) {
        if (gameToEvents[gameName].find(user) != gameToEvents[gameName].end()) {
            //if exists, set current to gameToEvents[gameName][user]
            current = gameToEvents[gameName][user];
        }
        else {
            //if not exists, create new Game object
            current = new Game(teamA, teamB);
        }
    }
    else {
        //if not exists, create new Game object
        current = new Game(teamA, teamB);
    }
    current->parseFrame(body);
    //add game to gameToEvents map by gameName, user and current
    gameToEvents[gameName][user] = current;
    return 0;
}

int StompProtocol::handleConnectedFrame() {
    //print "Login successful"
    std::cout << "Login successful" << std::endl;
    return 0;
}
    

void StompProtocol::processFrame(string frame) {
    //get substring of frame from start to first "\n"
    string command = frame.substr(0, frame.find("\n"));
    frame = frame.substr(frame.find("\n") + 1);
    //split frame to header and body
    string header = frame.substr(0, frame.find("\n\n"));
    string body = "";
    int bodyStart = frame.find("\n\n") + 2;
    if (bodyStart < (signed)frame.size()) {
        body = frame.substr(bodyStart);
    } 
    //check if frame is ERROR
    if (command.compare("ERROR") == 0) {
        handleErrorFrame(body);
    }
    //check if frame is RECEIPT
    if (command.compare("RECEIPT") == 0) {
        handleReceiptFrame(header);
    }
    //check if frame is MESSAGE
    if (command.compare("MESSAGE") == 0) {
        handleMessageFrame(header, body);
    }
    //check if frame is CONNECTED
    if (command.compare("CONNECTED") == 0) {
        handleConnectedFrame();
    }
}