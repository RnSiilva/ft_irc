#include "../../inc/Server.hpp"

void Server::cmd_invite(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	std::string nick = client->get_nick();

	// 1. Validação de parâmetros
	if (args.size() < 3) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "INVITE"), fd);
		return ;
	}

	std::string targetNick = args[1];
	std::string channelName = args[2];

	// 2. O alvo existe no servidor?
	int targetFd = -1;
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second.get_nick() == targetNick) {
			targetFd = it->first;
			break;
		}
	}
	if (targetFd == -1) {
		send_rpl(ERR_NOSUCHNICK(nick, targetNick), fd);
		return ;
	}

	// 3. O canal existe?
	if (_channels.find(channelName) == _channels.end()) {
		send_rpl(ERR_NOSUCHCHANNEL(nick, channelName), fd);
		return ;
	}

	Channel &chan = _channels.at(channelName);

	// 4. O remetente está no canal? (Regra do RFC)
	if (!chan.isClientInChannel(fd)) {
		send_rpl(ERR_NOTONCHANNEL(nick, channelName), fd);
		return ;
	}

	// 5. O alvo já está no canal?
	if (chan.isClientInChannel(targetFd)) {
		send_rpl(ERR_USERONCHANNEL(nick, targetNick, channelName), fd);
		return ;
	}

	// 7. EXECUTAR O CONVITE
	// Adiciona à lista interna do canal para permitir o JOIN futuro se for +i
	chan.addInvite(targetFd);

	// Notifica o Alvo: :nick!user@host INVITE alvo :#canal
	std::string inviteMsg = ":" + client->get_prefix() + " INVITE " + targetNick + " :" + channelName + "\r\n";
	send_rpl(inviteMsg, targetFd);

	// Confirma para o Remetente
	send_rpl(RPL_INVITING(nick, targetNick, channelName), fd);
}