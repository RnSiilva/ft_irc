#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

class Client;

class Channel {
    private:
        std::string name;
        std::string topic;
        std::string key;
        int user_limit;
        bool mode_i; // invite-only
        bool mode_t; // topic ops-only
        bool mode_k; // key required
        bool mode_l; // user limit
        std::vector<Client *> members;
        std::vector<Client *> operators;
        std::vector<Client *> invited;
    public:
        Channel(std::string name);
        ~Channel();
 
        // Getters
        std::string get_name();
        std::string get_topic();
        std::string get_key();
        int         get_user_limit();
        bool        get_mode_i();
        bool        get_mode_t();
        bool        get_mode_k();
        bool        get_mode_l();
        std::vector<Client *> &get_members();
 
        // Setters
        void set_topic(std::string topic);
        void set_key(std::string key);
        void set_user_limit(int limit);
        void set_mode_i(bool val);
        void set_mode_t(bool val);
        void set_mode_k(bool val);
        void set_mode_l(bool val);
 
        // Member management
        void add_member(Client *client);
        void remove_member(Client *client);
        void add_operator(Client *client);
        void remove_operator(Client *client);
        void add_invited(Client *client);
        void remove_invited(Client *client);
 
        // Checks
        bool is_member(Client *client);
        bool is_operator(Client *client);
        bool is_invited(Client *client);
        bool is_empty();
 
        // Broadcast
        void broadcast(std::string msg, int exclude_fd = -1);
 
        // Utils
        std::string get_member_list();
};

#endif