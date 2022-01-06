//
// Created by sossi on 03/11/2021.
//

#ifndef NETWORK_TOOLS_SOCKET_H
#define NETWORK_TOOLS_SOCKET_H

#ifdef linux
    #include <sys/socket.h>
    #include <netinet/in.h>
	#include <netdb.h>
    #include <stdio.h>
#endif

#ifdef _WIN32
    #include <WS2tcpip.h>
#endif

#include <iostream>
#include "connectionProtocol.h"
#include "connectionInterface.h"
#include "socketErrorHandler.h"

struct message{
    int lengh;
    void* message;
};

class Socket{

    public:
        Socket();
        ~Socket();

        char* getAddressToListen();
        char* getAddressToConnect();
        int getPortToListen();
        int getPortToConnect();

        bool isConnected();
        bool isListening();

        virtual int connect(char[], int) = 0;
        virtual int listen(int) = 0;
        virtual int send(void*) = 0;
        virtual void* receive() = 0;
        void close();

    protected:

        //Setters
        void setPortToListen(int);
        void setPortToConnect(int);
        void setInSocket(nt::ConnectionProtocol, nt::ConnectionInterface);
        void setOutSocket(const struct sockaddr_in*);
        void setConnected(bool);
        void setListening(bool);
        void setInSocketID(int);
        void setOutSocketID(int);

        //Getter
        int getInSocketID();
        int getOutSocketID();
        const struct sockaddr_in* getInSocket();
        const struct sockaddr_in* getOutSocket();

        //Socket tools
        void bindInSocket();
    #ifdef WIN32
        void initWindowsSocket();
    #endif

private:

        int portToListen;
        int portToConnect;
        char* addressToListen;
        char* addressToConnect;
        sockaddr_in* inSocket;
        sockaddr_in* outSocket;
        int* inSocketID;
        int* outSocketID;
        bool connected;
        bool listening;

        //Private socket management tools
        void closeInSocket();
        void closeOutSocket();
        void setSocketSettings(sockaddr_in*, nt::ConnectionProtocol, nt::ConnectionInterface, int);
        void copySocketSettings(sockaddr_in *destinationSocketSettings, const struct sockaddr_in* originSocketSettings);


};

#endif //NETWORK_TOOLS_SOCKET_H
