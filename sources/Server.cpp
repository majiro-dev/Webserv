/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/03/14 20:34:32 by cmorales         ###   ########.fr       */
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
    _sock(), _connect_sock(-1), _addrlen(sizeof(_socketaddr))
{
    /* struct in_addr addr;
    addr.s_addr = config.GetHost();
    _ip_addr = inet_ntoa(addr);  auxiliar para que funcione */
    
    bzero(&_socketaddr, _addrlen);
    _socketaddr.sin_family = AF_INET;
    _socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _socketaddr.sin_port = htons(8080);
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
        Utils::logger(e.what(), ERROR);
        //std::cerr << RED << e.what() << '\n';
    }
    
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

    Utils::logger("Start server", INFO);
    //Utils::log("Start server", YELLOW);
}

void Server::removeClient(Client *client)
{
    std::vector<Client*>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), client);
    if(it != this->_clients.end())
    {
        delete *it;
        this->_clients.erase(it);
    }
}

int Server::acceptConnection()
{
    //Ver los socketaddr para ponerlo en añadir conexion
    this->_connect_sock = accept(_sock, (sockaddr *)&_socketaddr, &_addrlen);
    if (this->_connect_sock < 0)
    {
        Utils::logger("Failed to accept connection", ERROR);
        return -1;
    }  

    if (fcntl(this->_connect_sock, F_SETFL, O_NONBLOCK) < 0)
    {
        Utils::logger("Error failed setting socket to non-blocking", ERROR);
        return -1;
    }
    
    Client* new_client = new Client(_connect_sock);
    this->_clients.push_back(new_client);

    struct pollfd client_poll;
    client_poll.fd = _connect_sock;
    client_poll.events = POLLIN;
    this->_pollfds.push_back(client_poll);
        
    return 0;
}


void Server::loopListen()
{
    struct pollfd pollsock;
    pollsock.fd = _sock;
    pollsock.events = POLLIN;
    this->_pollfds.push_back(pollsock);

    int timeout = 2 * 1000;
        
    while(1)
    {
        
        if((this->_ret = poll(this->_pollfds.data(), this->_pollfds.size(), timeout)) < 0)
            Utils::exceptWithError("Poll error");
        
        if(this->_ret)
        {
            for(size_t i = 0; i < this->_pollfds.size(); i++)
            {
                if (this->_pollfds[i].revents & POLLIN) 
                {
                    if (this->_pollfds[i].fd == _sock)
                    {
                        acceptConnection();
                        Utils::logger("Accept new connection", LOG);
                    }
                }
            }
        }
        for(size_t i = 0; i < this->_pollfds.size(); i++)
        {
            for(size_t j = 0; j < this->_clients.size(); j++)
            {
                if (this->_pollfds[i].revents & (POLLERR | POLLHUP) && this->_pollfds[i].fd == this->_clients[j]->getSocket()) 
                {
                    Utils::logger("Client close the connection", INFO);
                    close(this->_clients[j]->getSocket());
                    this->removeClient(this->_clients[j]);
                    this->_pollfds.erase(this->_pollfds.begin() + i);
                    break;
                }
                if (this->_pollfds[i].revents & POLLNVAL && this->_pollfds[i].fd == this->_clients[j]->getSocket()) 
                {
                    Utils::logger("Client socket is closed", INFO);
                    close(this->_clients[j]->getSocket());
                    this->removeClient(this->_clients[j]);
                    this->_pollfds.erase(this->_pollfds.begin() + i);
                    break;
                }
                if (this->_pollfds[i].revents & POLLIN && this->_pollfds[i].fd == this->_clients[j]->getSocket()) 
                {
                    this->_clients[j]->handleRecv();
                    if(this->_clients[j]->getfinishReq() == true)
                    {
                        this->_clients[j]->generateReponse();
                        this->_pollfds[i].events = POLLOUT;
                    }
                    break;
                }
                if (this->_pollfds[i].revents & POLLOUT && this->_pollfds[i].fd == this->_clients[j]->getSocket()) 
                {
                    this->_clients[j]->sendResponse();
                    this->_pollfds.erase(this->_pollfds.begin() + i);
                    this->removeClient(this->_clients[j]);
                    break; 
                }
            }
        } 
    }
}


