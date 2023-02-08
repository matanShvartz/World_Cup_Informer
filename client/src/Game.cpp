#include "../include/Game.h"
#include <regex>
#include <sstream>
#include <fstream>
using namespace std;

Game:: Game(string teamA, string teamB) : teamAname(teamA), teamBname(teamB),
generalUpdates(), teamAstats(), teamBstats(), eventsInfo()
{
}

Game::~ Game()
{
}

void Game::parseFrame(string body) {
    regex replacer("\\s+");
    string eventName = "";
    string description = "";
    string eventTime = "";
    int whichUpdate = 0; // 1 - general game updates, 2 - team A updates, 3 - team B updates, 4 - description
    stringstream ss(body);
    string line;
    while (getline(ss, line, '\n')) {
        if (line.find("game updates:") != line.npos) {
            whichUpdate = 1;
            continue;
        }
        if (line.find("a updates: ") != line.npos) {
            whichUpdate = 2;
            continue;
        }
        if (line.find("b updates: ") != line.npos) {
            whichUpdate = 3;
            continue;
        }
        if (line.find("description:") != line.npos) {
            whichUpdate = 4;
            continue;
        }
        if (whichUpdate == 0) {
            // event name and time
            if (line.find("event name:") != line.npos) {
                eventName = line.substr(12);
            }
            else if (line.find("time:") != line.npos) {
                eventTime = line.substr(5);
            }
        }
        else if (whichUpdate == 1) {
            // general game updates
            line = regex_replace(line, replacer, "");
            int index = line.find(":");
            string key = line.substr(0, index);
            string value = line.substr(index + 1);
            if (generalUpdates.find(key) == generalUpdates.end()) {
                generalUpdates.insert({key, value});
            }
            else {
                generalUpdates[key] = value;
            }
        }
        else if (whichUpdate == 2) {
            // team A updates
            line = regex_replace(line, replacer, "");
            int index = line.find(":");
            string key = line.substr(0, index);
            string value = line.substr(index + 1);
            if (teamAstats.find(key) == teamAstats.end()) {
                teamAstats.insert({key, value});
            }
            else {
                teamAstats[key] = value;
            }
        }
        else if (whichUpdate == 3) {
            // team B updates
            line = regex_replace(line, replacer, "");
            int index = line.find(":");
            string key = line.substr(0, index);
            string value = line.substr(index + 1);
            if (teamBstats.find(key) == teamBstats.end()) {
                teamBstats.insert({key, value});
            }
            else {
                teamBstats[key] = value;
            }
        }
        else if (whichUpdate == 4) {
            // description
            description += line + "\n";
            }   
    } 
    // build event vector
    vector<string> event;
    event.push_back(eventName);
    event.push_back(description);
    // add event time and event vector to eventsInfo map
    eventsInfo.insert({eventTime, event});
}

// write to_string of game to file, create if doesn't exist, overwrite if exists
void Game::writeToFile(string path) {
    std::ofstream file;
    string game = to_string();
    file.open(path);
    file << game;
    file.close();
}


string Game::to_string() {
    string game = teamAname + " vs " + teamBname + "\n" +
    "Game stats:\n" + "General stats:\n";
    for (auto const& x : generalUpdates) {
        game += x.first + ": " + x.second + "\n";
    }
    game += teamAname + " stats:\n";
    for (auto const& x : teamAstats) {
        game += x.first + ": " + x.second + "\n";
    }
    game += teamBname + " stats:\n";
    for (auto const& x : teamBstats) {
        game += x.first + ": " + x.second + "\n";
    }
    game += "Game event reports:\n";
    for (auto const& event : eventsInfo) {
        game += event.first + " - " + event.second[0] + "\n\n" + event.second[1] + "\n\n";
    }
    return game;
}