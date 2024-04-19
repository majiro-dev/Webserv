/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 17:29:22 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/19 13:56:40 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cluster.hpp"

Cluster::Cluster(std::list<Config> &configs)
{
    this->_configs.assign(configs.begin(), configs.end());
}

Cluster::Cluster(const Cluster &src)
{
    *this = src;
}

Cluster::~Cluster()
{
    for (size_t i = 0; i < this->_servers.size(); i++) 
    {
        delete _servers[i];
    }
}

bool Cluster::init()
{
    size_t servers = this->_configs.size();
    //ModIFICAR PARA EL MAIN SE TERMINE EL BUCLE
    try
    {
        for(size_t i = 0; i < servers; i++)
        {
            Server* server = new Server(this->_configs[i]);
            this->_servers.push_back(server);
        }
    }
    catch(const std::exception& e)
    {
        Utils::logger(e.what(), ERROR);
        return false;
    }
    return true;
}


int Cluster::acceptConnection(unsigned int sock, int idxServer, sockaddr_in servSockAdrr)
{
    int connect_sock;
    struct sockaddr_in      socketaddr;
    unsigned int            addrlen = sizeof(socketaddr);

    connect_sock = accept(sock, (sockaddr *)&socketaddr, &addrlen);
    if (connect_sock < 0)
    {
        Utils::logger("Failed to accept connection", ERROR);
        return -1;
    }  

    if (fcntl(connect_sock, F_SETFL, O_NONBLOCK) < 0)
    {
        Utils::logger("Error failed setting socket to non-blocking", ERROR);
        return -1;
    }
    this->_servers[idxServer]->addClient(new Client(connect_sock, socketaddr));
    
    char serverIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(socketaddr.sin_addr), serverIP, INET_ADDRSTRLEN);
    uint16_t serverPort = ntohs(servSockAdrr.sin_port);
    uint16_t clientPort = ntohs(socketaddr.sin_port);
    
    std::string msg;
    msg = this->_servers[idxServer]->getName() + ":" + Utils::IntToString(serverPort);
    msg += " has accepted new client " +  std::string(serverIP) + ":" +  Utils::IntToString(clientPort);
    Utils::logger(msg , INFO);
    return 0;
}

void Cluster::addSocketsPoll()
{
    this->_pollfds.clear();
    for(size_t i = 0; i < this->_servers.size(); i++)
    {
        std::vector<uint16_t>sockets = this->_servers[i]->getSockets();
        for(size_t j = 0; j < sockets.size() && j < 1024; j++)
        {
            struct pollfd pollsock;
            pollsock.fd = sockets[j];
            pollsock.events = POLLIN;
            this->_pollfds.push_back(pollsock);
        }
        std::vector<Client *> clients;
        clients = this->_servers[i]->getClients();
        for(size_t j = 0; j < clients.size(); j++)
        {
            struct pollfd pollsock;
            pollsock.fd = clients[j]->getSocket();
            if(clients[j]->getfinishReq())
                pollsock.events = POLLOUT;
            else
                pollsock.events = POLLIN;
            this->_pollfds.push_back(pollsock);
        }
    }
}



void Cluster::checkServerSockets()
{
    for(size_t i = 0; i < this->_servers.size(); i++)
    {
        std::vector<uint16_t> sockets = this->_servers[i]->getSockets();
        for(size_t j = 0; j < sockets.size() && j < 1024; j++)
        {
            for(size_t k = 0; k < this->_pollfds.size(); k++)
            {
                if(this->_pollfds[k].revents & POLLIN && this->_pollfds[k].fd == sockets[j])
                {
                    acceptConnection(sockets[j], i, _servers[i]->getSockaddrs()[j]);
                    break;
                }
            }
        }
    }
}

#include <iostream>

void Cluster::checkClientSockets()
{
    std::cout << "Starting checkClientSockets" << std::endl;
    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        std::cout << "Checking server " << i << std::endl;
        std::vector<Client *> clients = this->_servers[i]->getClients();
        for (size_t j = 0; j < clients.size(); j++)
        {
            std::cout << "Checking client " << j << " on server " << i << std::endl;
            for (size_t k = 0; k < this->_pollfds.size(); k++)
            {
                this->_pollfds[k].fd = clients[j]->getSocket();
                std::cout << "Checking pollfd " << k << " for client " << j << " on server " << i << std::endl;
                std::cout << "pollfd k fd: " << this->_pollfds[k].fd << " client socket: " << clients[j]->getSocket() << std::endl;
                if (this->_pollfds[k].revents & (POLLERR | POLLHUP) && this->_pollfds[k].fd == clients[j]->getSocket())
                {
                    std::cout << "Client close the connection" << std::endl;
                    close(clients[j]->getSocket());
                    this->_servers[i]->removeClient(clients[j]);
                    this->_pollfds.erase(this->_pollfds.begin() + k);
                    std::cout << "Client " << j << " on server " << i << " closed the connection" << std::endl;
                    break; 
                }
                if (this->_pollfds[k].revents & (POLLNVAL) && this->_pollfds[k].fd == clients[j]->getSocket())
                {
                    std::cout << "Client socket is not open" << std::endl;
                    this->_servers[i]->removeClient(clients[j]);
                    this->_pollfds.erase(this->_pollfds.begin() + k);
                    std::cout << "Client " << j << " on server " << i << " socket is not open" << std::endl;
                    break; 
                }
                if (this->_pollfds[k].revents & POLLIN && this->_pollfds[k].fd == clients[j]->getSocket())
                {
                    std::cout << "Client " << j << " on server " << i << " has data to read" << std::endl;
                    int val_recv;
                    val_recv = clients[j]->handleRecv();
                    if(val_recv == 0)
                    {
                        this->_servers[i]->generateResponse(clients[j]->getRequest(), clients[j]->getSocketaddr());
                        std::cout << "Generated response for client " << j << " on server " << i << std::endl;
                    }
                    if(val_recv < 0)
                    {
                        std::cout << "Error reading from client " << j << " on server " << i << std::endl;
                        this->_servers[i]->removeClient(clients[j]);
                        this->_pollfds.erase(this->_pollfds.begin() + k);
                        std::cout << "Error reading from client " << j << " on server " << i << std::endl;
                    }
                    break;
                }
                if (this->_pollfds[k].revents & POLLOUT && this->_pollfds[k].fd == clients[j]->getSocket())
                {
                    std::cout << "Client " << j << " on server " << i << " is ready to receive data" << std::endl;
                    int val_send;
                    val_send = clients[j]->sendResponse(this->_servers[i]->getResponse());
                    if(val_send == 0)
                    {
                        //usleep(2000);
                        this->_servers[i]->removeClient(clients[j]);
                        std::cout << "Removed client " << j << " on server " << i << std::endl;
                    }
                    if(val_send == -1)
                    {
                        this->_servers[i]->removeClient(clients[j]);
                        std::cout << "Failed to send response to client " << j << " on server " << i << std::endl;
                    }
                    this->_pollfds.erase(this->_pollfds.begin() + k);
                    std::cout << "Erased pollfd " << k << " for client " << j << " on server " << i << std::endl;
                    break;
                }
            }
        }
    }
    std::cout << "Finished checkClientSockets" << std::endl;
}

void Cluster::run()
{
    int timeout = 2000;
    while(1)
    {
        addSocketsPoll();
        int ret = poll(this->_pollfds.data(), this->_pollfds.size(), timeout);
        if(ret)
        {
            checkServerSockets();
            checkClientSockets();
        }
    }
}
