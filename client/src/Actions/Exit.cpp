#include "../include/StompProtocol.h"
using namespace std;

string StompProtocol::buildUnsubscribeFrame(string gameName) {
    string frame = string("UNSUBSCRIBE\n") +
					string("id :") + getSubByGame(gameName) + string("\n") +
					string("receipt :") + getReceiptID() + string("\n\n");
	return frame;
}

int StompProtocol::handleExitRequest(vector<string> parameters) {
    // Extract game_name
	string gameName = parameters[1];
	if (getSubByGame(gameName) == "") {
		std::cout << "You are not subscribed to this game.\n" << std::endl;
		return 1;
	}
	// Build frame
	string UnsubFrame = buildUnsubscribeFrame(gameName);
	// handle receiptID map
	addMsg(gameName);
	// Send frame
	if (sendFrame(UnsubFrame) == 1) {
            std::cout << "Failed to send UNSUBSCRIBE frame. Exiting...\n" << std::endl;
            return 1;
        }
    return 0;
}