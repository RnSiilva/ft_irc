#include "../inc/Server.hpp"

bool Server::signal = false;
void Server::sig_handler(int sig)
{
	(void)sig;
	Server::signal = true;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "USAGE: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    
    Server serv;
    try {
		signal(SIGINT, Server::sig_handler);
		signal(SIGQUIT, Server::sig_handler);
		serv.server_start(atoi(argv[1]), argv[2]);
	}
	catch(const std::exception& e) {
		serv.close_fd();
		std::cerr << e.what() << std::endl;
	}
    std::cout << "Server Closed!" << std::endl;
    return 0;
}
