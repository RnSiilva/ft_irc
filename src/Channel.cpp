#include "../inc/Channel.hpp"

Channel::Channel(std::string name) :
	_name(name), _topic(""), _key(""), _limit(0),
	_modeInvite(false), _modeTopic(false), _modeKey(false),
	_modeLimit(false) {}

Channel::~Channel() {}

// ============ GETTERS ============
std::string Channel::getName() const	{ return _name; }
std::string Channel::getTopic() const	{ return _topic; }
std::string Channel::getKey() const		{ return _key; }
size_t		Channel::getLimit() const	{ return _limit; }

bool		Channel::hasMode(char mode) const {
	if (mode == 'i') return _modeInvite;
	if (mode == 't') return _modeTopic;
	if (mode == 'k') return _modeKey;
	if (mode == 'l') return _modeLimit;

	return false;
}

std::string Channel::getModes() const {
	std::string modes = "";
	if (_modeInvite)	modes += "i";
	if (_modeTopic)		modes += "t";
	if (_modeKey)		modes += "k";
	if (_modeLimit)		modes += "l";

	return modes;
}

// ============ SETTERS ============
void Channel::setTopic(std::string t)	{ _topic = t; }
void Channel::setKey(std::string k)		{ _key = k; }
void Channel::setLimit(size_t l)			{ _limit = l; }

void Channel::setModeInvite(bool val)		{ _modeInvite = val; }
void Channel::setModeTopic(bool val)		{ _modeTopic = val; }
void Channel::setModeKey(bool val)			{ _modeKey = val; }
void Channel::setModeLimit(bool val)      { _modeLimit = val; }

// // ============ MEMBER MANAGEMENT ============
void Channel::addInvite(int fd)
{
    if (!isInvited(fd))
        _invitedFds.push_back(fd);
}

bool Channel::isInvited(int fd)
{
    for (size_t i = 0; i < _invitedFds.size(); i++)
        if (_invitedFds[i] == fd)
            return true;
    return false;
}

void Channel::removeInvite(int fd)
{
    for (std::vector<int>::iterator it = _invitedFds.begin(); it != _invitedFds.end(); ++it)
    {
        if (*it == fd)
        {
            _invitedFds.erase(it);
            break;
        }
    }
}
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

	// Nova Regra: Se a lista de operadores ficou vazia MAS ainda há membros...
	if (_operators.empty() && !_members.empty()) {
		// Promove o próximo membro disponível a operador
		addOperator(_members[0]);

		// Notifica o canal sobre o novo operador (Opcional)
		std::string msg = ":" + _members[0]->get_nick() + " MODE " + _name + " +o " + _members[0]->get_nick() + "\r\n";
		broadcast(msg);
	}
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

int Channel::checkCanJoin(Client *client, std::string provided_key)
{
	if (this->hasMode('i') && !this->isInvited(client->get_clientfd()))
		return (473);
	
	if (this->hasMode('k') && provided_key != this->_key)
        return (475);

    if (this->hasMode('l') && _members.size() >= _limit)
        return (471);

    return 0;
}
