#include "../inc/Server.hpp"

bool Server::signal = false;
void Server::sig_handler(int sig)
{
	(void)sig;
	signal = true;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "USAGE: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    
	char *end;
    long port = strtol(argv[1], &end, 10);
    if (*end != '\0' || port < 1024 || port > 65535)
    {
        std::cerr << "Error: port must be a number between 1024 and 65535" << std::endl;
        return 1;
    }

    Server serv;
    try {
		signal(SIGINT, Server::sig_handler);
		signal(SIGQUIT, Server::sig_handler);
		serv.server_start((int)port, argv[2]);
	}
	catch(const std::exception& e) {
		serv.close_fd();
		std::cerr << e.what() << std::endl;
	}
    std::cout << "Server Closed!" << std::endl;
    return 0;
}
