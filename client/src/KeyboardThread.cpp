#include "../include/KeyboardThread.h"
#include <thread>

KeyboardThread::KeyboardThread(ConnectionHandler &handler, StompProtocol &protocol) : handler(&handler), protocol(&protocol) {}

void KeyboardThread::run() {
    while(!protocol->shouldTerminate()) {  
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        vector<string> parameters = extractParameters(line);
        string command = parameters.front();
        int actionResult = 0;
        //Terminate - Added for valgrind and proper closer tests.
        if (command.compare("terminate") == 0) {
            cout << "Terminating..." << endl;
            protocol->setTerminate();
        }
        //login
        else if(command.compare("login") == 0) {
            actionResult = protocol->handleLoginRequest(parameters);
        }
        //join
        else if(command.compare("join") == 0) {
            actionResult = protocol->handleJoinRequest(parameters);
        }
        //exit
        else if(command.compare("exit") == 0) {
            actionResult = protocol->handleExitRequest(parameters);
        }
        //report
        else if(command.compare("report") == 0) {
            actionResult = protocol->handleReportRequest(parameters);
        }
        //summary
        else if(command.compare("summary") == 0) {
            actionResult = protocol->handleSummaryRequest(parameters);
        }
        //logout
        else if(command.compare("logout") == 0) {
            actionResult = protocol->handleLogoutRequest();
            while (protocol->isWaitingForDisconnect())
            {
                //sleep for half a second
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

            }
        }
        if (actionResult == 1) {
            cout << "Action requested failed" << endl;
        }
    }
}

vector<string> KeyboardThread::extractParameters(string line) {
    vector<string> words;
    istringstream iss(line);
    do {
        string word;
        if (iss >> word)
            words.push_back(word);
    } while (iss);
    return words;
}

// Copy constructor
KeyboardThread::KeyboardThread(const KeyboardThread &other) : handler(other.handler), protocol(other.protocol) {}

// Copy assignment operator
KeyboardThread &KeyboardThread::operator=(const KeyboardThread &other) {
    if (this != &other) {
        handler = other.handler;
        protocol = other.protocol;
    }
    return *this;
}

// Move constructor
KeyboardThread::KeyboardThread(KeyboardThread &&other) : handler(other.handler), protocol(other.protocol) {}

// Move assignment operator
KeyboardThread &KeyboardThread::operator=(KeyboardThread &&other) {
    if (this != &other) {
        handler = other.handler;
        protocol = other.protocol;
    }
    return *this;
}

// Destructor
KeyboardThread::~KeyboardThread() {
    // Required if handler and protocol were allocated on the heap instead of the stack
    // if (handler != nullptr){
    //     delete handler;
    // }
    // if (protocol != nullptr){
    //     delete protocol;
    // }
}