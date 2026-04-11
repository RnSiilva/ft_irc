NAME = ircserv

# ------------------------------ Colors ------------------------------
BOLD_PURPLE     =       \033[1;35m
BOLD_CYAN       =       \033[1;36m
BOLD_GREEN      =       \033[1;32m
BOLD_YELLOW     =       \033[1;33m
NO_COLOR        =       \033[0m

# ------------------------------ Message ------------------------------
COMP_START      =       echo "\n $(BOLD_YELLOW)Make: $(NO_COLOR)Starting the compilation...\n"
READY           =       echo "\n $(BOLD_GREEN)Ready: $(NO_COLOR)The program$(BOLD_CYAN) $(NAME) $(NO_COLOR)has been compiled!\n"
CLEANED         =       echo "\n $(BOLD_PURPLE)clean: $(NO_COLOR)Removed all the \".o\" files \n"
FCLEANED        =       echo "\n $(BOLD_PURPLE)fclean: $(NO_COLOR)Removed the executables \n"

# ------------------------------ Variables ------------------------------
CXX = c++
RM = rm -f
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
DN = /dev/null 2>&1
SRC = src/main.cpp src/Server.cpp src/Client.cpp src/Auth.cpp src/Utils.cpp src/Channel.cpp src/cmd/Quit.cpp src/cmd/Join.cpp src/cmd/Privmsg.cpp src/cmd/Part.cpp src/cmd/Kick.cpp src/cmd/Invite.cpp src/cmd/Topic.cpp

OBJ = ${SRC:.cpp=.o}

.cpp.o:
	@${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

${NAME}: ${OBJ}
	@$(COMP_START)
	@${CXX} ${OBJ} ${CXXFLAGS} -o ${NAME}
	@$(READY) 

all: ${NAME}

clean:
	@${RM} ${OBJ}
	@$(CLEANED)

fclean: clean
	@${RM} ${NAME}
	@$(FCLEANED)

re: fclean all

.PHONY: all clean fclean re
