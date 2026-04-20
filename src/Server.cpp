#include "../inc/Server.hpp"

Server::Server() : socketfd(-1)
{
	initHostname();
}
Server::~Server() {}

void Server::initHostname()
{
	char hostname[256];
	struct hostent *host_info;

	if (gethostname(hostname, sizeof(hostname)) == 0) {
		host_info = gethostbyname(hostname);
		if (host_info == NULL) {
			_hostname = "localhost";
			_ip = "127.0.0.1";
		}
		else {
			_hostname = hostname;
			_ip = inet_ntoa(*(struct in_addr *)host_info->h_addr);
		}	
	}
	else {
		_hostname = "localhost";
		_ip = "127.0.0.1";
	}
}

Client *Server::get_client(int fd)
{ 
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it != clients.end())
		return &(it->second); // Retorna o endereço do Client encontrado
	return NULL;
}

Client *Server::get_client_by_nick(std::string nick)
{
	std::map<int, Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (it->second.get_nick() == nick)
			return &(it->second);
	}
	return NULL;
}

void Server::createChannel(const std::string &name, Client *owner) {
	Channel newChannel(name);
	_channels.insert(std::make_pair(name, newChannel));

	// Acessando o canal recém criado para configurar
	Channel &chan = _channels.at(name);
	chan.addMember(owner);
	chan.addOperator(owner);
}

void Server::sendJoinError(int fd, int code, const std::string &chanName, const std::string &nick) {
    switch (code) {
        case 473:
            send_rpl(ERR_INVITEONLYCHAN(nick, chanName), fd);
            break;
        case 475:
            send_rpl(ERR_BADCHANNELKEY(nick, chanName), fd);
            break;
        case 471:
            send_rpl(ERR_CHANNELISFULL(nick, chanName), fd);
            break;
    }
}

void Server::executeJoinActions(Client* client, Channel &chan, const std::string &nick, int fd) {
    std::string chanName = chan.getName();

    // 1. Adiciona o membro e limpa convite se houver
    chan.addMember(client);
    if (chan.isInvited(fd))
        chan.removeInvite(fd);

    // 2. Broadcast para o canal (Avisa todo mundo, inclusive o próprio client)
    std::string joinMsg = ":" + client->get_prefix() + " JOIN " + chanName + "\r\n";
    chan.broadcast(joinMsg);

    // 3. Envia o Tópico
    if (!chan.getTopic().empty())
        send_rpl(RPL_TOPIC(nick, chanName, chan.getTopic()), fd);
    else
        send_rpl(RPL_NOTOPIC(nick, chanName), fd);

    // 4. Envia a lista de nomes (RPL_NAMREPLY + RPL_ENDOFNAMES)
    send_rpl(RPL_NAMREPLY(nick, chanName, chan.getMemberList()), fd);
    send_rpl(RPL_ENDOFNAMES(nick, chanName), fd);
}

// ============ SERVER CONFIGURATION ============ 

void Server::serverInfo(void)
{
	std::cout << "\t--Welcome to the IRC server!--" << std::endl;
	std::cout << std::left << std::setw(20) << "\t Server hostname: " << _hostname << std::endl;
	std::cout << std::left << std::setw(20) << "\t Server IP: " << _ip << std::endl;
	std::cout << std::left << std::setw(20) << "\t Server port: " << port << std::endl;
	std::cout << std::left << std::setw(20) << "\t Server password: " << password << std::endl << std::endl;
	std::cout << "Waiting connections..." << std::endl;
}

void Server::server_start(int port, std::string pwd)
{
    this->port = port;
    password = pwd;
    create_socketfd(); //-> create the server socket

	serverInfo();
    // std::cout << "Waiting connections..." << std::endl;
    while (signal == false)
    {
        if((poll(&fd_poll[0], fd_poll.size(), -1) == -1) && signal == false)
            throw(std::runtime_error("Failed to poll."));

        for (size_t i = 0; i < fd_poll.size(); i++)
		{
			if (fd_poll[i].revents & POLLIN)
			{
				if (fd_poll[i].fd == socketfd)
					accept_client();
				else {
					size_t current_size = fd_poll.size(); // Salva o tamanho atual
					recvData(fd_poll[i].fd);

					if (fd_poll.size() < current_size) // Se o tamanho diminuiu, um cliente saiu
						i--; // Volta um índice para não pular o próximo cliente da lista
				}
			}
		}
    }
    close_fd();
}

