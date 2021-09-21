cmake_minimum_required(VERSION 3.0)
project(network_tools)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32 wsock32)

add_executable(network_tools main.cpp unistd.h src/network-tools/host.cpp include/network-tools/host.h)
