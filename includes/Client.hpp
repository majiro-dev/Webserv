/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:00:02 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/19 13:40:01 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP


# include "Client.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "Utils.hpp"

class Client
{
    private:
        unsigned int    _fd;
        std::string     _request;
        bool            _finishReq;
        sockaddr_in     _socketaddr;
    public:
        Client();
        Client(int fd, sockaddr_in socketaddr);
        ~Client();
        Client(const Client& src);
        Client& operator=(const Client& src);
        
        int         handleRecv();
        int         sendResponse(Response res);
        
        int         getSocket();
        bool        getfinishReq();
        std::string getRequest();
        sockaddr_in getSocketaddr();
};

#endif