#pragma once

#include <string>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ConnectionHandler {
private:
	std::string host_;
	short port_;
	boost::asio::io_service io_service_;   // Provides core I/O functionality
	tcp::socket socket_;
	bool connected;

public:
	// default constructor
	ConnectionHandler();
	
	// Constructor
	ConnectionHandler(std::string host, short port);

	virtual ~ConnectionHandler();

	// Sets the host and port of the connection
	void setConnection(std::string host, short port);

	// Connect to the remote machine
	bool connect();

	// Read a fixed number of bytes from the server - blocking.
	// Returns false in case the connection is closed before bytesToRead bytes can be read.
	bool getBytes(char bytes[], unsigned int bytesToRead);

	// Send a fixed number of bytes from the client - blocking.
	// Returns false in case the connection is closed before all the data is sent.
	bool sendBytes(const char bytes[], int bytesToWrite);

	// Read an ascii line from the server
	// Returns false in case connection closed before a newline can be read.
	bool getLine(std::string &line);

	// Send an ascii line from the server
	// Returns false in case connection closed before all the data is sent.
	bool sendLine(std::string &line);

	// Get Ascii data from the server until the delimiter character
	// Returns false in case connection closed before null can be read.
	bool getFrameAscii(std::string &frame, char delimiter);

	// Send a message to the remote host.
	// Returns false in case connection is closed before all the data is sent.
	bool sendFrameAscii(const std::string &frame, char delimiter);

	//Checks if the connection is open
	bool isConnected();

	// Close down the connection properly.
	void close();

	// defines the connection handler as connected
	void connecting();

	// reset connection handler
	void reset();

}; //class ConnectionHandler
