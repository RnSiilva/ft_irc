#include "../../inc/Server.hpp"

void	Server::cmd_join(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	// if (!client)
	// 	return;
	std::string nick = client->get_nick();

	if (args.size() < 2) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "JOIN"), fd);
		return ;
	}

 	// 1. Parsing de canais e senhas
	std::vector<std::string> chans = ft_split(args[1], ',');
	std::vector<std::string> keys;
	if (args.size() > 2)
		keys = ft_split(args[2], ',');

	// 2. Loop principal
	for (size_t i = 0; i < chans.size(); ++i) {
		std::string chanName = chans[i];
		std::string key = (i < keys.size()) ? keys[i] : "";

		// Validação básica de nome
		if (chanName[0] != '#' || chanName.length() > 50 || chanName.length() < 2) {
			send_rpl(ERR_NOSUCHCHANNEL(nick, chanName), fd);
			continue ;
		}
		
		// 3. Lógica de criação/entrada
		if (_channels.find(chanName) == _channels.end())
			createChannel(chanName, client);

		Channel &chan = _channels.at(chanName);
		
		// 4. Verificação de permissões
		int error = chan.checkCanJoin(client, key);
		if (error != 0) {
			sendJoinError(fd, error, chanName, nick);
			continue;
		}

		// 5. Execução da entrada e Broadcast
		executeJoinActions(client, chan, nick, fd);
	}
}