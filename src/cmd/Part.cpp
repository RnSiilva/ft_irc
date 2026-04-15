#include "../../inc/Server.hpp"

void Server::cmd_part(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	std::string nick = client->get_nick();

	if (args.size() < 2) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "PART"), fd);
		return ;
	}

	std::string reason = (args.size() > 2) ? args[2] : "Leaving";
	std::stringstream ss(args[1]);
	std::string channelName;

	// 1. Suporte a múltiplos canais (separados por vírgula)
	while (std::getline(ss, channelName, ',')) {
		// Verificar se o canal existe
		if (_channels.find(channelName) == _channels.end()) {
			send_rpl(ERR_NOSUCHCHANNEL(nick, channelName), fd);
			continue ;
		}

		Channel &chan = _channels.at(channelName);

		// Verificar se o usuário está no canal
		if (!chan.isClientInChannel(fd)) {
			send_rpl(ERR_NOTONCHANNEL(nick, channelName), fd);
			continue ;
		}

		// 2. O Broadcast da saída
		// Formato: :nick!user@host PART #canal :razão
		std::string partMsg = ":" + client->get_prefix() + " PART " + channelName + " :" + reason + "\r\n";
		chan.broadcast(partMsg);

		// 3. Remoção física
		chan.removeMember(fd);

		// 4. Autolimpeza: se o canal ficou vazio, remove do servidor
		if(chan.getMembers().empty())
			_channels.erase(channelName);
	}
}