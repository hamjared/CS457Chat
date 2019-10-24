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

int VERSION = 457;

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
    startClient(port,ip);

}

void startServer(int portNumber) {
    struct sockaddr_in serverIPAddr;
    int opt = 1;

    int serverSocket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket_fd == -1) {
        //socket creation failed, abort program
        cerr << "Server socket creation failed. Exiting" << "\n";
        exit(0);
    }
    if (setsockopt(serverSocket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        //optional step that aids in avoiding address already in use errors
        cerr << "Server socket creation failed. Exiting" << "\n";
        exit(0);
    }

    serverIPAddr.sin_family = AF_INET; // IPV4
    serverIPAddr.sin_port = htons(portNumber);
    serverIPAddr.sin_addr.s_addr = INADDR_ANY; //listen on all interfaces
    if (bind(serverSocket_fd, (struct sockaddr *) &serverIPAddr, sizeof serverIPAddr) == -1) {
        cerr << "Server socket bind failed. Exiting" << "\n";
        exit(0);
    }

    if (listen(serverSocket_fd, 2) == -1) {
        //set server socket to the listening state with a max backlog of 2
        cerr << "Server socket set as listen failed. Exiting" << "\n";
        exit(0);
    }

    size_t hostNameLength = 24;
    char hostname[hostNameLength];
    gethostname(hostname, hostNameLength);

    struct hostent *hp = gethostbyname(hostname);

    int i = 0;
    while (hp->h_addr_list[i] != nullptr) {
        cout << "Waiting for connection on " << inet_ntoa(*(struct in_addr *) (hp->h_addr_list[i])) << " port 6324"
             << "\n";
        i++;
    }

    int serverIpAddrSize = sizeof(serverIPAddr);
    int newSocket = accept(serverSocket_fd, (struct sockaddr *) &serverIPAddr, (socklen_t * ) & serverIpAddrSize);
    cout << "Found a friend, you will receive first" << "\n";
    int valread;
    char buffer[1024];
    string message;
    while (true){
        //send and receive messages


        message.clear();
        cout << "Friend: " << flush;
        valread = read( newSocket , buffer, 1024);
        cout << decode(buffer) << "\n";

        while (true) {
            cout << "You: " ;
            message.clear();
            getline(cin, message);
            if (message.length() > 140) {
                cout << "Error: input too long\n";
                continue;
            }
            break;
        }

        sendall(newSocket, encode(message).c_str(), encode(message).length());
    }




}

void startClient(int port, string ip){
    string message;
    char buffer[1024];
    int valread;
    struct sockaddr_in server_addr;
    int clientSocket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket_fd ==-1){
        cerr << "client socket failed to create, exiting" << "\n";
        exit(0);
    }

    server_addr.sin_family = AF_INET; //IPV4
    server_addr.sin_port = htons(port);
    if(inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr)<=0)
    {
        cerr << "invalid ip address, exiting" << "\n";
        exit(0);
    }
    cout << "connecting to server..." ;
    if (connect(clientSocket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        cerr << "connection to server failed, exiting" << "\n";
        exit(0);
    }
    cout << "connected!\n";
    cout << "connected to a friend! You send first.\n";

    while(true) {
        cout << "You: ";
        while (true) {
            message.clear();
            getline(cin, message);
            if (message.length() > 140) {
                cout << "Error: input too long\n";
                cout << "You: ";
                continue;
            }
            break;
        }

        sendall(clientSocket_fd, encode(message).c_str() , encode(message).length());
        cout << "Friend: " << flush;
        valread = read(clientSocket_fd, buffer, 1024);
        cout  << decode(buffer) << "\n";
    }
}


void printHelp(){
    cout << "Help:" << "\n";
    cout << "Usage: " << "\n";
    cout << "To run the server: " << "chat" << "\n";
    cout << "To run client: " << "chat -p [port number] -s [IP address]" << "\n";
}

int sendall(int s, const char *buf, int len)  //Function from beej guide to network programming: http://beej.us/guide/bgnet/html/multi/advanced.html#sendall
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = len; // how many we have left to send
    int n;

    while(total < len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }



    return n==-1?-1:0; // return -1 on failure, 0 on success
}

string encode(string message){
    string encodedString;
    char buff[4];
    packi16(buff, VERSION);
    buff[2] = '\0';
    encodedString += buff[0];
    encodedString += buff[1];
    packi16(buff, message.length());
    buff[2] = '\0';
    encodedString += buff[0];
    encodedString += buff[1];
    encodedString += message;

    return encodedString;
}

string decode(char * message){


    char buf[4];
    buf[0] = message[0];
    buf[1] = message[1];
    buf[2] = '\0';
    int version = unpacki16(buf);

    buf[0] = message[2];
    buf[1] = message[3];
    buf[2] = '\0';
    int length = unpacki16(buf);


    string decodedString;
    for(int i = 0; i < length; i++){
        decodedString += message[i+4];
    }




    return decodedString;



}

void packi16( char *buf, unsigned int i) // from https://beej.us/guide/bgnet/html/multi/advanced.html#serialization
{
    *buf++ = i>>8;
    *buf++ = i;
}

int unpacki16( char *buf)
{
    int leftSide = ((int)buf[0]) << 8;
    int rightSide = ((int)buf[1]) & 0x000000FF;
    return leftSide | rightSide;
}



