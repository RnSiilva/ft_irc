#include "../../inc/Server.hpp"

static void updateAppliedChanges(std::string &changes, bool adding, char mode)
{
	char last_sign = '\0';
	for (size_t i = 0; i < changes.length(); i++) {
		if (changes[i] == '+' || changes[i] == '-')
			last_sign = changes[i];
	}

	char current_sign = adding ? '+' : '-';
	if (last_sign != current_sign)
		changes += current_sign;
	changes += mode;
}

void Server::cmd_mode(int fd, std::vector<std::string> args)
{
	Client *client = get_client(fd);
	//if (!client) return;
	std::string nick = client->get_nick();

	if (args.size() < 2) {
		send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE"), fd);
		return ;
	}

	std::string target = args[1];
	if (_channels.find(target) == _channels.end()) {
		send_rpl(ERR_NOSUCHCHANNEL(nick, target), fd);
		return ;
	}

	Channel &chan = _channels.at(target);

	// 1. Se só enviou o nome do canal, quer VER os modos (RPL_CHANNELMODEIS)
	if (args.size() == 2) {
		std::string modes = chan.getModes();
		std::string params = "";

		if (chan.hasMode('k'))
			params += " " + chan.getKey();
		if (chan.hasMode('l')) {
			std::stringstream ss;
			ss << chan.getLimit();
			params += " " + ss.str();
		}

		send_rpl(RPL_CHANNELMODEIS(nick, target, modes, params), fd );
		return ;
	}

	// 2. Verificação de Operador (Só Ops mudam modos)
	if (!chan.isOperator(fd)) {
		send_rpl(ERR_CHANOPRIVSNEEDED(nick, target), fd);
		return ;
	}

	// 3. O Coração do MODE: Parsing das Flags
	std::string modes = args[2];
	size_t param_idx = 3; // Os parâmetros extras (se houver)
	bool adding = true; // Controla se estamos em modo + ou -
	std::string changesStr = ""; // Para o broadcast final
	std::string paramsStr = "";

	for (size_t i = 0; i < modes.length(); i++) {
		char c = modes[i];
		if (c == '+') { adding = true; continue; }
		if (c == '-') { adding = false; continue; }

		// if (c == 'i' || c == 't' || c == 'k') {
		// 	// Adiciona o sinal apenas se ele mudou ou se é a primeira flag
		// 	char symbol = (adding ? '+' : '-');
		// 	// if (changesStr.empty() || changesStr[changesStr.length() - 1] != symbol) {
		// 	// 	changesStr += symbol;
		// 	// }
		// 	if (changesStr.find(symbol) == std::string::npos || (i > 0 && modes[i-1] != '+' && modes[i-1] != '-'))
		// 		changesStr += symbol;
			
		// 	changesStr += c;
		// }

		// if (i == 0 && c != '+' && c != '-') {
		// 	send_rpl(ERR_UNKNOWNMODE(nick, c), fd);
		// 	continue;
		// }
		if (modes[0] != '+' && modes[0] != '-') {
			send_rpl(ERR_UNKNOWNMODE(nick, c), fd);
			continue;
		}
		if (c == 'i') {
			if (chan.hasMode('i') != adding) {
				chan.setModeInvite(adding); // true para +, false para -
				updateAppliedChanges(changesStr, adding, 'i');
			}
		} 
		else if (c == 't') {
			if (chan.hasMode('t') != adding) {
				chan.setModeTopic(adding);
				updateAppliedChanges(changesStr, adding, 't');
			}
		}
		else if (c == 'k') {
			if (adding) {
				if (param_idx < args.size()) {
					std::string newKey = args[param_idx++];
					// Só muda se a senha for diferente da atual
                    if (chan.getKey() != newKey) {
						chan.setKey(newKey);
						chan.setModeKey(true);
						updateAppliedChanges(changesStr, true, 'k');
						paramsStr += " " + newKey; // Para o broadcast
					}
				} else {
					send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE +k"), fd);
					//changesStr.erase(changesStr.end() -1);
					continue ;
				}
			} else {
				if (chan.hasMode('k')) {
					chan.setKey("");
					chan.setModeKey(false);
					updateAppliedChanges(changesStr, false, 'k');
				}
			}
		}
		else if (c == 'l') {
			if (adding) {
				if (param_idx < args.size()) {
					std::string val = args[param_idx++];
					// Só muda se o limite for diferente do atual
					int newLimit = std::atoi(val.c_str());
					if (newLimit == 0) {
						send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE +l"), fd);
						continue ;
					}
					else if (newLimit > 0 && (size_t)newLimit != chan.getLimit()) {
						chan.setLimit(newLimit);
						chan.setModeLimit(true);
						updateAppliedChanges(changesStr, true, 'l');
						paramsStr += " " + val;
					}
				} else {
					send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE +l"), fd);
					continue;
				}
			} else {
				if (chan.hasMode('l')) {
					chan.setModeLimit(false);
					updateAppliedChanges(changesStr, false, 'l');
				}
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
					send_rpl(ERR_USERNOTINCHANNEL(nick, targetNick, target), fd);
					continue;
				}
				// 3. Execução
				else {
					bool isOp = chan.isOperator(targetClient->get_clientfd());
					if (isOp != adding) {
						if (adding) chan.addOperator(targetClient);
						else chan.removeOperator(targetClient->get_clientfd());

						updateAppliedChanges(changesStr, adding, 'o');
						paramsStr += " " + targetNick;
					}
				}
			} else {
				send_rpl(ERR_NEEDMOREPARAMS(nick, "MODE +o"), fd);
				continue;
			}
		}
		else {
			send_rpl(ERR_UNKNOWNMODE(nick, std::string(1, c)), fd);
			continue;
		}
	}

	if (!changesStr.empty()) {
		std::string modeMsg = ":" + client->get_prefix() + " MODE " + target + " " + changesStr + paramsStr + "\r\n";
		chan.broadcast(modeMsg);
	}
}