void Server::create_socketfd(void)
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
    if(socketfd == -1) //-> check if the socket is created
		throw(std::runtime_error("Failed to create socket."));

    int opt = 1;

	//-> set the socket option (SO_REUSEADDR) to reuse the address
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw(std::runtime_error("Failed to set option: (SO_REUSEADDR)."));

	//-> set the socket option (O_NONBLOCK) for non-blocking socket
    if (fcntl(socketfd, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("Failed to set option: (O_NONBLOCK)."));

    addr.sin_family = AF_INET; //-> set the address family to ipv4
    addr.sin_port = htons(this->port); //-> convert the port to network byte order (big endian)
    addr.sin_addr.s_addr = INADDR_ANY; //-> set the address to any local machine address

	//-> bind the socket to the address
    if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw(std::runtime_error("Failed to bind socket."));

	//-> listen for incoming connections and making the socket a passive socket
    if (listen(socketfd, SOMAXCONN) == -1)
        throw(std::runtime_error("Failed to listen to socket."));

    new_client.fd = socketfd; //-> add the server socket to the pollfd
    new_client.events = POLLIN; //-> set the event to POLLIN for reading data
    new_client.revents = 0; //-> set the revents to 0
    fd_poll.push_back(new_client); //-> add the server socket to the pollfd
}

void Server::accept_client(void)
{
    socklen_t len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));

    int fd = accept(socketfd, (sockaddr *)&client_addr, &len);
    if (fd == -1)
        throw(std::runtime_error("Failed to accept client."));

    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("Failed to set option: (O_NONBLOCK)."));

    Client client;
    client.set_clientfd(fd);
    client.set_host(inet_ntoa(client_addr.sin_addr));

	clients[fd] = client;

    new_client.fd = fd;
    new_client.events = POLLIN;
    new_client.revents = 0;
    fd_poll.push_back(new_client);

    std::cout << "Client " << fd << " connected!" << std::endl;
}

void Server::recvData(int fd)
{
    char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);

	if (bytes <= 0)
    {
        disconnect_client(fd, "Connection lost");
        return;
	}

    Client *client = get_client(fd);
	if (!client) // Segurança extra: se o cliente não existir no mapa, pare aqui.
		return ;
    client->append_buffer(buff);
    
    while (true)
    {
        // Revalidar cliente a cada iteração
        client = get_client(fd);
        if (!client)
            return;

        std::string& buf = client->get_buffer();

        size_t pos = buf.find("\r\n");
        if (pos == std::string::npos)
            break;

        std::string cmd = buf.substr(0, pos);
        buf.erase(0, pos + 2);

        if (!cmd.empty())
            handle_cmd(cmd, fd);
    }
}

void Server::handle_cmd(std::string &cmd, int fd)
{
    Client *client = get_client(fd);
    std::vector<std::string> args = split_cmd(cmd);
    std::string nick = client->get_nick().empty() ? "*" : client->get_nick();

    std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::toupper);

    if (args[0] == "PASS")
        cmd_pass(fd, args);
    else if (args[0] == "NICK")
        cmd_nick(fd, args);
    else if (args[0] == "USER")
        cmd_user(fd, args);
    else if (args[0] == "QUIT")
        cmd_quit(fd, args);
	
	else if (!client->get_registered())
		send_rpl(ERR_NOTREGISTERED(nick), fd);
	
	else if (args[0] == "JOIN")
		cmd_join(fd, args);
	else if (args[0] == "PRIVMSG")
        cmd_privmsg(fd, args);
	else if (args[0] == "PART")
		cmd_part(fd, args);

	else if (args[0] == "KICK")
		cmd_kick(fd, args);
	else if (args[0] == "INVITE")
		cmd_invite(fd, args);
	else if (args[0] == "TOPIC")
		cmd_topic(fd, args);

	else if (args[0] == "MODE")
		cmd_mode(fd, args);

    else if (client->get_registered())
        send_rpl(ERR_UNKNOWNCOMMAND(nick, args[0]), fd);
    else
        send_rpl(ERR_NOTREGISTERED(nick), fd);
}

