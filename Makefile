CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

NAME = webserv
SRC = Client.cpp\
	main.cpp
OBJ = $(SRC:.cpp=.o)


all : $(NAME)

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o:%.cpp Includes/webserv.hpp
	c++ $(CPPFLAGS) -c $< -o  $@

clean :
	rm -f $(OBJ) client.o

fclean : clean
	rm -f $(NAME) $(CLIENTNAME)

re : fclean all

.PHONY : all clean fclean re