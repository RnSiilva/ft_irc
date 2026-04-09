CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = src/main.cpp src/Server.cpp src/Client.cpp src/Auth.cpp src/Utils.cpp src/Channel.cpp src/cmd/Quit.cpp
OBJS = $(SRCS:.cpp=.o)
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all