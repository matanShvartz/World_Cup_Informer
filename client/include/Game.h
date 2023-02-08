#pragma once
#include <map>
#include <vector>
#include "event.h"
using namespace std;


class  Game
{
private:
    // name of the teams
    string teamAname;
    string teamBname;
    // general stats and updates ordered alphabetically
    map<string, string> generalUpdates;
    // team a stats ordered alphabetically
    map<string, string> teamAstats;
    // team a stats ordered alphabetically
    map<string, string> teamBstats;
    // event time and description. key - time, value - vector of event name in[0] and event description in [1]
    map<string, vector<string>> eventsInfo;


public:
     Game(string teamA, string teamB);
    ~ Game();
    void parseFrame(string body);
    void writeToFile(string filePath);
    string to_string();
};
