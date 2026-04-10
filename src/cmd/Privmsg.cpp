#include "../../inc/Server.hpp"

void Server::cmd_privmsg(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	std::string nick = client->get_nick();

	// 1. Validação: Precisa de alvo e mensagem
	if (args.size() < 2) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "PRIVMSG"), fd);
		return ;
	}
	if (args.size() < 3) {
		send_rpl(ERR_NOTEXTTOSEND(nick), fd);
		return ;
	}

	std::string target = args[1];
	std::string message = args[2]; // O split_cmd já removeu o ':' se existir

	// 2. Se o alvo for um CANAL
	if (target[0] == '#') {
		if (_channels.find(target) == _channels.end()) {
			send_rpl(ERR_NOSUCHCHANNEL(nick, target), fd);
			return ;
	}

	Channel &chan = _channels.at(target);
	// Opcional: Verificar se o cliente é membro do canal antes de deixar ele falar
	if (!chan.isClientInChannel(fd)) {
		send_rpl(ERR_CANNOTSENDTOCHAN(nick, target), fd);
		return ;
	}

	// Formato: :nick!user@host PRIVMSG #canal :mensagem
	std::string fullMsg = ":" + client->get_prefix() + " PRIVMSG " + target + " :" + message + "\r\n";

	// No canal, mandamos para todos EXCETO para quem enviou
	chan.broadcast(fullMsg, fd);
	}

	// 3. Se o alvo for um USUÁRIO (Mensagem Privada)
	else {
		int targetFd = -1;
		for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
			if (it->second.get_nick() == target) {
				targetFd = it->first;
				break ;
			}
		}

		if (targetFd == -1) {
			send_rpl(ERR_NOSUCHNICK(nick, target), fd);
			return ;
		}

		std::string fullMsg = ":" + client->get_prefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		send_rpl(fullMsg, targetFd);
	}
}