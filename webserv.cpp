#include "includes/WebServer.hpp"

int main()
{
    IOMultiplexing io;
    Server server;

    server.ServerSetup(8080);
    server.ServerBind();
    server.ServerListen();

    io._maxFD = server._socket._fd + 1;
    FD_ZERO(&io._readFDs);
    FD_ZERO(&io._writeFDs);
    FD_SET(server._socket._fd,&io._readFDs);
    while (1)
    {
        io._cpy_readFDs = io._readFDs;
        io._cpy_writeFDs = io._writeFDs;
        if ((io._selectRet = select(io._maxFD,&io._cpy_writeFDs,&io._cpy_writeFDs,NULL,&io._timeOutVal)) != -1)
        {
            for (int i = 2; i < io._maxFD; i++)
            {
                if (FD_ISSET(i,&io._cpy_readFDs))
                {
                    //new connection
                    if (i == server._socket._fd)
                    {

                    } // request
                    else{

                    }
                }
                else if (FD_ISSET(i,&io._cpy_writeFDs))
                {
                    //response
                }
            }
        }
        else if (io._selectRet  == 0)
            continue;
        else
        {
            std::cout << "select failed" << std::endl;
            exit(1);
        }
    }
    
    return 0;
}