//
// Created by sossi on 05/11/2021.
//

#include "../../include/nt/udpSocket.h"

nt::UdpSocket::UdpSocket(ConnectionInterface connectionInterface) {

    this->connectionInterface = connectionInterface;

}

int nt::UdpSocket::connect(char[], int) {


    try {

        #ifdef WIN32
            this->initWindowsSocket();
        #endif
        this->setInSocketID( socket(NetworkProtocol::IPV4, ConnectionProtocol::UDP, 0) );
    }
    catch(nt::SocketErrorHandler e){
        std::cout<<e.getDescription();
    }

}


int nt::UdpSocket::listen(int portToListen){

    if(this->isConnected())
        throw nt::SocketErrorHandler();

    setPortToListen(portToListen);

    try {

#ifdef WIN32
        this->initWindowsSocket();
#endif
        this->setInSocketID( socket(NetworkProtocol::IPV4, ConnectionProtocol::UDP, 0) );
        this->setInSocket(ConnectionProtocol::UDP, this->connectionInterface);
        this->bindInSocket();
        this->setListening(true);
    }
    catch(nt::SocketErrorHandler e){
        std::cout<<e.getDescription();
    }

    return 0;
};

int nt::UdpSocket::send(void *) {

    char hello[256] = "hello";

    sendto(this->getInSocketID(), (const char *)hello, strlen(hello),
           0, (const struct sockaddr *) this->getOutSocket(),
           sizeof(sockaddr));
}

void* nt::UdpSocket::receive() {

    int n, len;
    len = sizeof(sockaddr);
    char buffer[256];
    sockaddr_in* outSocketTemp;
    outSocketTemp = new sockaddr_in;
    int outSocketIDTemp;

    n = recvfrom( outSocketIDTemp, (char *)buffer, 256,
                 MSG_WAITALL, ( struct sockaddr *) outSocketTemp, &len);

    this->setInSocketID(outSocketIDTemp);
    this->setOutSocket(outSocketTemp);

    buffer[n] = '\0';
    printf("Client : %s\n", buffer);

};





