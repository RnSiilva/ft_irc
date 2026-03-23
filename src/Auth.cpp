#include "../inc/Server.hpp"

void Server::cmd_pass(int fd, std::vector<std::string> args)
{
    Client *client = get_client(fd);
    std::string nick = client->get_nick().empty() ? "*" : client->get_nick();

    if (args.size() < 2)
    {
        send_rpl(ERR_NEEDMOREPARAMS(nick, args[0]), fd);
        return;
    }
    if (client->get_pwd_status())
    {
        send_rpl(ERR_ALREADYREGISTERED(nick), fd);
        return;
    }
    if (args[1] == password)
        client->set_pwd_status(true);
    else
        send_rpl(ERR_PASSWDMISMATCH(nick), fd);
}

void Server::cmd_nick(int fd, std::vector<std::string> args)
{
    Client *client = get_client(fd);
    std::string nick = client->get_nick().empty() ? "*" : client->get_nick();

    if (!client->get_pwd_status())
    {
        send_rpl(ERR_NOTREGISTERED(nick), fd);
        return;
    }
    if (args.size() < 2)
    {
        send_rpl(ERR_NONICKNAMEGIVEN(nick), fd);
        return;
    }
    if (nick_in_use(args[1]) && args[1] != nick)
    {
        send_rpl(ERR_NICKNAMEINUSE(nick, args[1]), fd);
        return;
    }
    if (invalid_nick(args[1]))
    {
        send_rpl(ERR_ERRONEUSNICKNAME(nick, args[1]), fd);
        return;
    }

    client->set_nick(args[1]);
    std::string msg = ":" + nick + "!" + client->get_user() + "@" + client->get_host() + " NICK " + args[1] + "\r\n";
    send_rpl(msg, fd);

    if (!client->get_user().empty() && !client->get_registered())
    {
        client->set_registered(true);
        send_rpl(RPL_WELCOME(args[1], client->get_user(), client->get_host()), fd);
    }
}

void Server::cmd_user(int fd, std::vector<std::string> args)
{
    Client *client = get_client(fd);
    std::string nick = client->get_nick().empty() ? "*" : client->get_nick();

    if (!client->get_pwd_status())
    {
        send_rpl(ERR_NOTREGISTERED(nick), fd);
        return;
    }
    if (args.size() < 5)
    {
        send_rpl(ERR_NEEDMOREPARAMS(nick, args[0]), fd);
        return;
    }
    if (!client->get_user().empty())
    {
        send_rpl(ERR_ALREADYREGISTERED(nick), fd);
        return;
    }

    client->set_user(args[1]);

    if (!client->get_nick().empty() && !client->get_registered())
    {
        client->set_registered(true);
        send_rpl(RPL_WELCOME(nick, client->get_user(), client->get_host()), fd);
    }
}
