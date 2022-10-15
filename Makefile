Name = webserv
src = 	./srcs/parsing/Config.cpp\
		./srcs/parsing/Server.cpp\
		./srcs/parsing/Location.cpp\
		./srcs/parsing/Socket.cpp\
		./srcs/networking/IOMultiplexing.cpp\

main = webserv.cpp
obj = $(src:.cpp=.o)

CPP = c++ -Wall -Wextra -Werror -std=c++98 -fsanitize=address  -g

$(Name): $(obj)
	$(CPP) $(main) -o $(Name) $(obj)

%.o: %.cpp
	$(CPP) -c $< -o $@

clean:
	rm -rf $(obj)

fclean: clean
	rm -rf $(Name)

re: fclean $(Name)
