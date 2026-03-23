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
