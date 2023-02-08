#include <tr1/unordered_map>
#include "../include/StompProtocol.h"
#include "../include/ConnectionHandler.h"
#include "../include/KeyboardThread.h"
#include "vector"
#include <sstream>
#include <thread>
using namespace std;


int main(int argc, char *argv[]) {
	ConnectionHandler handler;
	StompProtocol protocol(handler);
	KeyboardThread keyThread(handler, protocol);

	// Thread that reads from keyboard
	std::thread readFromKeyboardThread(&KeyboardThread::run, &keyThread);
	while(!protocol.shouldTerminate()) {
		if (handler.isConnected()) {
			string ans;
			if (!handler.getFrameAscii(ans, '\0')) {
				cout << "Disconnected. Exiting...\n" << endl;
				break;
			}
			if (ans.length() > 0)
				protocol.processFrame(ans);
		}
		else {
			//sleep for a second
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		
	}
	readFromKeyboardThread.join();
	return 0;
}