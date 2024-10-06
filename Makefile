NAME = ircserv
SRC_FOLDER = src/
OBJ_FOLDER = obj/
SRC =	ft_irc.cpp \
		command/command.cpp \
		command/parser.cpp \
		channel/channel.cpp \
		client/client.cpp \
		server/server.cpp \
		server/commands.cpp 
SRC_O = $(SRC:.cpp=.o)
#FLAGS =  -g -std=c++98 -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(addprefix $(OBJ_FOLDER), $(SRC_O))
	c++ $(FLAGS) -o $@ $^
	@echo "\n" "100% - Compiled $(NAME) \n"

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.cpp
	@mkdir -p $(OBJ_FOLDER)/command
	@mkdir -p $(OBJ_FOLDER)/channel
	@mkdir -p $(OBJ_FOLDER)/client
	@mkdir -p $(OBJ_FOLDER)/server
	c++ $(FLAGS) -c $< -o $@

re: fclean all

clean:
	rm -rf $(OBJ_FOLDER)

fclean: clean
	rm -f $(NAME)

.PHONY: all clean fclean re