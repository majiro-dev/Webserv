/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/02/06 20:26:15 by cmorales         ###   ########.fr       */
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
    _sock(), _connect_sock(), _socketaddr(), _addrlen(sizeof(_socketaddr)),
    _server_message(buildResponse())
{
    /* struct in_addr addr;
    addr.s_addr = config.GetHost();
    _ip_addr = inet_ntoa(addr);  auxiliar para que funcione */

    _socketaddr.sin_family = AF_INET;
    _socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _socketaddr.sin_port = htons(_port);

  /*   if (startServer() != 0)
    {
        Utils::exceptWithError(ERROR_SOCKET_CREATE);
    } */
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
        if (startServer() != 0)
            Utils::exceptWithError(ERROR_SOCKET_CREATE);
        loopListen();
    }
    catch(const MyError& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
    
}

int Server::startServer()
{
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0)
        Utils::exceptWithError(ERROR_SOCKET_CREATE);

    if (bind(_sock, (struct sockaddr *)&_socketaddr, sizeof(_socketaddr)) < 0)
        Utils::exceptWithError(ERROR_SOCKET_BIND);
    return (0);
}

void Server::loopListen()
{
    int bytesReceived = 0;
    
    if (listen(_sock, 20) < 0)
        Utils::exceptWithError(ERROR_SOCKET_LISTEN);

    Utils::log("Server is running");
    char ip_server[INET_ADDRSTRLEN];
    while (1)
    {
        Utils::log(WAITING_CONNECTION);
        acceptConnection(_connect_sock);
        inet_ntop(AF_INET,&_socketaddr.sin_addr, ip_server, INET_ADDRSTRLEN);
        Utils::log("Listening to address: " + (std::string)ip_server + " on port: " + Utils::IntToString(_port));

        char buffer[BUFFER_SIZE] = {0};
        
        bytesReceived = recv(_connect_sock, buffer, BUFFER_SIZE, 0);
        if (bytesReceived < 0)
            Utils::exceptWithError(ERROR_SOCKET_READ);
            
        Utils::log(REQUEST_RECEIVED);
        sendResponse();
        close(_connect_sock);
    }
}

void Server::acceptConnection(int &connect_sock)
{
    connect_sock = accept(_sock, (sockaddr *)&_socketaddr, &_addrlen);
    if (connect_sock < 0)
        Utils::exceptWithError(ERROR_SOCKET_ACCEPT);
}

std::string Server::buildResponse()
{
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> HELLO BURNING WORLD :D </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;

    return ss.str();
}

void Server::sendResponse()
{
    unsigned long bytesSenT;

    bytesSenT = write(_connect_sock, _server_message.c_str(), _server_message.size());
    if (bytesSenT == _server_message.size())
        Utils::log("Response sent successfully.");
    else
        Utils::exceptWithError("Failed to send response.");
}