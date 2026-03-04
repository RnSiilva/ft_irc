#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstdlib>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <cstring>
#include "Client.hpp"

class Client;

class Server {
    private:
        int socketfd;
        int port;
        static bool signal;
        std::string password;
        struct sockaddr_in addr;
        struct pollfd new_client;
        std::vector<struct pollfd> fd_poll;
        struct sockaddr_in client_addr;
        std::vector<Client> clients;
    public:
        Server();
        ~Server();

        // Configs
        void server_start(int port, std::string pwd);
        void create_socketfd();
        void accept_client();
        void recvData(int fd);

        // Utils
        static void sig_handler(int sig);
        void remove_client(int fd);
        void close_fd();
};

#endif
