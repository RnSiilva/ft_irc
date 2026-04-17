#ifndef REPLIES_HPP
#define REPLIES_HPP

// RPL
#define RPL_WELCOME(nick, user, host) ":ircserv 001 " + nick + " :Welcome to the IRC Network, " + nick + "!" + user + "@" + host + "\r\n"

// (332) - Enviado ao entrar para mostrar o assunto do canal
#define RPL_TOPIC(nick, channel, topic) ":ircserv 332 " + nick + " " + channel + " :" + topic + "\r\n"

// (353) - Enviado para listar os usuários no canal
// O '=' indica que o canal é público
#define RPL_NAMREPLY(nick, channel, users) ":ircserv 353 " + nick + " = " + channel + " :" + users + "\r\n"

// (366) - Indica que a lista de nomes terminou
#define RPL_ENDOFNAMES(nick, channel) ":ircserv 366 " + nick + " " + channel + " :End of /NAMES list\r\n"

#define RPL_INVITING(nick, target, channel) ":ircserv 341 " + nick + " " + target + " " + channel + "\r\n"

// RPL_NOTOPIC (331) - Quando o canal não tem assunto definido
#define RPL_NOTOPIC(nick, channel) ":ircserv 331 " + nick + " " + channel + " :No topic is set\r\n"




// ERR
#define ERR_NEEDMOREPARAMS(client, cmd) ":ircserv 461 " + client + " " + cmd + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(client) ":ircserv 462 " + client + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(client) ":ircserv 464 " + client + " :Password incorrect\r\n"
#define ERR_NOTREGISTERED(client) ":ircserv 451 " + client + " :You have not registered\r\n"
#define ERR_NONICKNAMEGIVEN(client) ":ircserv 431 " + client + " :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(client, nick) ":ircserv 433 " + client + " " + nick + " :Nickname is already in use\r\n"
#define ERR_ERRONEUSNICKNAME(client, nick) ":ircserv 432 " + client + " " + nick + " :Erroneus nickname\r\n"
#define ERR_UNKNOWNCOMMAND(client, cmd) ":ircserv 421 " + client + " " + cmd + " :Unknown command\r\n"

// (412) - Nenhuma mensagem para enviar
#define ERR_NOTEXTTOSEND(nick) ":ircserv 412 " + nick + " :No text to send\r\n"

#define ERR_CANNOTSENDTOCHAN(nick, target) ":ircserv 404 " + nick + " " + target + " :Cannot send to channel\r\n"

#define ERR_NOSUCHNICK(nick, target) ":ircserv 401 " + nick + " " + target + " :No such nick/channel\r\n"

#define ERR_NOSUCHCHANNEL(nick, channel) ":ircserv 403 " + nick + " " + channel + " :No such channel\r\n"

#define ERR_NOTONCHANNEL(nick, channel) ":ircserv 442 " + nick + " " + channel + " :You're not on that channel\r\n"

#define ERR_CHANOPRIVSNEEDED(nick, channel) ":ircserv 482 " + nick + " " + channel + " :You're not channel operator\r\n"

#define ERR_USERNOTINCHANNEL(nick, target, channel) ":ircserv 441 " + nick + " " + target + " " + channel + " :They aren't on that channel\r\n"

#define ERR_USERONCHANNEL(nick, target, channel) ":ircserv 443 " + nick + " " + target + " " + channel + " :is already on channel\r\n"

#define ERR_BADCHANNELKEY(nick, channel) ":ircserv 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n"

#define ERR_CHANNELISFULL(nick, channel) ":ircserv 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n"

#define ERR_INVITEONLYCHAN(nick, channel) ":ircserv 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"

#define ERR_UNKNOWNMODE(nick, mode) ":ircserv 472 " + nick + " " + mode + " :is unknown mode char to me\r\n"

#endif