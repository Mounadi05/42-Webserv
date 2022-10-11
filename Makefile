Name = webserv
src = 	./src/IOMultiplexing.cpp\
		./src/Request.cpp\
		./src/Response.cpp\
		./src/Socket.cpp\
		./src/Server.cpp\
		./src/Client.cpp\

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
	rm -rf $(Name)

re: fclean $(Name)
