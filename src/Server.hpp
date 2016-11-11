/**
 * This is the Server class definition.
 * @author James Baldassari (jdb@cs.wpi.edu)
 */

#ifndef _SERVER_HPP
#define _SERVER_HPP

// XML-RPC includes
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

// Declare which objects from std we're using.
using std::string;

// Define Server to be in the cs4513 namespace
namespace cs4513 {

/**
 * The Server class is responsible for starting an XML-RPC
 * server, registering RPC handlers, and servicing client
 * requests.  Note: methods are implemented/documented 
 * in Server.cpp.
 */
class Server {
	private:
		/**
		 * Private constructor because Server is a singleton class.
		 * Note that the constructor initializes the running flag 
		 * to false.
		 */
		Server() : running(false) { }
		static const string LOG_PATH;
		int serverPort;
		bool running;
		xmlrpc_c::registry* xmlrpcRegistry;
		xmlrpc_c::serverAbyss* abyssServer;
		pthread_t serverThread;
		static void* runServerThread(void*);
		static Server* serverInstance;
	public:
		~Server();
		static const string ADD_RPC_NAME;
		static const string SHUTDOWN_RPC_NAME;
		static Server* instance();
		void setServerPort(int);
		bool isRunning() const;
		void initializeXMLRPCServer();
		void run();
		void stop();
		void shutdown();
		static void shutdown(int);
};

} // end namespace cs4513

#endif // #define _SERVER_HPP
