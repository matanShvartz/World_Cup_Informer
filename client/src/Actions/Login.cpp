#include "../include/StompProtocol.h"

string StompProtocol::buildConnectFrame(string username, string password) {
	string frame = string("CONNECT\n") +
				string("accept - version :1.2\n") +
				string("host : stomp . cs . bgu . ac . il\n") +
				string("login :") + username + string("\n") +
				string("passcode :") + password + string("\n\n");
	return frame;
}


int StompProtocol::handleLoginRequest(vector<string> parameters) {
	if (!sentLogin())
	{
		// Extract host, port, username, password
		stringstream hostPortStream(parameters[1]);
		string host; 
		getline(hostPortStream, host, ':');
		string portString;
		getline(hostPortStream, portString, ':');
		string thisUsername = parameters[2];
		string password = parameters[3];

		//Build CONNECT frame
		string connectFrame = buildConnectFrame(thisUsername, password);
		//Open connection with server
		short port = stoi(portString);
		setHandler(port, host);
		if (connectToServer() == 1) {
			std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
			return 1;			
    	}
		//Set login flag
		setLogin();
		//Send CONNECT frame
		if (sendFrame(connectFrame) == 1) {
            std::cout << "Failed to send CONNECT frame. Exiting...\n" << std::endl;
            return 1;
        }
		//Set username
		setUsername(thisUsername);
		return 0;
	} else {
		std::cout << "You are already logged in.\n" << std::endl;
		return 1;
	}
	
	return 1;
}