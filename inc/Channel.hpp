#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"

class Channel {
    private:
        std::string		_name;
        std::string		_topic;
        std::string		_password;
        unsigned int	_limit;

		// Flags de Modo (Essenciais para a Fase 4)
        bool mode_i; // invite-only
        bool mode_t; // topic ops-only
        // bool mode_k; // key required
        // bool mode_l; // user limit
	
		// As listas de quem está lá dentro
        std::vector<Client *> _members;
        std::vector<Client *> _operators;
        std::vector<int> _invitedFds;


    public:
        Channel(std::string name);
        ~Channel();
 
        // Getters Básicos
        std::string getName() const;
        std::string getTopic() const;
		void setTopic(std::string topic);
        //std::string get_password();

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

		// O "Coração" do Canal: Enviar mensagem para todos
		void broadcast(std::string msg, int excludeFd = -1);
		
		// Utils
        std::string getMemberList();

        // // Member management
        void addInvite(int fd);
        void removeInvite(int fd);
        bool isInvited(int fd);

		// // Getters
        // int         get_user_limit();
        // bool        get_mode_i();
        // bool        get_mode_t();
        // bool        get_mode_k();
        // bool        get_mode_l();
 
        // // Setters
        // void set_topic(std::string topic);
        // void set_key(std::string key);
        // void set_user_limit(int limit);
        // void set_mode_i(bool val);
        // void set_mode_t(bool val);
        // void set_mode_k(bool val);
        // void set_mode_l(bool val);
 
        // // Checks
        // bool is_member(Client *client);
        // bool is_empty();

};

#endif