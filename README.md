*This project has been created as part of the 42 curriculum by diolivei and resilva.*

## Description
**ft_irc** is a C++ project that implements a simplified Internet Relay Chat (IRC) server based on the IRC protocol. It allows multiple clients to connect, authenticate, and communicate in real time through channels and private messages. The server handles client connections, command parsing, and message distribution while respecting the core behavior of an IRC network.

## Instructions

### Compilation
Use the provided Makefile at the root of the repository:
```bash
make
```
This will generate the executable:
```bash
./ircserv
```

### Execution
Run the server with:
```bash
./ircserv <port> <password>
```
- `port`: Port number the server will listen on (must be between 1024 and 65535)
- `password`: Password required for client authentication

Once launched, the server will be ready to accept client connections.

### Usage
You can connect using an IRC client such as **HexChat**, or test it manually using `nc` (netcat) in another terminal.

To register, a client must provide:
- Server password
- Nickname
- Username

Most IRC clients allow you to fill these before connecting. If testing manually (e.g., with `nc`), you must send:
- `PASS <password>`
- `NICK <nickname>`
- `USER <username 0 * :real name>`

After registration, the following commands are supported:
- `JOIN <channel>` — join or create a channel
- `INVITE <nick> <channel>` — invite a user to a channel
- `KICK <channel> <nick>` — remove a user from a channel
- `PART <channel>` — leave a channel
- `PRIVMSG <target> <message>` — send a message to a user or channel
- `TOPIC <channel> [topic]` — view or change the channel topic
- `MODE <channel> <modes>` — set channel modes (`i` invite-only, `t` topic restriction, `k` password, `o` operator, `l` user limit)
- `QUIT [reason]` — disconnect from the server

## Resources

### References
- [What is an IRC Server](https://medium.com/@mohamedsarda/ft-irc-channels-and-command-management-ff1ff3758a0b)
- [Introduction to Network Programming](https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9)
- [IRC Protocol Documentation](https://modern.ircdocs.horse/)

### Use of AI
Artificial Intelligence tools were used as a support resource throughout the development of this project. They were mainly used for learning and better understanding key concepts, helping with project organization, and assisting in debugging and troubleshooting issues during implementation. All final decisions and code were developed and validated manually.