//
// Created by sossi on 05/01/2022.
//

#include "../../include/nt/socketErrorHandler.h"

nt::SocketErrorHandler::SocketErrorHandler(int errorID){

    if(errorID >= this->errorList.size() || errorID <= 0)
        this->errorID = 1;
    else
        this->errorID = errorID;

}

const nt::socketError *nt::SocketErrorHandler::getDescription() const throw() {
    return const_cast<socketError *>(&this->errorList[this->errorID]);
}

