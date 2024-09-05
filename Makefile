CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

NAME = webserv
SRC = \
	main.cpp
OBJ = $(SRC:.cpp=.o)

CLIENTNAME = client

all : $(NAME)

test : $(NAME) $(CLIENTNAME)

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) -o $(NAME) $(OBJ)

$(CLIENTNAME) : client.o
	c++ -o $(CLIENTNAME) client.o

%.o:%.cpp Includes/webserv.hpp
	c++ $(CPPFLAGS) -c $< -o  $@

clean :
	rm -f $(OBJ) client.o

fclean : clean
	rm -f $(NAME) $(CLIENTNAME)

re : fclean all

.PHONY : all clean fclean re