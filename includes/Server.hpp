/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:07 by manujime          #+#    #+#             */
/*   Updated: 2024/02/06 20:01:37 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"


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
        
        std::string _ip_addr;
        int         _port;
        int         _sock;
        int         _connect_sock;
        //long        _incoming_message;

        struct sockaddr_in      _socketaddr;
        unsigned int            _addrlen;
        std::string             _server_message;

        int startServer();
        void loopListen();
        void closeServer();
        void acceptConnection(int &new_socket);
        std::string buildResponse();
        void sendResponse();

};

#endif