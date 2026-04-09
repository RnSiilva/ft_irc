#include "../../inc/Server.hpp"

void Server::cmd_quit(int fd, std::vector<std::string> args)
{
    Client *client = get_client(fd);

    if (!client->get_registered())
    {
        remove_client(fd);
        close(fd);
        return;
    }

    std::string reason = args.size() > 1 ? args[1] : "Leaving";
    if (reason[0] == ':')
        reason = reason.substr(1);
    std::string msg = ":" + client->get_prefix() + " QUIT :" + reason + "\r\n";

    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].is_member(client))
        {
            channels[i].broadcast(msg, fd);
            channels[i].remove_member(client);
        }
    }

    send_rpl(msg, fd);
    remove_client(fd);
    close(fd);
}