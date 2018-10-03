/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Main.cpp
*/

#include <string>

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

		std::string line;
		while (std::getline(std::cin, line)) {

			if (line == "exit")
			{
				client.stop();
				exit(0);
			}

			Command command = {0};
			command.magic = COMMAND_MAGIC;
			command.data.id = (CommandIdentifier) std::stoi(line);
			int indexChar = 0;
			for (; indexChar < line.size() && !isalpha(line[indexChar]); ++indexChar);
			line = line.substr(static_cast<unsigned long>(indexChar));
			std::memcpy((char *)command.data.data + sizeof(CommandIdentifier), line.c_str(), line.size());

			client.write(BabelString((char *)&command, sizeof(Command)));
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

		std::string line;
		while (std::getline(std::cin, line)) {

			if (line == "exit")
			{
				server.stop();
				exit(0);
			}
			else
				server.broadcast(line.c_str());
		}

		server.stop();
	}
	catch (std::exception &e) {
		std::cerr << "An error occured. Exiting." << std::endl;
	}

	return 0;
}

int main(int argc, char *argv[]) {
	OpenSSL_add_all_algorithms();
	RAND_load_file("/dev/urandom", 1024);

	if (argc <= 1)
		std::cerr << "Usage: Chat <-c|-s> <...>\n";
	else {
		if (BabelString(argv[1]) == "-c")
			return main_client(argc - 1, argv + 1);
		else if (BabelString(argv[1]) == "-s")
			return main_server(argc - 1, argv + 1);
		else
			std::cerr << "Usage: Chat <-c|-s>\n";
	}

	return 0;
}