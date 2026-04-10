#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
    private:
        int fd;
        bool is_registered;
        bool pwd_status;
        std::string buffer;
        std::string nick;
        std::string user;
        std::string host;
    public:
        Client();
        ~Client();

        // Setters
        void set_clientfd(int fd);
        void set_registered(bool val);
        void set_pwd_status(bool val);
        void append_buffer(std::string buff);
        void set_nick(std::string nickname);
        void set_user(std::string usr);
        void set_host(std::string h);

        // Getters
        int get_clientfd();
        bool get_registered();
        bool get_pwd_status();
        std::string &get_buffer();
        std::string get_nick();
        std::string get_user();
        std::string get_host();
        std::string get_prefix();
};

#endif