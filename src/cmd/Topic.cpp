#include "../../inc/Server.hpp"

void Server::cmd_topic(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	std::string nick = client->get_nick();

	// 1. Validação: Precisa de pelo menos o canal
	if (args.size() < 2) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "TOPIC"), fd);
		return ;
	}

	std::string channelName = args[1];
	if (_channels.find(channelName) == _channels.end()) {
		send_rpl(ERR_NOSUCHCHANNEL(nick, channelName), fd);
		return ;
	}

	Channel &chan = _channels.at(channelName);

	// 2. Verificar se o usuário está no canal (Regra do RFC)
	if (!chan.isClientInChannel(fd)) {
		send_rpl(ERR_NOTONCHANNEL(nick, channelName), fd);
		return ;
	}

	// 3. CASO A: Visualizar o Tópico (Ex: TOPIC #porto)
	if (args.size() == 2) {
		if (chan.getTopic().empty())
			send_rpl(RPL_NOTOPIC(nick, channelName), fd);
		else
			send_rpl(RPL_TOPIC(nick, channelName, chan.getTopic()), fd);
	}
	// 4. CASO B: Alterar o Tópico (Ex: TOPIC #porto :Novo assunto)
	else {
		// Futuramente: if (chan.hasMode('t') && !chan.isOperator(fd)) -> Erro 482

		std::string newTopic = args[2];
		chan.setTopic(newTopic);

		// Notificar TODOS no canal sobre a mudança
		// Formato: :nick!user@host TOPIC #canal :Novo assunto
		std::string topicMsg = ":" + client->get_prefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
		chan.broadcast(topicMsg);
	}
}