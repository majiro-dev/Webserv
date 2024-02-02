/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/02/02 12:32:20 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(std::string ip_address, int port) : _ip_address(ip_address), _port(port),
    _socket(), _new_socket(), _socket_address(), _address_length(sizeof(_socket_address)),
    _server_message(buildResponse())
{
    _socket_address.sin_family = AF_INET;
    _socket_address.sin_addr.s_addr = inet_addr(_ip_address.c_str());
    _socket_address.sin_port = htons(_port);

    if (startServer() != 0)
    {
        Utils::exitWithError(ERROR_SOCKET_CREATE);
    }
}

Server::Server(Config config) : _config(config), _port(config.GetPort()),
    _socket(), _new_socket(), _socket_address(), _address_length(sizeof(_socket_address)),
    _server_message(buildResponse())
{
    struct in_addr addr;
    addr.s_addr = config.GetHost();
    _ip_address = inet_ntoa(addr);

    _socket_address.sin_family = AF_INET;
    _socket_address.sin_addr.s_addr = inet_addr(_ip_address.c_str());
    _socket_address.sin_port = htons(_port);

    if (startServer() != 0)
    {
        Utils::exitWithError(ERROR_SOCKET_CREATE);
    }
}

Server::~Server(void)
{
    closeServer();
}

void Server::closeServer()
{
    close(_socket);
    close(_new_socket);
    exit(0);
}

int Server::startServer()
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        Utils::exitWithError(ERROR_SOCKET_CREATE);
    }
    if (bind(_socket, (struct sockaddr *)&_socket_address, sizeof(_socket_address)) < 0)
    {
        Utils::exitWithError(ERROR_SOCKET_BIND);
    }
    return (0);
}

void Server::startListen()
{
    int bytesReceived = 0;
    if (listen(_socket, 20) < 0)
    {
        Utils::exitWithError(ERROR_SOCKET_LISTEN);
    }
    Utils::log("Server is running");
    Utils::log("Listening to address: " + _ip_address + " on port: " + Utils::IntToString(_port));
    while (1)
    {
        Utils::log(WAITING_CONNECTION);
        acceptConnection(_new_socket);

        char buffer[BUFFER_SIZE] = {0};
        bytesReceived = recv(_new_socket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived < 0)
        {
            Utils::exitWithError(ERROR_SOCKET_READ);
        }
        Utils::log(REQUEST_RECEIVED);
        sendResponse();

        close(_new_socket);
    }
}

void Server::acceptConnection(int &new_socket)
{
    new_socket = accept(_socket, (sockaddr *)&_socket_address, &_address_length);
    if (new_socket < 0)
    {
        Utils::exitWithError(ERROR_SOCKET_ACCEPT);
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

void Server::sendResponse()
{
    unsigned long bytesSenT;

    bytesSenT = write(_new_socket, _server_message.c_str(), _server_message.size());
    if (bytesSenT == _server_message.size())
    {
        Utils::log("Response sent successfully.");
    }
    else
    {
        Utils::exitWithError("Failed to send response.");
    }
}