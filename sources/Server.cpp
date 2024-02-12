/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/02/13 00:37:40 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

/* Server::Server(std::string ip_address, int port) : _ip_address(ip_address), _port(port),
    _sock(), _connect_sock(), _socketaddr(), _addrlen(sizeof(_socketaddr)),
    _server_message(buildResponse())
{
    _socketaddr.sin_family = AF_INET;
    _socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _socketaddr.sin_port = htons(_port);

    if (startServer() != 0)
    {
        Utils::exceptWithError(ERROR_SOCKET_CREATE);
    }
} */

Server::Server(Config config) : _config(config), _port(config.GetPort()),
    _sock(), _connect_sock(), _addrlen(sizeof(_socketaddr)),
    _server_message(buildResponse())
{
    /* struct in_addr addr;
    addr.s_addr = config.GetHost();
    _ip_addr = inet_ntoa(addr);  auxiliar para que funcione */
    
    bzero(&_socketaddr, _addrlen);
    _socketaddr.sin_family = AF_INET;
    _socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _socketaddr.sin_port = htons(_port);
}

Server::~Server(void)
{
    closeServer();
}

void Server::closeServer()
{
    close(_sock);
    close(_connect_sock);
    exit(0);
}

void Server::runServer()
{
    try
    {
        startServer();
        loopListen();
    }
    catch(const MyError& e)
    {
        std::cerr << RED << e.what() << '\n';
    }
    
}

std::string Server::buildResponse()
{
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> HELLO BURNING WORLD :D </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;

    return ss.str();
}

void Server::startServer()
{   
    if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        Utils::exceptWithError(ERROR_SOCKET_CREATE);

    if (bind(_sock, (struct sockaddr *)&_socketaddr, sizeof(_socketaddr)) < 0)
        Utils::exceptWithError(ERROR_SOCKET_BIND);
        
    if (listen(_sock, 1000) < 0)
        Utils::exceptWithError(ERROR_SOCKET_LISTEN);

    std::cout << YELLOW << "Server is running" << std::endl;   
}

void Server::loopListen()
{
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_SET(_sock, &read_set); 

    int max_fds = _sock;
       
    if (fcntl(_sock, F_SETFL, O_NONBLOCK) < 0) 
        Utils::exceptWithError("Error setting socket to non-blocking");
        
    while (1)
    {
        Utils::log(WAITING_CONNECTION);

       if((_ret = select(max_fds + 1, &read_set, &write_set, NULL, 0)) < 0)
            Utils::exceptWithError("Select error");
        
        for (int i = 0; i <= max_fds; i++)
        {
            if((_ret > 0) && ((FD_ISSET(i, &read_set)) || (FD_ISSET(i, &write_set))))
            {
                if(i == _sock)
                {
                    acceptConnection(_connect_sock);
                    FD_SET(_connect_sock, &read_set);
                    max_fds = std::max(max_fds, _connect_sock);
                }
                else
                {
                    if(FD_ISSET(i, &read_set))
                    {
                        handleConnection(i);
                        FD_CLR(i, &read_set);
                        FD_SET(i, &write_set);
                    }
                        
                    if(FD_ISSET(i, &write_set))
                    {
                        sendResponse(i);
                        FD_CLR(i, &write_set);
                        close(i);
                    }
                    max_fds = std::max(max_fds, i);
                }
            }
        }
    }
}
void Server::acceptConnection(int &connect_sock)
{
    connect_sock = accept(_sock, (sockaddr *)&_socketaddr, &_addrlen);
    if (connect_sock < 0)
        Utils::exceptWithError(ERROR_SOCKET_ACCEPT);
        
    if (fcntl(connect_sock, F_SETFL, O_NONBLOCK) < 0) 
        Utils::exceptWithError("Error setting socket to non-blocking");
        
    std::cout << MAGENTA << "Nuevo cliente conectado, socket: " << connect_sock << std::endl << RESET;
}

void Server::handleConnection(int &client_fd)
{
    //char ip_server[INET_ADDRSTRLEN];
    //inet_ntop(AF_INET,&_socketaddr.sin_addr, ip_server, INET_ADDRSTRLEN);
    //Utils::log("Listening to address: " + (std::string)ip_server + " on port: " + Utils::IntToString(_port));
    int bytesReceived = 0;
    char buffer[BUFFER_SIZE] = {0};
        
    bytesReceived = recv(client_fd, buffer, BUFFER_SIZE, 0);
   /*  if (bytesReceived < 0)
    {
        //TOCAR ESTO LOS ERRORES QUE DEVUELVE
        FD_CLR(client_fd, &read_set);
        close(client_fd);
        Utils::exceptWithError(ERROR_SOCKET_READ);
    } */
    if (bytesReceived == 0) 
    {
        FD_CLR(client_fd, &read_set);
        close(client_fd);
        std::cout << "Conexión cerrada por el cliente, socket: " << client_fd << std::endl; 
        return ;
    }

    //Utils::log(REQUEST_RECEIVED);
    std::cout << MAGENTA << "Mensaje del cliente en el socket " << client_fd << ": " << buffer << std::endl << RESET;
}


void Server::sendResponse(int &client_fd)
{
    unsigned long bytesSenT;

    bytesSenT = write(client_fd, _server_message.c_str(), _server_message.size());
    if (bytesSenT == _server_message.size())
        Utils::log("Response sent successfully.");
    else
        Utils::exceptWithError("Failed to send response.");
}
