//
// Created by sossi on 05/11/2021.
//

#include "../../include/nt/socket.h"

Socket::Socket(){

    this->connected = false;
    this->listening = false;
    this->addressToConnect = nullptr;
    this->addressToListen = nullptr;
    this->inSocket = nullptr;
    this->outSocket = nullptr;
    this->inSocketID = nullptr;
    this->outSocketID = nullptr;
    this->portToListen = -1;
    this->portToConnect = -1;

}

Socket::~Socket(){

    this->close();

}

void Socket::close() {

    this->closeOutSocket();
    this->closeInSocket();

    delete this->addressToListen;
    delete this->addressToConnect;

    this->addressToListen = nullptr;
    this->addressToConnect = nullptr;

}

bool Socket::isConnected() {
    return this->connected;
}

bool Socket::isListening() {
    return this->listening;
}

char* Socket::getAddressToConnect() {
    return this->addressToConnect;
}

int Socket::getPortToListen() {
    return this->portToListen;
}

int Socket::getPortToConnect() {
    return this->portToConnect;
}

char* Socket::getAddressToListen() {
    return addressToListen;
}

void Socket::setInSocket(nt::ConnectionProtocol connectionProtocol, nt::ConnectionInterface connectionInterface) {

    setSocketSettings(this->inSocket, connectionProtocol, connectionInterface, getPortToListen());

}

void Socket::setOutSocket(const struct sockaddr_in* originSocketSettings) {

    copySocketSettings(this->outSocket, originSocketSettings);

}

void Socket::copySocketSettings(sockaddr_in *destinationSocketSettings, const struct sockaddr_in* originSocketSettings) {


    if(destinationSocketSettings == nullptr)
        destinationSocketSettings = new sockaddr_in;

    memcpy(destinationSocketSettings, originSocketSettings, sizeof(sockaddr_in));

}

void Socket::setSocketSettings(sockaddr_in* destinationSocketSettings, nt::ConnectionProtocol connectionProtocol, nt::ConnectionInterface connectionInterface, int port) {

    if(port <= 1)
        throw nt::SocketErrorHandler(2);

    if(destinationSocketSettings == nullptr)
        throw nt::SocketErrorHandler();

    destinationSocketSettings->sin_family = connectionProtocol;
    destinationSocketSettings->sin_addr.s_addr = htonl(connectionInterface);
    destinationSocketSettings->sin_port = htons(port);

}

void Socket::setConnected(bool flag) {

    if(isListening() && flag)
        throw nt::SocketErrorHandler();

    this->connected = flag;
    this->listening = !flag;

}

void Socket::setListening(bool flag) {


    if(isConnected() && flag)
        throw nt::SocketErrorHandler();

    this->connected = !flag;
    this->listening = flag;

}

void Socket::setPortToListen(int port) {
    this->portToListen = port;
}

void Socket::setPortToConnect(int port) {
    this->portToConnect = port;
}

void Socket::setInSocketID(int socketID) {

    if(socketID<0)
        throw nt::SocketErrorHandler();

    if(this->inSocketID != nullptr)
        throw nt::SocketErrorHandler(); //

    this->inSocketID = new int;
    *this->inSocketID = socketID;

}

void Socket::setOutSocketID(int socketID) {

    if(socketID<0)
        throw nt::SocketErrorHandler(); //Errore nella creazione della socket, id non valido

    if(this->outSocketID!= nullptr)
        throw nt::SocketErrorHandler(); //Se si sta creando una nuova socket, non ci dovrebbe essere nessun altra attualmente istanziata

    if(this->outSocketID == nullptr)
        this->outSocketID = new int;

    *this->outSocketID = socketID;

}


#ifdef WIN32

void Socket::initWindowsSocket() {
    // Initialize Winsock
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout<<"[Server] : WSAStartup failed: " << iResult;
    }
}

void Socket::closeInSocket() {

    if(this->inSocketID!=nullptr) {

#ifdef _WIN32
        closesocket(*this->inSocketID);
        WSACleanup();

#else
        close(*this->inSocketID;);
#endif

        delete this->inSocketID;
        delete this->inSocket;
        this->inSocketID = nullptr;
        this->inSocket = nullptr;
    }

}

void Socket::closeOutSocket() {

    if(this->outSocketID!=nullptr) {

#ifdef _WIN32
        closesocket(*this->outSocketID);
        WSACleanup();

#else
        close(*this->outSocketID;);
#endif

        delete this->outSocketID;
        delete this->outSocket;
        this->outSocketID = nullptr;
        this->outSocket = nullptr;
    }

}

void Socket::bindInSocket() {

    if ( (bind(this->getInSocketID(), (sockaddr*)this->getInSocket(), sizeof(sockaddr))) != 0 )
        throw nt::SocketErrorHandler();

}

    const struct sockaddr_in* Socket::getInSocket() {

        if(this->inSocket == nullptr)
            throw nt::SocketErrorHandler();
        return this->inSocket;
    }

    const struct sockaddr_in* Socket::getOutSocket() {
        if(this->outSocket == nullptr)
            throw nt::SocketErrorHandler();
        return this->outSocket;
    }

int Socket::getInSocketID() {
    if(this->inSocketID == nullptr)
        throw nt::SocketErrorHandler();
    return *this->inSocketID;
}

int Socket::getOutSocketID() {
    if(this->outSocketID == nullptr)
        throw nt::SocketErrorHandler();
    return *this->outSocketID;
}


#endif


