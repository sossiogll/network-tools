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

const nt::SocketError nt::SocketErrorHandler::getDescription() const throw() {
    return this->errorList[this->errorID];
}

std::ostream& nt::operator<<(std::ostream &os, nt::SocketError socketError) {
    return os << "[Error " <<socketError.id << "] : " << socketError.description <<"\n";

}
