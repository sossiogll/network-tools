//
// Created by sossi on 03/11/2021.
//

#ifndef NETWORK_TOOLS_CONNECTIONINTERFACE_H
#define NETWORK_TOOLS_CONNECTIONINTERFACE_H

#include "config.h"
#include "connectionProtocol.h"
#include "networkProtocol.h"

namespace nt{

    enum ConnectionInterface {
        ALL = INADDR_ANY,
        LOCALHOST = INADDR_LOOPBACK,
    };

}

#endif //NETWORK_TOOLS_CONNECTIONINTERFACE_H
