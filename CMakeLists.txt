cmake_minimum_required(VERSION 3.12)
project(CPP_babel_2018)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(CPP_babel_2018 Source/Main.cpp Source/Packet.cpp Source/Packet.h Source/Client.cpp Source/Client.h Source/Server.cpp Source/Server.h Source/User.cpp Source/User.h Source/Session.cpp Source/Session.h)
target_link_libraries(CPP_babel_2018 ${CONAN_LIBS})
