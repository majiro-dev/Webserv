/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:07 by manujime          #+#    #+#             */
/*   Updated: 2024/03/25 01:13:54 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "Socket.hpp"
# include "outputMacros.hpp"
# include "Request.hpp"
# include "Client.hpp"
# include "Response.hpp"
# include "Utils.hpp"

class Server
{
    private:
        Config                      _config;
        std::vector<Client *>       _clients;
        std::vector<uint16_t>       _sockets;
        std::vector<sockaddr_in>    _sockaddrs;
        std::vector<uint16_t>       _ports;
        //char                        *_host;
        int                         _max_socket;
        
        void addSocketsServer();
    public:
        //Server(std::string ip_address, int port);
        Server(Config config);
        ~Server(void);
        
        void removeClient(Client *client);
        void addClient(Client *client);
        
        std::vector<uint16_t> getSockets();
        std::vector<sockaddr_in> getSockaddrs();
        std::vector<Client *> getClients();
  
};

#endif