#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream> //-> For terminal logs (std::cout, std::cerr)
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include <cstdlib> //-> Standard functions (exit, atoi, free)
#include <cstring> //-> Manipulating C(memset, strlen, bzero)
#include <algorithm> //-> For utilities (std::find, std::remove)
#include <sstream> //-> To format strings and convert data.

#include "Client.hpp"
#include "Replies.hpp"

class Client;

class Server {
    private:
		int port;
        int socketfd;
        static bool signal;
        std::string password;
        std::vector<Client> clients;
        std::vector<struct pollfd> fd_poll;
        struct sockaddr_in addr;
        struct pollfd new_client;
        struct sockaddr_in client_addr;
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
        void cmd_quit(int fd, std::vector<std::string> args);

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
