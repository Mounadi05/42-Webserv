Name = webserv
src = 	./srcs/parsing/Config.cpp\
		./srcs/parsing/Server.cpp\
		./srcs/parsing/Location.cpp\
		./srcs/parsing/Socket.cpp\
		./srcs/networking/listDir.cpp\
		./srcs/networking/IOMultiplexing.cpp\
		./srcs/networking/Clinet.cpp\
		./srcs/networking/Request.cpp\
		./srcs/networking/Response.cpp\

main = webserv.cpp
obj = $(src:.cpp=.o)

CPP = c++ -Wall -Wextra -Werror -std=c++98

$(Name): $(obj)
	$(CPP) $(main) -o $(Name) $(obj)

%.o: %.cpp
	$(CPP) -c $< -o $@

clean:
	rm -rf $(obj)

fclean: clean
	rm -rf $(Name) $(Name).dSYM 

re: fclean $(Name)
