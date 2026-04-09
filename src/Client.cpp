#include "../inc/Client.hpp"

Client::Client()
{
    fd = -1;
    pwd_status = false;
    is_registered = false;
    nick = "";
    user = "";
    host = "";
    buffer = "";
}
Client::~Client() {}

void Client::set_clientfd(int fd) { this->fd = fd; }
int Client::get_clientfd() { return fd; }

void Client::append_buffer(std::string buff) { buffer += buff; }
std::string &Client::get_buffer() { return buffer; }

void Client::set_pwd_status(bool val) { pwd_status = val; }
bool Client::get_pwd_status() { return pwd_status; }

void Client::set_nick(std::string nickname) { nick = nickname; }
std::string Client::get_nick() { return nick; }

void Client::set_registered(bool val) { is_registered = val; }
bool Client::get_registered() { return is_registered; }

void Client::set_host(std::string h) { host = h; }
std::string Client::get_host() { return host; }

void Client::set_user(std::string usr) { user = usr; }
std::string Client::get_user() { return user; }

std::string Client::get_prefix() { return nick + "!" + user + "@" + host; }
