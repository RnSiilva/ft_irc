#include "../../inc/Server.hpp"

void	Server::cmd_join(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	std::string nick = client->get_nick();

	// 1. Validação de parâmetros
	if (args.size() < 2) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "JOIN"), fd);
		return ;
	}

    // 2. Extrair nome (ex: #c1) e suporte a múltiplos canais (ex: JOIN #c1,#c2)
	std::stringstream ss_chan(args[1]);
	std::string channelName;

	// Suporte a senhas (Opcional agora, mas bom ter no radar)
    std::stringstream ss_key("");
    if (args.size() > 2) ss_key.str(args[2]);
    std::string key;

	while (std::getline(ss_chan, channelName, ','))
	{
		// Garante que o nome comece com #
		if (channelName[0] != '#')
			channelName = '#' + channelName;

		// 3. Lógica de criação/entrada
		if (_channels.find(channelName) == _channels.end()) {
			// Canal novo: Cria e define o primeiro como Operador
			Channel newChannel(channelName);
			_channels.insert(std::make_pair(channelName, newChannel));
		
			_channels.at(channelName).addMember(client);
			_channels.at(channelName).addOperator(client);
		} else {
			// Canal existente: Apenas entra
			// Aqui futuramente checaremos MODE +i, +k, +l
			_channels.at(channelName).addMember(client);
		}
	}

	// TUDO ISSO DEVE ESTAR DENTRO DO WHILE
	Channel &chan = _channels.at(channelName);

	// 4. Broadcast: :nick!user@host JOIN #canal
    // Avisa a TODO MUNDO do canal (inclusive quem entrou)
	std::string joinMsg = ":" + client->get_prefix() + " JOIN " + channelName + "\r\n";
	chan.broadcast(joinMsg);

	// 5. INFORMAÇÕES DO CANAL (O que o cliente precisa para abrir a aba)
    // Topic
	std::string topic = chan.getTopic();
	if (topic.empty())
		send_rpl(RPL_TOPIC(nick, channelName, "No Topic is set"), fd);
    else
		send_rpl(RPL_TOPIC(nick, channelName, topic), fd);
	
	// Manda a lista: "353 nick = #canal :@OpUser Member2 Member3"
    send_rpl(RPL_NAMREPLY(nick, channelName, chan.getMemberList()), fd);

    // 4. RPL_ENDOFNAMES (366)
    // Fecha a lista de nomes: "366 nick #canal :End of /NAMES list"
    send_rpl(RPL_ENDOFNAMES(nick, channelName), fd);
}