#include "../../inc/Server.hpp"

void Server::cmd_mode(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	std::string nick = client->get_nick();

	// 1. Validação mínima
	if (args.size() < 2) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE"), fd);
		return ;
	}

	std::string target = args[1];

	// Ignoramos MODE para usuários (se o target não começar com #)
	if (target[0] != '#') return;

	if (_channels.find(target) == _channels.end()) {
		send_rpl(ERR_NOSUCHCHANNEL(nick, target), fd);
		return ;
	}

	Channel &chan = _channels.at(target);

	// 2. Se só enviou o nome do canal, ele quer VER os modos (RPL_CHANNELMODEIS)
	if (args.size() == 2) {
		send_rpl("324 " + nick + " " + target + " :+" + chan.getModes() + "\r\n", fd );
		return ;
	}

	// 3. Verificação de Operador (Só Ops mudam modos)
	if (!chan.isOperator(fd)) {
		send_rpl(ERR_CHANOPRIVSNEEDED(nick, target), fd);
		return ;
	}

	// 4. O Coração do MODE: Parsing das Flags
	std::string modes = args[2];
	size_t param_idx = 3; // Os parâmetros extras (se houver) começam no args[3]
	bool adding = true; // Controla se estamos em modo + ou -
	std::string applied_flags = ""; // Para o broadcast final
	std::string applied_params = "";

	for (size_t i = 0; i < modes.length(); i++) {
		char c = modes[i];
		if (c == '+') { adding = true; continue; }
		if (c == '-') { adding = false; continue; }

		if (c == 'i' || c == 't' || c == 'k') {
			// Adiciona o sinal apenas se ele mudou ou se é a primeira flag
			char symbol = (adding ? '+' : '-');
			// if (applied_flags.empty() || applied_flags[applied_flags.length() - 1] != symbol) {
			// 	applied_flags += symbol;
			// }
			if (applied_flags.find(symbol) == std::string::npos || (i > 0 && modes[i-1] != '+' && modes[i-1] != '-'))
				applied_flags += symbol;
			
			applied_flags += c;
		}
		if (c == 'i') chan.setModeInvite(adding); // true para +, false para -
		else if (c == 't') chan.setModeTopic(adding);
		else if (c == 'k') {
			if (adding) {
				// Precisamos de um parâmetro extra!
				if (param_idx < args.size()) {
					std::string newKey = args[param_idx++];
					chan.setKey(newKey);
					chan.setModeKey(true);
					applied_params += " " + newKey; // Para o broadcast
				} else {
					send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE +k"), fd);
					applied_flags.erase(applied_flags.end() -1);
					continue ;
				}
			} else {
				chan.setKey("");
				chan.setModeKey(false);
			}
		}
		else if (c == 'l') {
			if (adding) {
				if (param_idx < args.size()) {
					int newLimit = std::atoi(args[param_idx++].c_str());
					if (newLimit > 0) {
						chan.setLimit(static_cast<size_t>(newLimit));
						chan.setModeLimit(true);

						if (applied_flags.empty() || applied_flags[applied_flags.size() - 1] != '+')
							applied_flags += 'l';
		
						applied_params += " " + args[param_idx];
					}
					param_idx++;
				} else {
					send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE +l"), fd);
					continue;
				}
			} else {
				chan.setModeLimit(false);
				if (applied_flags.empty() || applied_flags[applied_flags.size() - 1] != '-')
                    applied_flags += "-";
                applied_flags += "l";
			}
		}
		else if (c == 'o') {
			if (param_idx < args.size()) {
				std::string targetNick = args[param_idx++];
				Client *targetClient = get_client_by_nick(targetNick);

				// 1. O usuário alvo existe no servidor?
				if (!targetClient) {
					send_rpl(ERR_NOSUCHNICK(nick, targetNick), fd);
					continue;
				}
				// 2. O usuário alvo está no canal?
				else if (!chan.isClientInChannel(targetClient->get_clientfd())) {
					send_rpl("441 " + nick + " " + targetNick + " " + target + " :They aren't on that channel\r\n", fd);
					continue;
				}
				// 3. Execução
				else {
					if (adding) {
						chan.addOperator(targetClient);
					} else {
						chan.removeOperator(targetClient->get_clientfd());
					}
					applied_flags += (adding ? "+" : "-");
					applied_params += "o";
					applied_params += " " + targetNick;
				}
			} else {
				send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE +o"), fd);
				continue;
			}
		}
	}

	if (!applied_flags.empty()) {
		std::string modeMsg = ":" + client->get_prefix() + " MODE " + target + " " + applied_flags + applied_params + "\r\n";
		chan.broadcast(modeMsg);
	}
}