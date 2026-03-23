#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../inc/Server.hpp"

class Client {
    private:
        int fd;
        std::string buffer;
        bool pwd_status;
        std::string nick;
        bool is_registered;
        std::string user;
        std::string host;
    public:
        Client();
        ~Client();

        // Setters
        void set_clientfd(int fd);
        void append_buffer(std::string buff);
        void set_pwd_status(bool val);
        void set_registered(bool val);
        void set_nick(std::string nickname);
        void set_host(std::string h);
        void set_user(std::string usr);

        // Getters
        int get_clientfd();
        std::string &get_buffer();
        bool get_pwd_status();
        std::string get_nick();
        bool get_registered();
        std::string get_host();
        std::string get_user();
};

#endif