/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:07 by manujime          #+#    #+#             */
/*   Updated: 2024/04/16 17:00:01 by manujime         ###   ########.fr       */
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
# include "Methods.hpp"
# include "Utils.hpp"

class Server
{
    private:
        Config                          _config;
        std::vector<Client *>           _clients;
        std::vector<uint16_t>           _sockets;
        std::vector<sockaddr_in>        _sockaddrs;
        std::vector<uint16_t>           _ports;
        std::vector<Config>             _locations;
        in_addr_t                       _host;
        std::string                     _name;
        size_t                          _maxBodySize;
        Response                        _response;
        std::map<int, std::string>      _errorPages; 
        void addSocketsServer();
    public:
        Server(Config config);
        ~Server(void);
        
        void                        removeClient(Client *client);
        void                        addClient(Client *client);
        
        std::vector<uint16_t>       getSockets();
        std::vector<sockaddr_in>    getSockaddrs();
        std::vector<Client *>       getClients();
        Config                      getConfig();
        Config *                    getLocation(Request &request);
        Response                    getResponse();
        std::string                 getName();

        Response                    hadleRequest(Request &request, Config *location);
        void                        generateResponse(const std::string& request, sockaddr_in socketaddr);

        void                        addErrorPage(Config &config);
        void                        putErrorPage(Response &response);
};

#endif