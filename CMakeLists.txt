cmake_minimum_required(VERSION 3.12)
project(CPP_babel_2018)

set(CMAKE_CXX_STANDARD 17)
ADD_DEFINITIONS(-D_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH)

if (WIN32)
    set(CMAKE_CXX_FLAGS "/MD")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(CPP_babel_2018 Source/Main.cpp Source/Packet.cpp Source/Packet.h Source/Client.cpp Source/Client.h Source/Server.cpp Source/Server.h Source/User.cpp Source/User.h Source/ServerSession.cpp Source/ServerSession.h Source/Message.cpp Source/Message.h Source/ClientSession.cpp Source/ClientSession.h Source/CommandLexer.cpp Source/CommandLexer.h Source/Command.h Source/ServerCrypto.h Source/ServerCrypto.cpp Source/ClientCrypto.cpp Source/ClientCrypto.h Source/CommonCrypto.cpp Source/CommonCrypto.h Source/CryptoException.cpp Source/CryptoException.h Source/BabelException.cpp Source/BabelException.h Source/BabelString.cpp Source/BabelString.h)
target_link_libraries(CPP_babel_2018 ${CONAN_LIBS})
