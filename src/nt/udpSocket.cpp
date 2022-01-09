//
// Created by sossi on 05/11/2021.
//

#include "../../include/nt/udpSocket.h"

nt::UdpSocket::UdpSocket(ConnectionInterface connectionInterface) {

    this->connectionInterface = connectionInterface;

}

int nt::UdpSocket::connectTo(char* address, char* port) {

    setPortToConnect(port);
    setAddressToConnect(address);
#ifdef WIN32
    this->initWindowsSocket();
#endif
    this->setOutSocketSettings(ConnectionProtocol::UDP, this->connectionInterface);
    this->generateOutSocket();
}


int nt::UdpSocket::listen(char* address, char* portToListen){

    if(this->isConnected())
        throw nt::SocketErrorHandler();

    try {

        setPortToListen(portToListen);
        setAddressToListen(address);
#ifdef WIN32
        this->initWindowsSocket();
#endif
        this->setInSocketSettings(ConnectionProtocol::UDP, this->connectionInterface);
        this->generateInSocket();

    }
    catch(nt::SocketErrorHandler e){
        std::cout<<e.getDescription();
    }

    return 0;
};

int nt::UdpSocket::send(void * message) {

    char hello[256] = "hello";

    sendto(this->getOutSocketID(), (const char *)hello, strlen(hello),
           0, (const struct sockaddr *) this->getOutSocket(),
           sizeof(sockaddr));
}

void* nt::UdpSocket::receive() {

    int n;
    char buffer[256];
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);
    n=-1;
    do {
        n = recvfrom(getInSocketID(), buffer, 256, 0,
                     (struct sockaddr *) &peer_addr, &peer_addr_len);
    }while(n==-1);
    buffer[n] = '\0';
    printf("Client : %s\n", *buffer);
    return nullptr;
};





