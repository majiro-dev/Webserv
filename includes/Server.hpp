/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:07 by manujime          #+#    #+#             */
/*   Updated: 2024/02/02 11:44:33 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"


class Server
{
    public:
        Server(std::string ip_address, int port);
        Server(Config config);
        ~Server(void);
        void startListen();

    private:
        Config  _config;
        
        std::string _ip_address;
        int         _port;
        int         _socket;
        int         _new_socket;
        long        _incoming_message;

        struct sockaddr_in      _socket_address;
        unsigned int            _address_length;
        std::string             _server_message;

        int startServer();
        void closeServer();
        void acceptConnection(int &new_socket);
        std::string buildResponse();
        void sendResponse();

};

#endif