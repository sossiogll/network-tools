//
// Created by sossi on 05/11/2021.
//

#ifndef NETWORK_TOOLS_UDPSOCKET_H
#define NETWORK_TOOLS_UDPSOCKET_H

#include "socket.h"
#include "connectionInterface.h"
#include <iostream>


namespace nt {

    class UdpSocket : public Socket {

    public:

        UdpSocket(ConnectionInterface);
        //~UdpSocket();

        int connect(char[], int);
        int listen(int);

        int send(void*);
        void* receive();

    private:
        nt::ConnectionInterface connectionInterface;
    };

}

#endif //NETWORK_TOOLS_UDPSOCKET_H
