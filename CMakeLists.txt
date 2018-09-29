cmake_minimum_required(VERSION 3.12)
project(CPP_babel_2018)

set(CMAKE_CXX_STANDARD 17)
ADD_DEFINITIONS(-D_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH)

if (WIN32)
    set(CMAKE_CXX_FLAGS "/MD -GX")
	set(CMAKE_BUILD_TYPE Release)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# fuck epitech
if(UNIX AND NOT APPLE)
    set(OPENSSL_DIR "${CMAKE_SOURCE_DIR}/Libraries/openssl/linux")
    set(OPENSSL_LIB "${OPENSSL_DIR}/lib/libssl.a")
    set(LIBCRYPTO "${CMAKE_DL_LIBS}" "${OPENSSL_DIR}/lib/libcrypto.a")
    include_directories(${OPENSSL_DIR}/include/)
elseif(WIN32)
    set(OPENSSL_DIR "${CMAKE_SOURCE_DIR}/Libraries/openssl/windows")
    set(OPENSSL_LIB "${OPENSSL_DIR}/lib/libssl.lib")
    set(LIBCRYPTO "${OPENSSL_DIR}/lib/libcrypto.lib")
    include_directories(${OPENSSL_DIR}/include/)
elseif(APPLE)
    set(OPENSSL_DIR "${CMAKE_SOURCE_DIR}/Libraries/openssl/macos")
    set(OPENSSL_LIB "${OPENSSL_DIR}/lib/libssl.dylib")
    include_directories(${OPENSSL_DIR}/include/)
endif()

message(${OPENSSL_LIB})

add_executable(CPP_babel_2018 Source/Main.cpp Source/Packet.cpp
        Source/Packet.h Source/Client.cpp Source/Client.h Source/Server.cpp
        Source/Server.h Source/User.cpp Source/User.h Source/ServerSession.cpp
        Source/ServerSession.h Source/Message.cpp Source/Message.h
        Source/ClientSession.cpp Source/ClientSession.h Source/CommandLexer.cpp
        Source/CommandLexer.h Source/Command.h Source/ServerCrypto.h
        Source/ServerCrypto.cpp Source/ClientCrypto.cpp Source/ClientCrypto.h
        Source/CommonCrypto.cpp Source/CommonCrypto.h Source/CryptoException.cpp
        Source/CryptoException.h Source/BabelException.cpp
        Source/BabelException.h Source/BabelString.cpp Source/BabelString.h)
target_link_libraries(CPP_babel_2018 ${OPENSSL_LIB} ${LIBCRYPTO} ${CONAN_LIBS})
