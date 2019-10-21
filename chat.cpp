#include<iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include "chat.h"

using namespace std;

// main function -
// where the execution of program begins
int main(int argc, char *argv[])
{
    parseArguments(argc, argv);

    return 0;
}

void parseArguments(int argc, char *argv[]){
    if( argc == 1){
        startServer(6324);
        return;
    }
    int c;
    int port;
    string ip;
    while ((c = getopt (argc, argv, "p:s:")) != -1)
        switch (c)
        {
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                ip = optarg;
                break;

            default:
                printHelp();
        }
    int ipAddress = parseIP(ip);

}

void startServer(int portNumber){
    cout << "server starting on port 6324" << "\n";
}

void startClient(int port, int ip){
    cout << "client starting" << "\n";
}

int parseIP(string ip){
    return 0;
}

void printHelp(){
    cout << "Help:" << "\n";
    cout << "Usage: " << "\n";
    cout << "To run the server: " << "chat" << "\n";
    cout << "To run client: " << "chat -p [port number] -s [IP address]" << "\n";
}