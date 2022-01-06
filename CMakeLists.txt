cmake_minimum_required(VERSION 3.0)
set(PROJECT "network-tools")

project(${PROJECT})

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 9)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_CONTACT_EMAIL http://groups.google.com/group/mavlink)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32 wsock32)

configure_file (
        "${PROJECT_SOURCE_DIR}/include/nt/config.h.in"
        "${PROJECT_SOURCE_DIR}/include/nt/config.h"
)
add_executable(network_tools main.cpp include/nt/unistd.h.old src/nt/host.cpp.old include/nt/host.h.old include/nt/socket.h include/nt/connectionInterface.h include/nt/networkProtocol.h include/nt/connectionProtocol.h src/nt/udpSocket.cpp include/nt/udpSocket.h src/nt/socket.cpp include/nt/socketErrorHandler.h src/nt/socketErrorHandler.cpp)
