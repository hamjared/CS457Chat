
#ifndef CHAT
#define CHAT

// your declarations (and certain types of definitions) here
void startServer(int portNumber);
void parseArguments(int argc, char *argv[]);
void printHelp();
void startClient(int port, int ip);
int parseIP(std::string ip);


#endif