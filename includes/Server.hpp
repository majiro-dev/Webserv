/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:07 by manujime          #+#    #+#             */
/*   Updated: 2024/03/27 01:17:10 by cmorales         ###   ########.fr       */
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
        in_addr_t                   _host;
        std::string                 _name;
        size_t                      _maxBodySize;
        std::vector<bool>           _allowMethods;
        Response                     _response;
        std::map<int, std::string>  _errorPages; 
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
        Config getConfig();
        Response getReponse();

        Response hadleRequest(Request &request);
        void generateReponse(const std::string& request);

        void checkErrorPage();
        void putErrorPage(Response &response);
};

#endif