#include "../include/StompProtocol.h"
using namespace std;

string StompProtocol::buildDisconnectFrame() {
    string frame = string("DISCONNECT\nreceipt :") + getReceiptID() + string("\n\n");
    return frame;
}

int StompProtocol::handleLogoutRequest() {
	// Build frame
	string disconnectFrame = buildDisconnectFrame();
	// handlereceiptID map
	addMsg("DISCONNECT");
	// Send frame
	if (sendFrame(disconnectFrame) == 1) {
            std::cout << "Failed to send DISCONNECT frame. Exiting...\n" << std::endl;
            return 1;
        }
    setWaitingForDisconnect();
    return 0;
}