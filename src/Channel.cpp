#include "../inc/Channel.hpp"

Channel::Channel(std::string name) : _name(name), _topic(""), _password(""),
    _limit(0), mode_i(false), mode_t(false) {}

Channel::~Channel() {}

// ============ GETTERS ============
std::string Channel::getName() const	{ return _name; }
std::string Channel::getTopic() const	{ return _topic; }
//std::string Channel::get_password()        { return _password; }
//int         Channel::get_user_limit() { return user_limit; }
// bool        Channel::get_mode_i()		{ return mode_i; }
// bool        Channel::get_mode_t()		{ return mode_t; }
// bool        Channel::get_mode_k()		{ return mode_k; }
// bool        Channel::get_mode_l()		{ return mode_l; }

// --- Gestão de Membros ---
void Channel::addMember(Client *client)
{
    if (!isClientInChannel(client->get_clientfd()))
        _members.push_back(client);
}

void Channel::removeMember(int fd)
{
	// Remove da lista de clientes
    for (std::vector<Client *>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if ((*it)->get_clientfd() == fd)
        {
            _members.erase(it);
            break;
        }
    }

	// Garante que também seja removido da lista de operadores se estiver lá
    // If they leave, they also lose op and invite status
    removeOperator(fd);
    //remove_invited(client);
}

bool Channel::isClientInChannel(int fd)
{
	for (size_t i = 0; i < _members.size(); i++) {
        if (_members[i]->get_clientfd() == fd)
            return true;
    }
    return false;
}

// --- Gestão de Operadores ---
void Channel::addOperator(Client *client)
{
    if (!isOperator(client->get_clientfd()))
        _operators.push_back(client);
}

void Channel::removeOperator(int fd)
{
    for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if ((*it)->get_clientfd() == fd) {
            _operators.erase(it);
            break;
        }
    }
}

bool Channel::isOperator(int fd)
{
    for (size_t i = 0; i < _operators.size(); i++)
        if (_operators[i]->get_clientfd() == fd)
            return true;
    return false;
}

// --- O Coração do Canal: Broadcast ---
void Channel::broadcast(std::string msg, int excludeFd)
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i]->get_clientfd() != excludeFd)
			// Usando a função utilitária de envio
            send_rpl(msg, _members[i]->get_clientfd());
    }
}

const std::vector<Client *> &Channel::getMembers() const { return _members; }

// ============ UTILS ============
// Returns a space-separated list of nicks, with @ prefix for operators
std::string Channel::getMemberList()
{
    std::string list = "";
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (i > 0)
            list += " ";
        if (isOperator(_members[i]->get_clientfd()))
            list += "@";
        list += _members[i]->get_nick();
    }
    return list;
}

// // ============================================================

// // ============ SETTERS ============
// void Channel::set_topic(std::string t)  { topic = t; }
// void Channel::set_key(std::string k)    { key = k; }
// void Channel::set_user_limit(int limit) { user_limit = limit; }
// void Channel::set_mode_i(bool val)      { mode_i = val; }
// void Channel::set_mode_t(bool val)      { mode_t = val; }
// void Channel::set_mode_k(bool val)      { mode_k = val; }
// void Channel::set_mode_l(bool val)      { mode_l = val; }

// // ============ MEMBER MANAGEMENT ============
// void Channel::add_invited(Client *client)
// {
//     if (!is_invited(client))
//         invited.push_back(client);
// }

// void Channel::remove_invited(Client *client)
// {
//     for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
//     {
//         if (*it == client)
//         {
//             invited.erase(it);
//             break;
//         }
//     }
// }

// // ============ CHECKS ============
// bool Channel::is_member(Client *client) // isClientInChannel
// {
//     for (size_t i = 0; i < members.size(); i++)
//         if (members[i] == client)
//             return true;
//     return false;
// }

// bool Channel::is_invited(Client *client)
// {
//     for (size_t i = 0; i < invited.size(); i++)
//         if (invited[i] == client)
//             return true;
//     return false;
// }

// bool Channel::is_empty()
// {
//     return members.empty();
// }

