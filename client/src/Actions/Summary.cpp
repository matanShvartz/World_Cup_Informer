#include "../include/StompProtocol.h"
using namespace std;

int StompProtocol::handleSummaryRequest(vector<string> parameters) {
    // Tell Game object in gameToEvents to write to file
    // Extract game_name
    string gameName = parameters[1];
    // extract user
    string user = parameters[2];
    //extract filename
    string filename = parameters[3];
    int happened = 0;
    //check if Game object exists at gameToEvents[gameName][user]
    if (gameToEvents.find(gameName) != gameToEvents.end()) {
        if (gameToEvents[gameName].find(user) != gameToEvents[gameName].end()) {
            //if exists, call writeToFile(filename)
            gameToEvents[gameName][user]->writeToFile(filename);
        }
        else {
            happened = 1;
        }
    }
    else {
        happened = 1;
    }
    return happened;
}