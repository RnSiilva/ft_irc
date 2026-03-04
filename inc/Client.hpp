#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../inc/Server.hpp"

class Client {
    private:
        int fd;
    public:
        Client();
        ~Client();

        void set_clientfd(int fd);
        int get_clientfd();
};

#endif