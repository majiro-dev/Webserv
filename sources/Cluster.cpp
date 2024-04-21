/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 17:29:22 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/21 17:52:08 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cluster.hpp"

Cluster::Cluster(std::list<Config> &configs)
{
    this->_configs.assign(configs.begin(), configs.end());
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
int Cluster::addSocketsPoll(struct pollfd *fds)
{
    int nfds = 0;
    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        // Add server sockets
        std::vector<uint16_t> sockets = this->_servers[i]->getSockets();
        for (size_t j = 0; j < sockets.size() && nfds < 1024; j++)
        {
            fds[nfds].fd = sockets[j];
            fds[nfds].events = POLLIN;
            nfds++;
        }
        // Add client sockets
        std::vector<Client *> clients = this->_servers[i]->getClients();
        for (size_t j = 0; j < clients.size(); j++)
        {
            int clientSocket = clients[j]->getSocket();
            fds[nfds].fd = clientSocket;
            if (clients[j]->getfinishReq())
                fds[nfds].events = POLLOUT;
            else
                fds[nfds].events = POLLIN;
            nfds++;
        }
    }
    return nfds;
}
void Cluster::checkServerSockets(struct pollfd *fds)
{
    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        std::vector<uint16_t> sockets = this->_servers[i]->getSockets();
        for (size_t j = 0; j < sockets.size(); j++)
        {
            int serverSocket = sockets[j];
            for (int k = 0; k < 1024; k++)
            {
                if (fds[k].revents & POLLIN && fds[k].fd == serverSocket)
                {
                    acceptConnection(serverSocket, i, _servers[i]->getSockaddrs()[j]);
                    break;
                }
            }
        }
    }
}
// Ejemplo: Utilizar un índice directo en checkClientSockets
void Cluster::checkClientSockets(struct pollfd *fds) {
    for (size_t i = 0; i < this->_servers.size(); i++) {
        std::vector<Client *> clients = this->_servers[i]->getClients();
        for (size_t j = 0; j < clients.size(); j++) {
            Client *client = clients[j];
            int clientSocket = client->getSocket();
            
            for (int k = 0; k < 1024; k++)
            {
                if (fds[k].fd == clientSocket)
                {
                    int revents = fds[k].revents;
                    
                    if (revents & (POLLERR | POLLHUP))
                    {
                        Utils::logger("Client closed the connection", INFO);
                        close(clientSocket);
                        this->_servers[i]->removeClient(client);
                        break;
                    }
                    else if (revents & POLLNVAL)
                    {
                        Utils::logger("Client socket is not open", ERROR);
                        this->_servers[i]->removeClient(client);
                        break;
                    }
                    else if (revents & POLLIN)
                    {
                        int val_recv = client->handleRecv();
                        if (val_recv == 0) {
                            this->_servers[i]->generateResponse(client->getRequest(), client->getSocketaddr());
                        } else if (val_recv < 0) {
                            Utils::logger(ERROR_SOCKET_READ, ERROR);
                            this->_servers[i]->removeClient(client);
                        }
                        break;
                    }
                    else if (revents & POLLOUT)
                    {
                        int val_send = client->sendResponse(this->_servers[i]->getResponse());
                        if (val_send == 0) {
                            usleep(2000);
                            this->_servers[i]->removeClient(client);
                        } else if (val_send == -1) {
                            this->_servers[i]->removeClient(client);
                        }
                        break;
                    }
                }
            }
        }
    }
}

void Cluster::run()
{
    int timeout = 2000;
    while (1)
    {
        struct pollfd fds[1024 + 1];
        std::memset(fds, 0, sizeof(fds));
        int sizefd = addSocketsPoll(fds);
        int ret = poll(fds, sizefd, timeout);
        if (ret)
        {
            checkServerSockets(fds);
            checkClientSockets(fds);
        }
    }
}