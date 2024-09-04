CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
NAME = webserv
CLIENTNAME = client
SRC = \
	main.cpp
OBJ = $(SRC:.cpp=.o)

all : $(NAME)

test : $(NAME) $(CLIENTNAME) clean

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) -o $(NAME) $(OBJ)

$(CLIENTNAME) : client.o
	c++ -o $(CLIENTNAME) client.o

%.o:%.cpp Includes/webserv.hpp
	c++ $(CPPFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJ) client.o

fclean : clean
	rm -f $(NAME) $(CLIENTNAME)

re : fclean all

.PHONY : all clean fclean re