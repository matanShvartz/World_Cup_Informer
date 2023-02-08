#include "../include/StompProtocol.h"


string StompProtocol::buildSubscribeFrame(string gameName) {
	string frame = string("SUBSCRIBE\n") +
					string("destination :/") + gameName + string("\n") +
					string("id :") + getSubId() + string("\n") +
					string("receipt :") + getReceiptID() + string("\n\n");
	return frame;
}


int StompProtocol::handleJoinRequest(vector<string> parameters) {
	// Extract game_name
	string gameName = parameters[1];
	// Build frame
	string SubFrame = buildSubscribeFrame(gameName);
	// handle subscriptionID and receiptID maps
	addSubscription(gameName);
	addMsg(gameName);
	// Send frame
	if (sendFrame(SubFrame) == 1) {
            std::cout << "Failed to send SUBSCRIBE frame. Exiting...\n" << std::endl;
            return 1;
        }
    return 0;
}