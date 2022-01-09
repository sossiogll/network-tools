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
    #include <ws2tcpip.h>
    #include <windows.h>
    #define sleep(n) Sleep(n*1000)
    #define usleep(n) Sleep(n/1000)
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
        char* getPortToListen();
        char* getPortToConnect();

        bool isConnected();
        bool isListening();

        virtual int connectTo(char*, char*) = 0;
        virtual int listen(char*, char*) = 0;
        virtual int send(void*) = 0;
        virtual void* receive() = 0;
        char * close();

        virtual std::ostream& print(std::ostream&);
        friend std::ostream& operator<<(std::ostream&, Socket&);

    protected:

        //Setters
        void setPortToListen(char*);
        void setPortToConnect(char*);
        void setAddressToListen(char*);
        void setAddressToConnect(char*);
        void setInSocketSettings(nt::ConnectionProtocol, nt::ConnectionInterface);
        void setOutSocketSettings(nt::ConnectionProtocol, nt::ConnectionInterface);
        void setOutSocketSettings(const struct addrinfo*);

        void setConnected(bool);
        void setListening(bool);
        void generateInSocket();
        void generateOutSocket();

        //Getter
        int getInSocketID();
        int getOutSocketID();
        const struct addrinfo* getInSocket();
        const struct addrinfo* getOutSocket();

    #ifdef WIN32
        void initWindowsSocket();
    #endif

private:

        char* portToListen;
        char* portToConnect;
        char* addressToListen;
        char* addressToConnect;
        struct addrinfo* inSocket;
        struct addrinfo* outSocket;
        int* inSocketID;
        int* outSocketID;
        bool connected;
        bool listening;

        //Private socket management tools
        void closeInSocket();
        void closeOutSocket();
        void setSocketSettings(struct addrinfo**, nt::ConnectionProtocol, nt::ConnectionInterface, const char*, const char*);
        void copySocketSettings(struct addrinfo* destinationSocketSettings, const struct addrinfo* originSocketSettings);


};

#endif //NETWORK_TOOLS_SOCKET_H
