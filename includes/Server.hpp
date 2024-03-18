/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:07 by manujime          #+#    #+#             */
/*   Updated: 2024/03/14 20:18:45 by cmorales         ###   ########.fr       */
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
# include <fcntl.h>
# include <poll.h>


class Server
{
    public:
        //Server(std::string ip_address, int port);
        Server(Config config);
        ~Server(void);
        void startListen();
        void runServer();

    private:
        Config  _config;
        std::vector<Client *>_clients;
        std::vector<pollfd>_pollfds;
        std::string _ip_addr;
        int         _ret;
        int         _port;
        int         _sock;
        int         _connect_sock;
        int         _max_socket;
        struct sockaddr_in      _socketaddr;
        unsigned int            _addrlen;

        void startServer();
        void loopListen();
        void closeServer();
        int acceptConnection();
        void removeClient(Client *client);
};

#endif