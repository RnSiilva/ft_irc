#include "../inc/Server.hpp"

std::vector<std::string> split_cmd(std::string &cmd)
{
    std::vector<std::string> args;
    std::istringstream ss(cmd);
    std::string token;
    while (ss >> token)
        args.push_back(token);
    return args;
}

void send_rpl(std::string rpl, int fd)
{
    if (send(fd, rpl.c_str(), rpl.size(), 0) == -1)
        std::cerr << "send() failed for client " << fd << std::endl;
}

bool invalid_nick(std::string &nick)
{
    if (nick.empty())
        return true;
    if (nick[0] == '#' || nick[0] == ':')
        return true;
    for (size_t i = 0; i < nick.size(); i++)
    {
        if (!std::isalnum(nick[i]) && nick[i] != '[' && nick[i] != ']' &&
            nick[i] != '{' && nick[i] != '}' && nick[i] != '\\' && nick[i] != '|')
            return true;
    }
    return false;
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

void Server::close_fd()
{
	for (size_t i = 0; i < clients.size(); i++)
        close(clients[i].get_clientfd());

	if (socketfd != -1)
		close(socketfd);

    std::cout << "\nDisconnecting all clients and closing server..." << std::endl;
}
