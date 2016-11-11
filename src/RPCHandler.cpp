/**
 * This file contains the implementation of the RPC handler 
 * classes.
 * @author James Baldassari (jdb@cs.wpi.edu)
 */

// System includes
#include <iostream>

// XMLR-RPC Includes
#include <xmlrpc-c/girerr.hpp>

// Local includes
#include "RPCHandler.hpp"
#include "Server.hpp"
#include "errno.h"
#include <stdlib.h>

// Declare which objects from std we're using.
using std::cout;
using std::cerr;
using std::endl;

// The RPC handlers may need to access the Server.
using cs4513::Server;

// Define the RPC handler classes to be in the cs4513 namespace
namespace cs4513 {

/**
 * The server.add XML-RPC handler.  Converts the given strings to integers,
 * adds them together, and returns the sum.  Throws an Exception if there 
 * is an error converting the strings to integers.
 * @param params List of RPC parameters.  Should contain 2 strings.
 * @param retvalP Pointer to the return value of the method.  Always returns true.
 */
void AddHandler::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP) {
	int firstInt;
	int secondInt;

	cout << "Server: Handling \"" << cs4513::Server::ADD_RPC_NAME << "\" RPC." << endl;

	// Retrieve the two strings from the paramter list
	string num1 = (string)params.getString(0);
	string num2 = (string)params.getString(1);

	// Convert the first string to an int
	errno = 0;
	char* endptr = NULL;
	firstInt = (int) strtod(num1.c_str(), &endptr);
	if ((errno != 0) || (*endptr != 0)) {
		/*
		 * If the string could not be converted to an integer, throw an exception.
		 * The only exception that should be thrown is a girerr::error because the 
		 * XML-RPC library does not understand any other type of exception.  Throwing 
		 * a girerr::error causes the RPC to complete successfully with an error 
		 * condition set.  Any other type of exception causes the RPC to fail.
		 */
		string err = "server.add: \"" + string(num1) + "\" is not a valid integer.";
		cerr << err << endl;
		throw(girerr::error(err));
	}

	errno = 0;
	endptr = NULL;
	secondInt = (int) strtod(num2.c_str(), &endptr);
	if ((errno != 0) || (*endptr != 0)) {
		/*
		 * If the string could not be converted to an integer, throw an exception.
		 * The only exception that should be thrown is a girerr::error because the 
		 * XML-RPC library does not understand any other type of exception.  Throwing 
		 * a girerr::error causes the RPC to complete successfully with an error 
		 * condition set.  Any other type of exception causes the RPC to fail.
		 */
		string err = "server.add: \"" + string(num2) + "\" is not a valid integer.";
		cerr << err << endl;
		throw(girerr::error(err));
	}

	cout << "Server: Adding " << firstInt << " and " << secondInt << "." << endl;
	// At this point, both integers are valid, so compute the sum.
	int sum = firstInt + secondInt;
	cout << "Server: Returning " << sum << "." << endl;
	
	// Values are returned from the execute method by setting *retvalP equal to any xmlrpc_c::value.
	// Return the sum by creating an xmlrpc_c::value_int and setting *retvalP equal to it.
	*retvalP = xmlrpc_c::value_int(sum);
}

/**
 * This method is called when the sciapp.shutdown RPC is handled.
 * Calls the Server::shutdown() method.
 * @param params List of RPC parameters.
 * @param retvalP Pointer to the return value of the method.  Always returns true.
 */
void ShutdownHandler::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP) {
	cout << "Server: Handling \"" << cs4513::Server::SHUTDOWN_RPC_NAME << "\" RPC." << endl;
	Server::instance()->shutdown();
	*retvalP = xmlrpc_c::value_boolean(true);
}

} // end namespace cs4513
