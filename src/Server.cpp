#include "../inc/Server.hpp"

Server::Server() : socketfd(-1) {}
Server::~Server() {}

Client *Server::get_client(int fd)
{ 
    for (size_t i = 0; i < clients.size(); i++)
    {
		if (clients[i].get_clientfd() == fd)
			return &clients[i];
	}
	return NULL;
}

// ============ SERVER CONFIGURATION ============ 
void Server::server_start(int port, std::string pwd)
{
    this->port = port;
    password = pwd;
    create_socketfd();

    std::cout << "Waiting connections..." << std::endl;
    while (signal == false)
    {
        if((poll(&fd_poll[0], fd_poll.size(), -1) == -1) && signal == false)
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

void Server::accept_client()
{
    socklen_t len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));

    int fd = accept(socketfd, (sockaddr *)&client_addr, &len);
    if (fd == -1)
        throw(std::runtime_error("Failed to accept client."));

    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("Failed to set option: (O_NONBLOCK)."));

    Client client;
    client.set_clientfd(fd);
    client.set_host(inet_ntoa(client_addr.sin_addr));
    clients.push_back(client);

    new_client.fd = fd;
    new_client.events = POLLIN;
    new_client.revents = 0;
    fd_poll.push_back(new_client);

    std::cout << "Client " << fd << " connected!" << std::endl;
}

void Server::recvData(int fd)
{
    char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);

	if (bytes <= 0)
    {
		std::cout << "Client " << fd << " disconnected" << std::endl;
		remove_client(fd);
        close(fd);
        return;
	}

    Client *client = get_client(fd);
    client->append_buffer(buff);

    std::string &buf = client->get_buffer();
    size_t pos;

    while ((pos = buf.find("\r\n")) != std::string::npos)
    {
        std::string cmd = buf.substr(0, pos);
        buf.erase(0, pos + 2);
        if (!cmd.empty())
            handle_cmd(cmd, fd);
    }
}

void Server::handle_cmd(std::string &cmd, int fd)
{
    std::vector<std::string> args = split_cmd(cmd);

    std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::toupper);

    if (args[0] == "PASS")
        cmd_pass(fd, args);
    else if (args[0] == "NICK")
        cmd_nick(fd, args);
    else if (args[0] == "USER")
        cmd_user(fd, args);
    else
        std::cout << "Unknown command: " << args[0] << std::endl;
        // Implement rest of the commands
}

// ============ UTILS ============ 
void Server::close_fd()
{
	for (size_t i = 0; i < clients.size(); i++)
        close(clients[i].get_clientfd());

	if (socketfd != -1)
		close(socketfd);

    std::cout << "\nDisconnecting all clients and closing server..." << std::endl;
}

void Server::remove_client(int fd)
{
	for(std::vector<struct pollfd>::iterator it = fd_poll.begin(); it != fd_poll.end(); ++it)
    {
		if (it->fd == fd)
		{
            fd_poll.erase(it);
            break;
        }
	}

	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->get_clientfd() == fd)
        {
            clients.erase(it);
            break;
        }
    }
}

bool Server::nick_in_use(std::string &nick)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].get_nick() == nick)
            return true;
    }
    return false;
}
