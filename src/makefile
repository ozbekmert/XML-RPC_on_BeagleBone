EXPENDABLES =  Server Server.o RPCHandler.o

# Add any necessary include directories here.
INCLUDE_DIRS = -I /usr/local/include /usr/local/include/xmlrpc-c

# The following line is needed so that the
# system knows where to find the XML-RPC
# libraries at run time.
LD_OPTS = -Wl,-rpath=/usr/local/lib

# Add any necessary library directories here.
LIB_DIRS = -L /usr/local/lib

XML_LIBS = -lxmltok -lxmlparse

SYS_LIBS = -lmd5 -ldl -lz -lssl -lcrypto

CURL_LIBS = -lcurl -lidn -lssl -lcrypto -ldl -lz -lpthread

WWW_LIBS = -lwwwinit -lwwwapp -lwwwxml $(XML_LIBS) -lwwwzip -lwwwhtml -lwwwtelnet -lwwwnews -lwwwhttp -lwwwmime -lwwwgopher -lwwwftp -lwwwfile -lwwwdir -lwwwcache -lwwwstream -lwwwmux -lwwwtrans -lwwwcore -lwwwutils

COMMON_LIBS = $(WWW_LIBS) $(SYS_LIBS) $(CURL_LIBS)

XMLRPC_COMMON_LIBS = -lxmlrpc++ -lxmlrpc -lxmlrpc_xmlparse -lxmlrpc_xmltok

# Libraries needed for programs that act
# as both a server and a client
XMLRPC_SERVER_LIBS = -lxmlrpc_server_abyss++ -lxmlrpc_server_abyss++ -lxmlrpc_server++ -lxmlrpc_server_abyss -lxmlrpc_server -lxmlrpc_abyss

# Libraries needed for servers
SERVER_LIBS = $(XMLRPC_SERVER_LIBS) $(XMLRPC_COMMON_LIBS) 

# Libraries needed for servers that are also clients
SERVER_CLIENT_LIBS =  $(XMLRPC_SERVER_LIBS) $(XMLRPC_COMMON_LIBS)

all: Server

Server: ServerMain.cpp Server.o RPCHandler.o
	g++ -Wall -g -lpthread  $(LD_OPTS) $(LIB_DIRS) -o Server ServerMain.cpp Server.o RPCHandler.o $(SERVER_LIBS)

Server.o: Server.hpp Server.cpp
	g++ -Wall -g -lpthread  $(INCLUDE_DIRS) -c Server.cpp -o Server.o

RPCHandler.o: RPCHandler.hpp RPCHandler.cpp
	g++ -Wall -g  -lpthread  $(INCLUDE_DIRS) -c RPCHandler.cpp -o RPCHandler.o

clean:
	rm -f $(EXPENDABLES)

