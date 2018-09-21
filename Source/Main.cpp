#include "Client.h"
#include "Server.h"

#include <termios.h>
#include <unistd.h>

/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Main.cpp
*/

int main_client(int argc, char *argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: Chat -c <host> <port>\n";
            return 1;
        }

        std::cout << "Username: ";
        std::string username;
        getline(std::cin, username);

        std::cout << "Password: ";

        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        std::string password;
        getline(std::cin, password);

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        std::cout << std::endl;

        if (username.size() >= 16){
            std::cerr << "Username is too long\n";
            return 1;
        }

        Client c(username, password, argv[1], std::atoi(argv[2]));

        c.run();

        Packet nameMsg;
        nameMsg.bodyLength(username.size() + password.size() + 7);
        std::memcpy(nameMsg.body(), "LOGIN:", 6);
        std::memcpy(nameMsg.body() + 6, username.data(), username.size());
        std::memcpy(nameMsg.body() + username.size() + 6, "/", 1);
        std::memcpy(nameMsg.body() + username.size() + 7, password.data(), password.size());
        nameMsg.encodeHeader();
        c.write(nameMsg);

        char line[Packet::max_body_length + 1];
        while (std::cin.getline(line, Packet::max_body_length + 1)) {
            Packet msg;
            msg.bodyLength(std::strlen(line));
            std::memcpy(msg.body(), line, msg.bodyLength());
            msg.encodeHeader();
            c.write(msg);
        }

        c.close();
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

        Server server(std::atoi(argv[1]));

        server.run();
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