#include "../inc/Server.hpp"

// CONSTRUCTORS
Server::Server() : socketfd(-1) {}
Server::~Server() {}

// SERVER CONFIGURATION
void Server::server_start(int port, std::string pwd)
{
    this->port = port;
    password = pwd;
    create_socketfd();

    std::cout << "Waiting connections..." << std::endl;
    while (Server::signal == false)
    {
        if(poll(&fd_poll[0], fd_poll.size(), -1) == -1)
            throw(std::runtime_error("Failed to poll."));

        for (size_t i = 0; i < fd_poll.size(); i++)
		{
			if (fd_poll[i].revents == POLLIN)
			{
				if (fd_poll[i].fd == socketfd)
					accept_client();
				else
					recvData(fd_poll[i].fd);
			}
		}
    }
    close_fd();
}

void Server::create_socketfd()
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1)
		throw(std::runtime_error("Failed to create socket."));

    int opt = 1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw(std::runtime_error("Failed to set option: (SO_REUSEADDR)."));
    if (fcntl(socketfd, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("Failed to set option: (O_NONBLOCK)."));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw(std::runtime_error("Failed to bind socket."));

    if (listen(socketfd, SOMAXCONN) == -1)
        throw(std::runtime_error("Failed to listen to socket."));

    new_client.fd = socketfd;
    new_client.events = POLLIN;
    new_client.revents = 0;
    fd_poll.push_back(new_client);
}