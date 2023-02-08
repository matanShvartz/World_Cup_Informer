#include "../include/event.h"
#include "../include/StompProtocol.h"
using namespace std;

string StompProtocol::updatesToString(map<string,string> updatesMap) {
    string updates = "";
    for (auto update : updatesMap) {
        updates += string("    ") + update.first + string(": ") + update.second + string("\n");
    }
    return updates;
}

vector<string> StompProtocol::buildSendFrames(string filePath) {
    names_and_events infoForFrames = parseEventsFile(filePath);
    string gameName = infoForFrames.team_a_name + string("_") + infoForFrames.team_b_name;
    vector<string> frames;
    if (checkSub(gameName))
    {
        for (Event event : infoForFrames.events) {
            string frame = string("SEND\ndestination :/") + gameName +
                    string("\n\nuser: ") + getUsername() + string("\nevent name: ") + event.get_name() +
                    string("\ntime:") + to_string(event.get_time()) + 
                    string("\ngeneral game updates:\n") + updatesToString(event.get_game_updates()) +
                    string("team a updates: \n") + updatesToString(event.get_team_a_updates()) +
                    string("team b updates: \n") + updatesToString(event.get_team_b_updates()) +
                    string("description: \n") + event.get_discription() + "\n";
            frames.push_back(frame); 
        }
    }
    else {
        frames.push_back(string("ERROR"));
    }
    return frames;
}

int StompProtocol::handleReportRequest(vector<string> parameters) {
    
    string filePath = parameters[1];
    vector<string> sendFrames = buildSendFrames(filePath);
    if (sendFrames.back().compare(string("ERROR")) == 0) {
        std::cout << "You are not subscribed to this game yet. \n" << std::endl;
    }
    else {
        for (string frame : sendFrames)
        {
            if (sendFrame(frame) == 1) {
                std::cout << "Failed to send SEND frame. Exiting...\n" << std::endl;
                return 1;
            }
        }
    }
    return 0;
}