/**
 * This is the XML-RPC server for Example 1.  It waits for client requests 
 * for the "server.add" RPC.  If the two strings passed to the RPC cannot be
 * converted to valid integers, the server throws an exception.  Otherwise, 
 * it returns the sum of the two integers. 
 * @author James Baldassari (jdb@cs.wpi.edu)
 */

// System includes
#include <iostream>
#include <cstdlib>

// Local includes
#include "Server.hpp"
#include "RPCHandler.hpp"
#include <unistd.h>
#include "errno.h"
#include <stdlib.h>

// Declare which objects from std we're using.
using std::cout;
using std::cerr;
using std::endl;

// Use our namespace
using namespace cs4513;

/**
 * Prints the usage and exits.
 */
void printUsageAndExit() {
	cout << "Usage: Server port" << endl;
	cout << "\tport - The port on which to listen." << endl;
	cout << "Example: Server 9382" << endl;
	exit(EXIT_SUCCESS);
}

/**
 * Starts the XML-RPC server, registers the RPC handlers, 
 * and begins listening for client requests.
 * @param args Command-line arguments.  There should be one:
 * the port number on which to listen for XML-RPC requests.
 */
int main(int argc, char** argv) {
	if (argc != 2)
		printUsageAndExit();

	// Convert/validate the given port number.
	errno = 0;
	char* endptr = NULL;
	int portNum = (int) strtod(argv[1], &endptr);

	if ((portNum == 0) || (errno != 0) || (*endptr != 0)) {
		cerr << "Invalid port number: \"" << argv[1] << "\"" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Server: Using port " << portNum << endl;
	
	// Get the singleton instance of the Server class.
	Server* server = Server::instance();

	// Set the port number given via the command-line.
	server->setServerPort(portNum);

	// Initialize the XML-RPC server.
	server->initializeXMLRPCServer();

	// Start the XML-RPC server.
	server->run();

	cout << "Server: Waiting for client connections." << endl;

	/*
	 * The XML-RPC server is in its own thread,
	 * so just sleep until the server should
	 * shut down.  The server will shut down 
	 * when the server.shutdown RPC is executed
	 * or the main process receives a SIGINT 
	 * (CTRL+C).
	 */
	while (server->isRunning() == true) {
		sleep(1);
	}

	// Kill the XML-RPC server.
	server->stop();
	
	// Clean up.
	delete server;
	cout << "Server: Halted." << endl;
	return EXIT_SUCCESS;
}
