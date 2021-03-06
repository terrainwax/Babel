cmake_minimum_required(VERSION 2.8)
project(CPP_babel_2018)

set(CMAKE_CXX_STANDARD 17)
ADD_DEFINITIONS(-D_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH)
set(CMAKEDIR ${CMAKE_BINARY_DIR})
if (WIN32)
    set(CMAKE_CXX_FLAGS "/MD -GX")
	set(CMAKE_BUILD_TYPE Release)
endif()

include(${CMAKEDIR}/conanbuildinfo.cmake)
conan_basic_setup()
if (NOT QTCREATOR)
	add_executable(CPP_babel_2018 Source/Main.cpp Source/Packet.cpp
        Source/Packet.h Source/Client.cpp Source/Client.h Source/Server.cpp
        Source/Server.h Source/User.cpp Source/User.h Source/ServerSession.cpp
        Source/ServerSession.h Source/Message.cpp Source/Message.h
        Source/ClientSession.cpp Source/ClientSession.h Source/CommandLexer.cpp
        Source/CommandLexer.h Source/Command.h Source/ServerCrypto.h
        Source/ServerCrypto.cpp Source/ClientCrypto.cpp Source/ClientCrypto.h
        Source/CommonCrypto.cpp Source/CommonCrypto.h Source/CryptoException.cpp
        Source/CryptoException.h Source/BabelException.cpp
        Source/BabelException.h Source/BabelString.cpp Source/BabelString.h
		Source/Logger.cpp Source/Logger.h Source/Session.cpp Source/Session.h
		Source/ClientException.cpp Source/ClientException.h
		Source/ServerException.cpp Source/ServerException.h Source/NetworkException.cpp Source/NetworkException.h Source/Call.cpp Source/Call.h Source/Call.cpp Source/Call.h)
target_link_libraries(CPP_babel_2018 ${CONAN_LIBS})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Client)
include_directories(${CMAKE_BINARY_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets Multimedia)

set(project_sources
    Client/main.cpp
    Client/login.cpp
    Client/ClientCrypto.cpp
    Client/CommonCrypto.cpp
    Client/CryptoException.cpp
    Client/BabelException.cpp
    Client/BabelString.cpp
    Client/client.cpp
    Client/Packet.cpp
    Client/AudioManager.cpp
    Client/EncodeManager.cpp
    Client/user.cpp
    Client/call.cpp
    Client/AudioThread.cpp
    Client/receive.cpp Client/VoiceThread.cpp Client/VoiceThread.h Client/incall.cpp)
set(project_headers
    Client/login.h
    Client/ClientCrypto.h
    Client/CommonCrypto.h
    Client/CryptoException.h
    Client/BabelException.h
    Client/BabelString.h
    Client/client.h
    Client/Packet.h
    Client/AudioManager.h
    Client/EncodeManager.h
    Client/GlobalSession.h
    Client/Command.h
    Client/user.h
    Client/call.h Client/AudioThread.cpp Client/AudioThread.h
    Client/receive.h Client/VoiceThread.cpp Client/VoiceThread.h Client/incall.h)
set(project_ui 	Client/login.ui Client/user.ui Client/call.ui Client/receive.ui Client/incall.ui)

qt5_wrap_ui(ui_wrap ${project_ui})
qt5_wrap_cpp(moc_sources ${project_headers})
qt5_add_resources(moc_resources Client/assets.qrc)

add_executable(Client ${project_sources} 
	${project_headers}
	${ui_wrap} 
	${moc_sources}
	${moc_resources}
	)
target_link_libraries(Client ${CONAN_LIBS} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network Qt5::Multimedia)

if(WIN32)
	add_custom_command(TARGET Client POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:Client>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:Client>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:Client>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Multimedia> $<TARGET_FILE_DIR:Client>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Network> $<TARGET_FILE_DIR:Client>		)
endif(WIN32)
