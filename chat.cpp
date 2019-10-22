#include<iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
    struct sockaddr_in serverIPAddr;
    int opt = 1;

    int serverSocket_fd = socket(AF_INET, SOCK_STREAM, 0 );

    if (serverSocket_fd == -1){
        //socket creation failed, abort program
        cerr << "Server socket creation failed. Exiting" << "\n";
        exit(0);
    }
    if (setsockopt(serverSocket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1){
        //optional step that aids in avoiding address already in use errors
        cerr << "Server socket creation failed. Exiting" << "\n";
        exit(0);
    }

    serverIPAddr.sin_family = AF_INET; // IPV4
    serverIPAddr.sin_port =htons(portNumber);
    serverIPAddr.sin_addr.s_addr = INADDR_ANY; //listen on all interfaces
    if (bind(serverSocket_fd, (struct sockaddr*)&serverIPAddr, sizeof serverIPAddr) == -1){
        cerr << "Server socket bind failed. Exiting" << "\n";
        exit(0);
    }

    if(listen(serverSocket_fd, 2) == -1){
        //set server socket to the listening state with a max backlog of 2
        cerr << "Server socket set as listen failed. Exiting" << "\n";
        exit(0);
    }

    size_t hostNameLength = 24;
    char hostname[hostNameLength] ;
    gethostname(hostname, hostNameLength);

    struct hostent *hp = gethostbyname(hostname);
    cout << "Waiting for connection on " << inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[0])) << " port 6324" << "\n";



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