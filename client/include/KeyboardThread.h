#include "ConnectionHandler.h"
#include "StompProtocol.h"

class KeyboardThread {
    private:
        ConnectionHandler *handler;
        StompProtocol *protocol;

        vector<string> extractParameters(string line);
    public:
        KeyboardThread(ConnectionHandler &handler, StompProtocol &protocol);
        void run();
        //copy constructor
        KeyboardThread(const KeyboardThread &other);
        //copy assignment operator
        KeyboardThread &operator=(const KeyboardThread &other);
        //move constructor
        KeyboardThread(KeyboardThread &&other);
        //move assignment operator
        KeyboardThread &operator=(KeyboardThread &&other);
        //destructor
        ~KeyboardThread();
};