//
// Created by sossi on 03/11/2021.
//

#ifndef NETWORK_TOOLS_CONNECTIONPROTOCOL_H
#define NETWORK_TOOLS_CONNECTIONPROTOCOL_H

#include "config.h"

namespace nt {

    enum ConnectionProtocol {
        UDP = SOCK_DGRAM,
        TCP = SOCK_STREAM
    };

}

#endif //NETWORK_TOOLS_CONNECTIONPROTOCOL_H
