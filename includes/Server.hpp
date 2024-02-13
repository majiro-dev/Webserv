/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:07 by manujime          #+#    #+#             */
/*   Updated: 2024/02/13 00:00:36 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include <fcntl.h>


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
        int         _sock;
        int         _connect_sock;
        int         _max_socket;
        //long        _incoming_message;
        
        //std::vector<int>max_fds;
        fd_set read_set, write_set; 
        
        struct sockaddr_in      _socketaddr;
        unsigned int            _addrlen;
        std::string             _server_message;

        void startServer();
        void loopListen();
        void closeServer();
        void handleConnection(int &client_fd);
        void acceptConnection(int &new_socket);
        std::string buildResponse();
        void sendResponse(int &client_fd);

};

#endif