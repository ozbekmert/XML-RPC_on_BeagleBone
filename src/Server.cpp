/**
 * This file contains the implementation of the Server class.
 * The Server class initializes the XML-RPC server, starts the 
 * XML-RPC server in a new thread, and passes RPC requests to 
 * the appropriate RPC handler defined in RPCHandler.hpp.
 * @author James Baldassari (jdb@cs.wpi.edu)
 */

// System includes
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <signal.h>
#include <vector>

// XML-RPC includes
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

// Local includes
#include "RPCHandler.hpp"
#include "Server.hpp"

// Declare which objects from std we're using.
using std::string;
using std::cout;
using std::cerr;
using std::endl;

// Use the RPC handlers from RPCHandler.hpp
using cs4513::AddHandler;
using cs4513::ShutdownHandler;

// Define Server to be in the cs4513 namespace
namespace cs4513 {

// Initialize constants...
// Initialize the singleton instance to NULL.
Server* cs4513::Server::serverInstance = NULL;
// Initialize the path to the XML-RPC server log.
const string cs4513::Server::LOG_PATH = "log/server.log";
// Define the RPC_NAME constants.
const string cs4513::Server::ADD_RPC_NAME = "server.add";
const string cs4513::Server::SHUTDOWN_RPC_NAME = "server.shutdown";

/**
 * The destructor.  Frees memory allocated to XML-RPC objects.
 */
Server::~Server() {
	delete abyssServer;
	delete xmlrpcRegistry;
}

/**
 * @return Returns the singleton instance of the Server class.
 */
Server* Server::instance() {
	if (Server::serverInstance == NULL)
		Server::serverInstance = new Server();
	return serverInstance;
}

/**
 * Sets the port on which the server will listen.
 * @param serverPort The server port.
 */
void Server::setServerPort(int serverPort) {
	this->serverPort = serverPort;
}

/**
 * @return Returns true if the server is running, false otherwise.
 */
bool Server::isRunning() const {
	return running;
}

/**
 * Initializes the XML-RPC server and 
 * registers RPC handlers.
 */
void Server::initializeXMLRPCServer() {
	/*
	 * The XML-RPC registry maps RPCs
	 * to their handler functions.
	 */
	xmlrpcRegistry = new xmlrpc_c::registry();

	/*
	 * Register the RPC handlers.
	 * Note that the first parameter to the call to addMethod
	 * is the name of the RPC.  This is the name that the client
	 * will specify when it executes the RPC.  The addMethod 
	 * call associates an RPC handler with an RPC name.
	 */
	xmlrpc_c::methodPtr const addHandler(new AddHandler);
	xmlrpc_c::methodPtr const shutdownHandler(new ShutdownHandler);
	xmlrpcRegistry->addMethod(cs4513::Server::ADD_RPC_NAME, addHandler);
	xmlrpcRegistry->addMethod(cs4513::Server::SHUTDOWN_RPC_NAME, shutdownHandler);

	// Initialize the XML-RPC server.
	abyssServer = new xmlrpc_c::serverAbyss(
		*xmlrpcRegistry, serverPort, Server::LOG_PATH);
}

/**
 * Starts the XML-RPC server in its own thread.
 */
void Server::run() {
	// Register signal handler for SIGINT to gracefully shut down.
	signal(SIGINT, Server::shutdown);

	// Start the XML-RPC server thread.
	cout << "Server: Starting XML-RPC Server." << endl;
	int status = 
		pthread_create(&serverThread, NULL, Server::runServerThread, abyssServer);
	if (status != 0) {
		cerr << "Server: Error creating XML-RPC server thread.  Exiting." << endl;
		exit(EXIT_FAILURE);
	}

	// Set the server state to running.
	running = true;
}

/**
 * Kills the server thread.
 */
void Server::stop() {
	// Cancel the server thread
	pthread_cancel(serverThread);

	// Wait for the XML-RPC server thread to terminate.
	pthread_join(serverThread, NULL);
}

/**
 * This is the server thread.  It simply starts the XML-RPC server,
 * which will run in this thread until the application is terminated.
 */
void* Server::runServerThread(void* serverPtr) {
	// Cast the thread parameter to an abyss server object.
	xmlrpc_c::serverAbyss* server = static_cast<xmlrpc_c::serverAbyss*>(serverPtr);

	// Start the XML-RPC server in this thread.
	// This thread will run until the server
	// is shut down.
	server->run();

	pthread_exit(EXIT_SUCCESS);
}

/**
 * Shuts down the server by setting the running flag to false.
 */
void Server::shutdown() {
	cout << "Server: Shutting down..." << endl;

	// Set the running flag to false, which will cause the main to exit.
	this->running = false;
}

/**
 * Catches a signal and shuts down the singleton server.
 * @param signum The signal number that was caught.
 */
void Server::shutdown(int signum) {
	cout << "Server: Caught signal " << signum << "." << endl;
	Server::instance()->shutdown();
}

} // end namespace cs4513
