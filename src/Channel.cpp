#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

Channel::Channel(std::string name) : name(name), topic(""), key(""),
    user_limit(0), mode_i(false), mode_t(false), mode_k(false), mode_l(false) {}
Channel::~Channel() {}

// ============ GETTERS ============
std::string Channel::get_name()       { return name; }
std::string Channel::get_topic()      { return topic; }
std::string Channel::get_key()        { return key; }
int         Channel::get_user_limit() { return user_limit; }
bool        Channel::get_mode_i()     { return mode_i; }
bool        Channel::get_mode_t()     { return mode_t; }
bool        Channel::get_mode_k()     { return mode_k; }
bool        Channel::get_mode_l()     { return mode_l; }

std::vector<Client *> &Channel::get_members() { return members; }

// ============ SETTERS ============
void Channel::set_topic(std::string t)  { topic = t; }
void Channel::set_key(std::string k)    { key = k; }
void Channel::set_user_limit(int limit) { user_limit = limit; }
void Channel::set_mode_i(bool val)      { mode_i = val; }
void Channel::set_mode_t(bool val)      { mode_t = val; }
void Channel::set_mode_k(bool val)      { mode_k = val; }
void Channel::set_mode_l(bool val)      { mode_l = val; }

// ============ MEMBER MANAGEMENT ============
void Channel::add_member(Client *client)
{
    if (!is_member(client))
        members.push_back(client);
}

void Channel::remove_member(Client *client)
{
    for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (*it == client)
        {
            members.erase(it);
            break;
        }
    }
    // If they leave, they also lose op and invite status
    remove_operator(client);
    remove_invited(client);
}

void Channel::add_operator(Client *client)
{
    if (!is_operator(client))
        operators.push_back(client);
}

void Channel::remove_operator(Client *client)
{
    for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); ++it)
    {
        if (*it == client)
        {
            operators.erase(it);
            break;
        }
    }
}

void Channel::add_invited(Client *client)
{
    if (!is_invited(client))
        invited.push_back(client);
}

void Channel::remove_invited(Client *client)
{
    for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
    {
        if (*it == client)
        {
            invited.erase(it);
            break;
        }
    }
}

// ============ CHECKS ============
bool Channel::is_member(Client *client)
{
    for (size_t i = 0; i < members.size(); i++)
        if (members[i] == client)
            return true;
    return false;
}

bool Channel::is_operator(Client *client)
{
    for (size_t i = 0; i < operators.size(); i++)
        if (operators[i] == client)
            return true;
    return false;
}

bool Channel::is_invited(Client *client)
{
    for (size_t i = 0; i < invited.size(); i++)
        if (invited[i] == client)
            return true;
    return false;
}

bool Channel::is_empty()
{
    return members.empty();
}

// ============ BROADCAST ============
void Channel::broadcast(std::string msg, int exclude_fd)
{
    for (size_t i = 0; i < members.size(); i++)
    {
        if (members[i]->get_clientfd() != exclude_fd)
            send_rpl(msg, members[i]->get_clientfd());
    }
}

// ============ UTILS ============
// Returns a space-separated list of nicks, with @ prefix for operators
std::string Channel::get_member_list()
{
    std::string list;
    for (size_t i = 0; i < members.size(); i++)
    {
        if (i > 0)
            list += " ";
        if (is_operator(members[i]))
            list += "@";
        list += members[i]->get_nick();
    }
    return list;
}