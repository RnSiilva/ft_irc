#ifndef REPLIES_HPP
#define REPLIES_HPP

/* ---RPL--- */ 
// (001) - Mensagem de boas-vindas ao servidor
#define RPL_WELCOME(nick, user, host) ":ircserv 001 " + nick + " :Welcome to the IRC Network, " + nick + "!" + user + "@" + host + "\r\n"

// (324) - Indica os modos atuais do canal
#define RPL_CHANNELMODEIS(nick, channel, mode, mode_params) ":ircserv 324 " + nick + " " + channel + " +" + mode + mode_params + "\r\n"

// (331) - Canal sem tópico/assunto definido
#define RPL_NOTOPIC(nick, channel) ":ircserv 331 " + nick + " " + channel + " :No topic is set\r\n"

// (332) - Tópico/assunto do canal
#define RPL_TOPIC(nick, channel, topic) ":ircserv 332 " + nick + " " + channel + " :" + topic + "\r\n"

// (341) - Confirmação de convite enviado a um usuário
#define RPL_INVITING(nick, target, channel) ":ircserv 341 " + nick + " " + target + " " + channel + "\r\n"

// (353) - Listar os usuários no canal
#define RPL_NAMREPLY(nick, channel, users) ":ircserv 353 " + nick + " = " + channel + " :" + users + "\r\n"

// (366) - Indica que a lista de nomes terminou
#define RPL_ENDOFNAMES(nick, channel) ":ircserv 366 " + nick + " " + channel + " :End of /NAMES list\r\n"


/* ---ERR--- */ 
// (401) - O nickname ou canal indicado não existe
#define ERR_NOSUCHNICK(nick, target) ":ircserv 401 " + nick + " " + target + " :No such nick/channel\r\n"

// (403) - O nome do canal indicado não existe
#define ERR_NOSUCHCHANNEL(nick, channel) ":ircserv 403 " + nick + " " + channel + " :No such channel\r\n"

// (404) - Não é possível enviar mensagens para o canal
#define ERR_CANNOTSENDTOCHAN(nick, target) ":ircserv 404 " + nick + " " + target + " :Cannot send to channel\r\n"

// (411) - Nenhum destinatário foi especificado para o comando
#define ERR_NORECIPIENT(nick, cmd) ":ircserv 411 " + nick + " :No recipient given " + cmd + "\r\n"

// (412) - Nenhuma mensagem para enviar
#define ERR_NOTEXTTOSEND(nick) ":ircserv 412 " + nick + " :No text to send\r\n"

// (421) - O comando enviado não é reconhecido pelo servidor
#define ERR_UNKNOWNCOMMAND(client, cmd) ":ircserv 421 " + client + " " + cmd + " :Unknown command\r\n"

// (431) - Nenhum nickname foi fornecido
#define ERR_NONICKNAMEGIVEN(client) ":ircserv 431 " + client + " :No nickname given\r\n"

// (432) - O nickname contém caracteres inválidos
#define ERR_ERRONEUSNICKNAME(client, nick) ":ircserv 432 " + client + " " + nick + " :Erroneus nickname\r\n"

// (433) - O nickname já está a ser utilizado por outro usuário
#define ERR_NICKNAMEINUSE(client, nick) ":ircserv 433 " + client + " " + nick + " :Nickname is already in use\r\n"

// (441) - O usuário alvo não está no canal especificado
#define ERR_USERNOTINCHANNEL(nick, target, channel) ":ircserv 441 " + nick + " " + target + " " + channel + " :They aren't on that channel\r\n"

// (442) - Você não está no canal especificado
#define ERR_NOTONCHANNEL(nick, channel) ":ircserv 442 " + nick + " " + channel + " :You're not on that channel\r\n"

// (443) - O usuário já faz parte do canal
#define ERR_USERONCHANNEL(nick, target, channel) ":ircserv 443 " + nick + " " + target + " " + channel + " :is already on channel\r\n"

// (451) - O cliente ainda não completou o registro no servidor
#define ERR_NOTREGISTERED(client) ":ircserv 451 " + client + " :You have not registered\r\n"

// (461) - Parâmetros insuficientes para o comando
#define ERR_NEEDMOREPARAMS(client, cmd) ":ircserv 461 " + client + " " + cmd + " :Not enough parameters\r\n"

// (462) - O usuário já está registrado e não pode repetir o processo
#define ERR_ALREADYREGISTERED(client) ":ircserv 462 " + client + " :You may not reregister\r\n"

// (464) - A senha fornecida está incorreta
#define ERR_PASSWDMISMATCH(client) ":ircserv 464 " + client + " :Password incorrect\r\n"

// (471) - Limite de usuários do canal atingido (+l)
#define ERR_CHANNELISFULL(nick, channel) ":ircserv 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n"

// (472) - O caractere de modo indicado é desconhecido
#define ERR_UNKNOWNMODE(nick, mode) ":ircserv 472 " + nick + " " + mode + " :is unknown mode char to me\r\n"

// (473) - O canal é apenas para convidados (+i)
#define ERR_INVITEONLYCHAN(nick, channel) ":ircserv 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"

// (475) - Senha do canal incorreta ou ausente (+k)
#define ERR_BADCHANNELKEY(nick, channel) ":ircserv 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n"

// (482) - Privilégios de operador de canal necessários
#define ERR_CHANOPRIVSNEEDED(nick, channel) ":ircserv 482 " + nick + " " + channel + " :You're not channel operator\r\n"

// (696) - O parâmetro fornecido para o modo é inválido
#define ERR_INVALIDMODEPARAM(nick, channel, mode, param) ":ircserv 696 " + nick + " " + channel + " " + mode + " " + param + " :Invalid mode parameter" + "\r\n"

#endif