#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"

class Channel {
    private:
        std::string		_name;
        std::string		_topic;
        std::string		_key;
        size_t			_limit;

		// Flags de Modo (Essenciais para a Fase 4)
        bool _modeInvite; // invite-only +i
        bool _modeTopic; // topic ops-only +t
        bool _modeKey; // key required
        bool _modeLimit; // user limit
	
		// As listas de quem está lá dentro
        std::vector<Client *> _members;
        std::vector<Client *> _operators;
        std::vector<int> _invitedFds;


    public:
        Channel(std::string name);
        ~Channel();
 
        // --- Getters de Estado ---
		bool			hasMode(char mode) const;
        std::string		getTopic() const;
    	std::string		getKey() const;
		size_t			getLimit() const;
		std::string		getModes() const;

        std::string		getName() const;

		// --- Setters de Modo (Os "Interruptores") ---
		void setModeInvite(bool val);
        void setModeTopic(bool val);
		void setModeKey(bool val);
        void setModeLimit(bool val);

		// --- Setters de Dados ---
		void setTopic(std::string t);
        void setKey(std::string k);
		void setLimit(size_t l);

		// Gestão de Usuários
		void addMember(Client *client);
        void removeMember(int fd);
		bool isClientInChannel(int fd);

		// Gestão de Operadores
        void addOperator(Client *client);
        void removeOperator(int fd);
        bool isOperator(int fd);

		// Getters para os vetores (úteis para comandos como NAMES)
        const std::vector<Client *> &getMembers() const;

        // // Member management
        void addInvite(int fd);
        void removeInvite(int fd);
        bool isInvited(int fd);
		
		// O "Coração" do Canal: Enviar mensagem para todos
		void broadcast(std::string msg, int excludeFd = -1);
		
		// Utils
        std::string getMemberList();
		int checkCanJoin(Client *client, std::string provided_key);
};

#endif