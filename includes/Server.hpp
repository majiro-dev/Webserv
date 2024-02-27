/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:07 by manujime          #+#    #+#             */
/*   Updated: 2024/02/27 18:05:23 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "Socket.hpp"
# include "outputMacros.hpp"
# include "Request.hpp"
# include "Utils.hpp"
# include <fcntl.h>
# include <poll.h>
# include <poll.h>
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
        
        std::string _ip_addr;
        int         _ret;
        int         _port;
        int         _socket;
        int         _new_socket;
        //long        _incoming_message;

        struct sockaddr_in      _socket_address;
        unsigned int            _address_length;
        std::string             _server_message;

        void startServer();
        void loopListen();
        void closeServer();
        void handleConnection(int &client_fd);
        int acceptConnection();
        std::string buildResponse();
        void sendResponse(int &client_fd);

};

#endif