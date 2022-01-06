//
// Created by sossi on 03/11/2021.
//

#ifndef NETWORK_TOOLS_NETWORKPROTOCOL_H
#define NETWORK_TOOLS_NETWORKPROTOCOL_H

#include "config.h"

namespace nt{

    enum NetworkProtocol {
        IPV6 = AF_INET6,
        IPV4 = AF_INET
    };

}

#endif //NETWORK_TOOLS_NETWORKPROTOCOL_H
