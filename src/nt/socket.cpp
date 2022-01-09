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
    this->portToListen = nullptr;
    this->portToConnect = nullptr;

}

Socket::~Socket(){

    this->close();

}

char * Socket::close() {

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

char* Socket::getPortToListen() {

    return this->portToListen;
}

char* Socket::getPortToConnect() {
    return this->portToConnect;
}

char* Socket::getAddressToListen() {
    return addressToListen;
}

void Socket::setInSocketSettings(nt::ConnectionProtocol connectionProtocol, nt::ConnectionInterface connectionInterface) {

    if(this->inSocket!= nullptr)
        throw nt::SocketErrorHandler();

    setSocketSettings(&this->inSocket, connectionProtocol, connectionInterface,getAddressToListen(), getPortToListen());

}

void Socket::setOutSocketSettings(const struct addrinfo* originSocketSettings) {

    copySocketSettings(this->outSocket, originSocketSettings);

}

void Socket::setOutSocketSettings(nt::ConnectionProtocol connectionProtocol, nt::ConnectionInterface connectionInterface) {

    if(this->outSocket!= nullptr)
        throw nt::SocketErrorHandler();

    setSocketSettings(&this->outSocket, connectionProtocol, connectionInterface, getAddressToConnect(), getPortToConnect());

}

void Socket::copySocketSettings(addrinfo* destinationSocketSettings, const struct addrinfo* originSocketSettings) {


    if(destinationSocketSettings == nullptr)
        destinationSocketSettings = new addrinfo;

    memcpy(destinationSocketSettings, originSocketSettings, sizeof(sockaddr_in));

}

void Socket::setSocketSettings(struct addrinfo** destinationSocketSettings,const nt::ConnectionProtocol connectionProtocol, const nt::ConnectionInterface connectionInterface, const char* address, const char* port) {

    int status;
    struct addrinfo hints;
    struct addrinfo *res;

    if(port == nullptr)
        throw nt::SocketErrorHandler(2);

    if(destinationSocketSettings == nullptr)
        throw nt::SocketErrorHandler();

    memset(&hints,0, sizeof(hints));
    hints.ai_family = connectionProtocol;
    hints.ai_socktype = connectionInterface;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(address, port, &hints, &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        fprintf(stderr, "strerror error %s\n", strerror(errno));
        throw nt::SocketErrorHandler();
    }

    *destinationSocketSettings = res;

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

void Socket::setPortToListen(char* port) {

    if(this->portToListen != nullptr)
        throw nt::SocketErrorHandler();

    this->portToListen = new char[strlen(port)];
    strcpy(this->portToListen, port);
}

void Socket::setPortToConnect(char* port) {
    if(this->portToConnect != nullptr)
        throw nt::SocketErrorHandler();

    this->portToConnect = new char[strlen(port)];
    strcpy(this->portToConnect, port);

}

void Socket::generateInSocket() {

    int tempInSocketID;
    struct addrinfo *rp;

    for (rp = this->inSocket; rp != NULL; rp = rp->ai_next) {

        tempInSocketID = socket(this->inSocket->ai_family, this->inSocket->ai_socktype, this->inSocket->ai_protocol);

        if (tempInSocketID == -1)
            continue;

        this->inSocketID = new int;
        *this->inSocketID = tempInSocketID;

        if (bind(*this->inSocketID, rp->ai_addr, rp->ai_addrlen) == 0) {
            this->setListening(true);
            break;                  /* Success */
        }
        closeInSocket();
    }

    if (this->inSocketID == nullptr) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        throw nt::SocketErrorHandler();
    }

}

void Socket::generateOutSocket() {

    int tempOutSocketID;
    struct addrinfo *rp;
    int connectInfo = 0;

    for (rp = this->outSocket; rp != NULL; rp = rp->ai_next) {

        tempOutSocketID = socket(this->outSocket->ai_family, this->outSocket->ai_socktype, this->outSocket->ai_protocol);

        if (tempOutSocketID == -1)
            continue;

        this->outSocketID = new int;
        *this->outSocketID = tempOutSocketID; /* Success */

        break;
        /*connectInfo = connect(*this->outSocketID, rp->ai_addr, rp->ai_addrlen);
        if (connectInfo == 0) {
            setConnected(true);
            break;
        }*/


    }

    if (this->outSocketID == nullptr) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
    }


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

#endif


void Socket::closeInSocket() {

    if(this->inSocket!=nullptr) {

#ifdef _WIN32
        closesocket(*this->inSocketID);
        WSACleanup();

#else
        close(*this->inSocketID;);
#endif

        delete this->inSocketID;
        freeaddrinfo(inSocket);
        this->inSocketID = nullptr;
        this->inSocket = nullptr;
    }

}

void Socket::closeOutSocket() {

    if(this->outSocket!=nullptr) {

#ifdef _WIN32
        closesocket(*this->outSocketID);
        WSACleanup();

#else
        close(*this->outSocketID;);
#endif

        delete this->outSocketID;
        freeaddrinfo(inSocket);
        this->outSocketID = nullptr;
        this->outSocket = nullptr;
    }

}

    const struct addrinfo* Socket::getInSocket() {

        if(this->inSocket == nullptr)
            throw nt::SocketErrorHandler();
        return this->inSocket;
    }

    const struct addrinfo* Socket::getOutSocket() {
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

std::ostream &operator<<(std::ostream& os, Socket& socket) {
    socket.print(os);
    return os;
}

std::ostream &Socket::print(std::ostream& os) {
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    os<<"[Socket info]:\n";

    if(isListening())
    {
        if (getnameinfo(this->inSocket->ai_addr, this->inSocket->ai_addrlen, hbuf, sizeof(hbuf), sbuf,
                        sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
            os<<"host: "<<hbuf<<"\nport: "<<sbuf;
    }
    else if(isConnected()){
        if (getnameinfo(this->outSocket->ai_addr, this->outSocket->ai_addrlen, hbuf, sizeof(hbuf), sbuf,
                        sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
            os<<"host: "<<hbuf<<"\nport: "<<sbuf;
    }else{
        os<<"host: n.d.\nport: n.d.";
    }


    os<<"\n";

    return os;
}

void Socket::setAddressToListen(char* address) {

    if(this->addressToListen != nullptr)
        throw nt::SocketErrorHandler();

    if(address != NULL || address != nullptr) {
        this->addressToListen = new char[strlen(address)];
        strcpy(this->addressToListen, address);
    }

}

void Socket::setAddressToConnect(char * address) {

    if(this->addressToConnect != nullptr)
        throw nt::SocketErrorHandler();

    if(address != NULL || address != nullptr) {
        this->addressToConnect = new char[strlen(address)+1];
        strcpy(this->addressToConnect, address);
        this->addressToConnect[strlen(address)]='\0';
    }

}





