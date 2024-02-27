/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/02/26 23:19:56 by cmorales         ###   ########.fr       */
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
    _sock(), _connect_sock(-1), _addrlen(sizeof(_socketaddr)),
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
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Ruben se va a follar a Villa y se la pela :):D </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;

    return ss.str();
}

void Server::startServer()
{   
    int on = 1;
    
    if ((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        Utils::exceptWithError(ERROR_SOCKET_CREATE);

    if(setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
        Utils::exceptWithError("Error failed to set socket options");

    if (bind(this->_sock, (struct sockaddr *)&_socketaddr, sizeof(_socketaddr)) < 0)
        Utils::exceptWithError(ERROR_SOCKET_BIND);
        
    if (listen(this->_sock, MAX_CLIENTS) < 0)
        Utils::exceptWithError(ERROR_SOCKET_LISTEN);
        
    if (fcntl(this->_sock, F_SETFL, O_NONBLOCK) < 0) 
        Utils::exceptWithError("Error setting socket to non-blocking");

    Utils::log("Start server", YELLOW);
}


void Server::loopListen()
{
    std::vector<pollfd>pollfds;

    struct pollfd pollsock;
    pollsock.fd = _sock;
    pollsock.events = POLLIN;
    pollfds.push_back(pollsock);

    while(1)
    {
        Utils::log(WAITING_CONNECTION, RESET);
        
        if((this->_ret = poll(pollfds.data(), pollfds.size(), -1)) < 0)
            Utils::exceptWithError("Poll error");
        
        if(this->_ret)
        {
            for(size_t i = 0; i < pollfds.size(); i++)
            {
                if(pollfds[i].revents & POLLIN)
                {
                    if(pollfds[i].fd == _sock)
                    {
                        this->_connect_sock = acceptConnection();
                        if (_connect_sock >= 0)
                        {
                            struct pollfd new_client;
                            new_client.fd = _connect_sock;
                            new_client.events = POLLIN;
                            pollfds.push_back(new_client);
                        }
                    }
                    else
                    {
                        //Tomar las request y convertir en respuesta activando escritura
                        handleConnection(pollfds[i].fd);
                        pollfds[i].events = POLLOUT;
                    }
                }
            }
            
        }
        for(size_t i = 0; i < pollfds.size(); i++)
        {
            if(pollfds[i].revents & POLLOUT)
            {
                sendResponse(pollfds[i].fd);
                pollfds.erase(pollfds.begin() + i);
                i--;
            }
        }
    }
}


int Server::acceptConnection()
{
    int connect_sock = accept(_sock, (sockaddr *)&_socketaddr, &_addrlen);
    if (connect_sock < 0)
    {
        //No esta listo (esta modo-noblcoking) puede seguir otras tareas
     /*    if (errno == EAGAIN || errno == EWOULDBLOCK)
            return -1; */
       Utils::exceptWithError(ERROR_SOCKET_ACCEPT);
    }  

    if (fcntl(connect_sock, F_SETFL, O_NONBLOCK) < 0) 
        Utils::exceptWithError("Error setting socket to non-blocking");
        
    return connect_sock;
}


//Meter cliente
void Server::handleConnection(int &client_fd)
{
    int bytesReceived = 0;
    char buffer[BUFFER_SIZE] = {0};
        
    bytesReceived = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0)
    {
        //**PREGUNTAR** No esta listo (esta modo-noblcoking) puede seguir otras tareas
        /* if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;   */          
        //Utils::exceptWithError(ERROR_SOCKET_READ);
        close(client_fd);
        Utils::log(ERROR_SOCKET_READ, RED);
        return ;
    } 
    if (bytesReceived == 0) 
    {
        close(client_fd);
        std::cout << "Conexión cerrada por el cliente, socket: " << client_fd << std::endl; 
        return ;
    }
    std::cout << MAGENTA << "Mensaje del cliente en el socket "  << client_fd << ": " << buffer << std::endl << RESET;
    //CLientev variable request => buffer para parselo a request y luego al response
    Request req(buffer);
}


void Server::sendResponse(int &client_fd)
{
    unsigned long bytesSenT;

    bytesSenT = write(client_fd, _server_message.c_str(), _server_message.size());
    if (bytesSenT == _server_message.size())
        Utils::log("Response sent successfully.", GREEN);
    else
    {
        //Utils::exceptWithError("Failed to send response.");
        Utils::log("Failed to send response", RED);
        return ;
    }
    close(client_fd);
}
