#include "../../inc/Server.hpp"

void Server::disconnect_client(int fd, std::string reason)
{
    Client *client = get_client(fd);

    std::string nick = client->get_nick().empty() ? "*" : client->get_nick();
    std::string quit_msg = ":" + nick + " QUIT :" + reason + "\r\n";

    for (size_t i = 0; i < channels.size(); i++)
    {
        Channel &channel = channels[i];

        if (channel.is_member(client))
        {
            channel.broadcast(quit_msg, fd);
            channel.remove_member(client);

            if (channel.is_empty())
            {
                channels.erase(channels.begin() + i);
                i--;
            }
        }
    }

    std::cout << "Client " << fd << " disconnected (" << reason << ")" << std::endl;
    remove_client(fd);
    close(fd);
}

void Server::cmd_quit(int fd, std::vector<std::string> args)
{
    std::string reason = " ";
    if (args.size() > 1)
        reason = args[1];

    disconnect_client(fd, reason);
}
