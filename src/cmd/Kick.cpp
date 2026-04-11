#include "../../inc/Server.hpp"

void Server::cmd_kick(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	std::string nick = client->get_nick();

	// 1. Validação de parâmetros (Canais e Usuários são obrigatórios)
	if (args.size() < 3) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "KICK"), fd);
		return ;
	}

	std::string channelName = args[1];
	std::string targetNick = args[2];
	std::string reason = (args.size() > 3) ? args[3] : "No reason given";

	// 2. Verificar se o canal existe
	if (_channels.find(channelName) == _channels.end()) {
		send_rpl(ERR_NOSUCHCHANNEL(nick, channelName), fd);
		return ;
	}

	Channel &chan = _channels.at(channelName);

	// 3. Verificar se QUEM ESTÁ CHUTANDO é operador
	if (!chan.isOperator(fd)) {
		send_rpl(ERR_CHANOPRIVSNEEDED(nick, channelName), fd);
		return ;
	}

	// 4. Verificar se o ALVO está no canal
	int targetFd = -1;
	const std::vector<Client *> &members = chan.getMembers();
	for (size_t i = 0; i < members.size(); i++) {
		if (members[i]->get_nick() == targetNick) {
			targetFd = members[i]->get_clientfd();
			break;
		}
	}

	if (targetFd == -1) {
		send_rpl(ERR_USERNOTINCHANNEL(nick, targetNick, channelName), fd);
		return ;
	}

	// 5. Executar o KICK
	// Formato: :nick!user@host KICK #canal alvo :razão
	std::string kickMsg = ":" + client->get_prefix() + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";

	// O broadcast avisa a todos, inclusive o que foi chutado, para ele fechar a aba
	chan.broadcast(kickMsg);

	// Remove o alvo do canal
	chan.removeMember(targetFd);

	// 6. Autolimpeza (mesma lógica do PART/QUIT)
	if (chan.getMembers().empty())
		_channels.erase(channelName);
}