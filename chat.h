
#ifndef CHAT
#define CHAT

// your declarations (and certain types of definitions) here
void startServer(int portNumber);
void parseArguments(int argc, char *argv[]);
void printHelp();
void startClient(int port, std::string);
int sendall(int s, const char *buf, int len);
std::string encode(std::string message);
std::string decode(char* message);
void packi16( char *buf, unsigned int i);
int unpacki16( char *buf);


#endif