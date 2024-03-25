/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/03/26 00:09:44 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(Config config):
    _config(config)
{
    std::stringstream ss;
    std::string name = config.GetServerName();
    
    this->_name = name.size() ? name : "server";
    this->_ports = config.GetPorts();
    this->_host = config.GetHost();
    this->_bodySize = config.GetClientMaxBodySize();
    this->_max_socket = 0;
    addSocketsServer();
    ss << "New server started => " << '[' << this->_name << ']';
    Utils::logger(ss.str(), INFO);
    
}

Server::~Server(void)
{
    for(size_t i = 0; i < this->_ports.size(); i++)
    {
        close(this->_ports[i]);
    }
    for(size_t i = 0; i < this->_clients.size(); i++)
    {
        delete(this->_clients[i]);
    }
}


void Server::addSocketsServer()
{   
    int on = 1;
    int sock;
    struct sockaddr_in sockaddr;
    for(size_t i = 0; i < this->_ports.size(); i++)
    {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            Utils::exceptWithError(ERROR_SOCKET_CREATE);

        if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
            Utils::exceptWithError("Error failed to set socket options");
            
        memset(&sockaddr, 0, sizeof(sockaddr));
        //**ECHAR UN OJO EXCEPTCION CON BIND => SEGMENTATION FAULT
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = this->_host;
        sockaddr.sin_port = htons(this->_ports[i]);
        
        if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
            Utils::exceptWithError(ERROR_SOCKET_BIND);
            
        if (listen(sock, MAX_CLIENTS) < 0)
            Utils::exceptWithError(ERROR_SOCKET_LISTEN);
            
        if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) 
            Utils::exceptWithError("Error setting socket to non-blocking");
        this->_sockets.push_back(sock);
        this->_sockaddrs.push_back(sockaddr);
    }
}

std::vector<uint16_t> Server::getSockets()
{
    return this->_sockets;
}

std::vector<sockaddr_in> Server::getSockaddrs()
{
    return this->_sockaddrs;
}

std::vector<Client *> Server::getClients()
{
    return this->_clients;
}


Config Server::getConfig()
{
    return this->_config;
}

void Server::removeClient(Client *client)
{
    std::vector<Client*>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), client);
    if(it != this->_clients.end())
    {
        close(client->getSocket());
        delete *it;
        this->_clients.erase(it);
    }
}

void Server::addClient(Client *client)
{
    this->_clients.push_back(client);
}



