//
// Created by sossi on 05/01/2022.
//

#ifndef NETWORK_TOOLS_SOCKETERRORHANDLER_H
#define NETWORK_TOOLS_SOCKETERRORHANDLER_H


#include <exception>
#include <vector>
#include <string>

namespace nt {

    struct socketError {

        int id;
        std::string description;

    };

    class SocketErrorHandler : public std::exception {


    private:
        const std::vector <socketError> errorList{
                {
                    0,
                    "OK"
                },
                {
                    1,
                    "Generic Error"
                },
                {   2,
                    "Port not valid"
                },
        };

        int errorID = 0;

    public:

        SocketErrorHandler(int errorID = 1);
        const socketError* getDescription() const throw();



    };

}

#endif //NETWORK_TOOLS_SOCKETERRORHANDLER_H
