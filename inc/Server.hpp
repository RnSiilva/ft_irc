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
#include <algorithm>
#include <sstream>
#include "Client.hpp"
#include "Replies.hpp"

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
        void handle_cmd(std::string &cmd, int fd);

        // Getters
        Client *get_client(int fd);

        // CMDS
        void cmd_pass(int fd, std::vector<std::string> args);
        void cmd_nick(int fd, std::vector<std::string> args);
        void cmd_user(int fd, std::vector<std::string> args);

        // Utils
        static void sig_handler(int sig);
        void remove_client(int fd);
        void close_fd();
        bool nick_in_use(std::string &nick);
};

std::vector<std::string> split_cmd(std::string &cmd);
void send_rpl(std::string rpl, int fd);
bool invalid_nick(std::string &nick);

#endif
