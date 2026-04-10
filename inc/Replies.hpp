#ifndef REPLIES_HPP
#define REPLIES_HPP

// RPL
#define RPL_WELCOME(nick, user, host) "001 " + nick + " :Welcome to the IRC Network, " + nick + "!" + user + "@" + host + "\r\n"

// (332) - Enviado ao entrar para mostrar o assunto do canal
#define RPL_TOPIC(nick, channel, topic) "332 " + nick + " " + channel + " :" + topic + "\r\n"

// (353) - Enviado para listar os usuários no canal
// O '=' indica que o canal é público
#define RPL_NAMREPLY(nick, channel, users) "353 " + nick + " = " + channel + " :" + users + "\r\n"

// (366) - Indica que a lista de nomes terminou
#define RPL_ENDOFNAMES(nick, channel) "366 " + nick + " " + channel + " :End of /NAMES list\r\n"



// ERR
#define ERR_NEEDMOREPARAMS(client, cmd) "461 " + client + " " + cmd + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(client) "462 " + client + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(client) "464 " + client + " :Password incorrect\r\n"
#define ERR_NOTREGISTERED(client) "451 " + client + " :You have not registered\r\n"
#define ERR_NONICKNAMEGIVEN(client) "431 " + client + " :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(client, nick) "433 " + client + " " + nick + " :Nickname is already in use\r\n"
#define ERR_ERRONEUSNICKNAME(client, nick) "432 " + client + " " + nick + " :Erroneus nickname\r\n"
#define ERR_UNKNOWNCOMMAND(client, cmd) "421 " + client + " " + cmd + " :Unknown command\r\n"

// (412) - Nenhuma mensagem para enviar
#define ERR_NOTEXTTOSEND(nick) "412" + nick + " :No text to send\r\n"

#define ERR_CANNOTSENDTOCHAN(nick, target) "404 " + nick + " " + target + " :Cannot send to channel\r\n"

#define ERR_NOSUCHNICK(nick, target) "401 " + nick + " " + target + " :No such nick/channel\r\n"

#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel\r\n"

#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You're not on that channel\r\n"

#endif