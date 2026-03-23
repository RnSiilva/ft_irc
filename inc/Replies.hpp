#ifndef REPLIES_HPP
#define REPLIES_HPP

// RPL
#define RPL_WELCOME(nick, user, host) "001 " + nick + " :Welcome to the IRC Network, " + nick + "!" + user + "@" + host + "\r\n"

// ERR
#define ERR_NEEDMOREPARAMS(client, cmd) "461 " + client + " " + cmd + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(client) "462 " + client + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(client) "464 " + client + " :Password incorrect\r\n"
#define ERR_NOTREGISTERED(client) "451 " + client + " :You have not registered\r\n"
#define ERR_NONICKNAMEGIVEN(client) "431 " + client + " :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(client, nick) "433 " + client + " " + nick + " :Nickname is already in use\r\n"
#define ERR_ERRONEUSNICKNAME(client, nick) "432 " + client + " " + nick + " :Erroneus nickname\r\n"

#endif