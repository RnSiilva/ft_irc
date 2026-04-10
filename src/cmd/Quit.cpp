#include "../../inc/Server.hpp"

void Server::disconnect_client(int fd, std::string reason)
{
    Client *client = get_client(fd);
	if (!client) return;

    std::string nick = client->get_nick().empty() ? "*" : client->get_nick();
    std::string quit_msg = ":" + client->get_prefix() + " QUIT :" + reason + "\r\n";

	// Usamos um iterador para percorrer o mapa de canais com segurança
	std::map<std::string, Channel>::iterator it = _channels.begin();
    while (it != _channels.end()) {
        Channel &channel = it->second;

        if (channel.isClientInChannel(fd))
        {
			// 1. Avisa os outros membros do canal que o cliente saiu
            channel.broadcast(quit_msg, fd);

			// 2. Remove o cliente da lista interna do canal
            channel.removeMember(fd);

			// 3. Se o canal ficou vazio, ele deve ser deletado
            if (channel.getMemberList().empty())
            {
				// No C++98, erase(it++) é o truque para deletar e avançar o iterador com segurança
                _channels.erase(it++);
                continue;
            }
        }
		++it;
    }

    std::cout << "Client " << fd << " disconnected (" << reason << ")" << std::endl;
    remove_client(fd);
    close(fd);
}

void Server::cmd_quit(int fd, std::vector<std::string> args)
{
    std::string reason = " ";
    if (args.size() > 1)
        reason = args[1];

    disconnect_client(fd, reason);
}
