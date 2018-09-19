#include "Client.h"
#include "Server.h"

/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Main.cpp
*/

int main_client(int argc, char *argv[]) {
    try {
        if (argc != 4) {
            std::cerr << "Usage: Chat -c <username> <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        std::string username = std::string(argv[1]);

        if (username.size() >= 16){
            std::cerr << "Username is too long\n";
            return 1;
        }

        auto endpoint_iterator = resolver.resolve({argv[2], argv[3]});
        Client c(username, io_service, endpoint_iterator);

        std::thread t([&io_service]() { io_service.run(); });

        Packet *namePacket = new Packet();
        namePacket->bodyLength(username.size() + 5);
        std::memcpy(namePacket->body(), "NAME:", 5);
        std::memcpy(namePacket->body() + 5, username.data(), username.size());
        namePacket->encodeHeader();
        c.write(namePacket);

        char line[Packet::max_body_length + 1];
        while (std::cin.getline(line, Packet::max_body_length + 1)) {
            Packet *messagePacket = new Packet();
            messagePacket->bodyLength(std::strlen(line));
            std::memcpy(messagePacket->body(), line, messagePacket->bodyLength());
            messagePacket->encodeHeader();
            c.write(messagePacket);
        }

        c.close();
        t.join();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

int main_server(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: Chat -s <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_service io_service;

        std::list<Server> servers;
        for (int i = 1; i < argc; ++i) {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_service, endpoint);
        }

        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc <= 1)
        std::cerr << "Usage: Chat <-c|-s> <...>\n";
    else {
        if (std::string(argv[1]) == "-c")
            return main_client(argc - 1, argv + 1);
        else if (std::string(argv[1]) == "-s")
            return main_server(argc - 1, argv + 1);
        else
            std::cerr << "Usage: Chat <-c|-s>\n";
    }

    return 0;
}