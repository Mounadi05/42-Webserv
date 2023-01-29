
# webserver_42

A HTTP server implemented in C++ 98.
## Usage
```Bash
./webserv [configuration file]
```

## Features
- Takes a configuration file as argument, or use a default path.
- Non-blocking and uses  `select()` for all the I/O operations between the client and the server (listening included).
- `select()` checks read and write at the same time.
- No read or write operation without going through `select()`.
- Compatible with web browsers.
- Accurate HTTP response status codes.
- Default error pages if none provided.
- Serve a fully static website.
- Clients can upload files.
- Supports GET, POST, and DELETE methods.
- Stress-tested and stays available at all costs.
- Able to listen to multiple ports (configured in the configuration file).

## Configuration file
- Choose the port and host of each server.
- Setup server names or not.
- First server for a host:port is the default for that host:port.
- Setup default error pages.
- Limit client body size.
- Setup routes with rules/configuration:
  - List of accepted HTTP methods for the route.
  - HTTP redirection.
  - Turn directory listing on or off.
  - Default file to answer if request is a directory.
  - Execute CGI based on file extension (for example `.php`).
  - Route able to accept uploaded files and configure where to save them.
 ## Config file example
```
server {
	listen 		8000
	error_page 	404 www/pages_error/404.html 403 www/pages_error/403.html 405 www/pages_error/405.html
	upload_path 	www/Upload
	cgi /Users/ytaya/Desktop/42-Webserv/cgi_bin/php-cgi .php
	cgi /usr/bin/python3 .py
	location 	/
	{
		allow_methods POST GET
		root ./www/
		autoindex on

	}		

	location 	/gym
	{
		allow_methods Post GET
		root ./www/gym-website-template/
		index index.html
	}
	location 	/media
	{
		allow_methods GET 
		root ./www/multimedia/
		autoindex on
	}
	location 	/upload
	{
		allow_methods POST DELETE
		root ./www/
	}
	location 	/shop
	{
		allow_methods GET
		root ./www/shop-website/
		index index.html
	}
}

server {
	listen 		8080

	server_name 	google.com
	location 	/website1
	{
		allow_methods GET POST
		root ./www/shop-website/
		index index.html
	}
	location 	/website2
	{
		allow_methods GET POST
		root ./www/gym-website-template/
		index index.html
	}
}

server {
	listen 		5555
	error_page 	404 www/pages_error/404.html 403 www/pages_error/403.html 405 www/pages_error/405.html
	location 	/
	{
		allow_methods GET
		root ./www/gym-website-template/
		index index.html
	}

}
```

## CGI
- The server calls the CGI with the requested file as the first argument.
- The CGI is run in the correct directory for relative path file access.
- The server works with one CGI (e.g. `php-CGI`, `Python`, etc.).
- The server unchunks chunked requests and the CGI expects EOF as the end of the body.
- The output of the CGI is handled the same way. If the CGI doesn't return a content length, EOF marks the end of the returned data.

## Extra Features
- Support cookies and session management.
- Handle multiple CGI.

