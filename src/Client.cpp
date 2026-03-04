#include "../inc/Client.hpp"

Client::Client() {}
Client::~Client() {}

void Client::set_clientfd(int fd) { this->fd = fd; }

int Client::get_clientfd() { return fd; }


