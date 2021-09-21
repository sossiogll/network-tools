//
// Created by Sossio Giallaurito on 21/09/2021.
//

#ifndef NETWORK_TOOLS_HOST_H
#define NETWORK_TOOLS_HOST_H

#include <list>
#include <string>
namespace gll {

    class Host {

        typedef struct {
            Host host();
            std::string name;
            bool flag;
        } HostStatus;

    public:
        Host();

        void *scanNetwork(void *);

        void *connect(void *);

        void *disconnect(void *);

        void *send(void *);

    private:
        std::list<HostStatus> networkStatus;
        


    };
}

#endif //NETWORK_TOOLS_HOST_H
