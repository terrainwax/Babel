/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Main.cpp
*/

#include "Client.h"
#include "Server.h"

#ifdef BOOST_NO_EXCEPTIONS
namespace boost {
	void throw_exception(std::exception const &e) {}
}
#endif

int main_client(int argc, char *argv[]) {
	try {
		if (argc != 3) {
			std::cerr << "Usage: ChatClient -client <address> <port>\n";
			return 1;
		}

		Client client(argv[1], std::atoi(argv[2]));

		client.start();

		/*
		sleep(1);

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

		Packet nameMsg;
		nameMsg.bodyLength(username.size() + password.size() + 7);
		std::memcpy(nameMsg.body(), "LOGIN:", 6);
		std::memcpy(nameMsg.body() + 6, username.data(), username.size());
		std::memcpy(nameMsg.body() + username.size() + 6, "/", 1);
		std::memcpy(nameMsg.body() + username.size() + 7, password.data(), password.size());
		nameMsg.encodeHeader();
		client.write(nameMsg);*/

		std::string line;
		while (std::getline(std::cin, line)) {

			Command command = {0};
			command.magic = COMMAND_MAGIC;
			command.data.id = (CommandIdentifier) std::stoi(line);
			int indexChar = 0;
			for (; indexChar < line.size() && !isalpha(line[indexChar]); ++indexChar);
			line = line.substr(static_cast<unsigned long>(indexChar));
			std::memcpy((char *)command.data.data + sizeof(CommandIdentifier), line.c_str(), line.size());

			client.write(std::string((char *)&command, sizeof(Command)));
		}

		client.stop();
	}
	catch (std::exception &e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

int main_server(int argc, char *argv[]) {
	try {
		if (argc < 2) {
			std::cerr << "Usage: ChatServer -s <port> [<port> ...]\n";
			return 1;
		}

		Server server(std::atoi(argv[1]));

		server.start();

		//sleep(1);

		char line[Packet::max_body_length + 1];
		while (std::cin.getline(line, Packet::max_body_length + 1)) {
			Packet packet;
			packet.bodyLength(std::strlen(line));
			std::memcpy(packet.body(), line, packet.bodyLength());
			packet.encodeHeader();
			std::cout << line << std::endl;
			//server.broadcast(Message(packet, nullptr));
		}
	}
	catch (std::exception &e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

int main(int argc, char *argv[]) {
	OpenSSL_add_all_algorithms();
	RAND_load_file("/dev/urandom", 1024);

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