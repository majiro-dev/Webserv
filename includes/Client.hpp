/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:00:02 by cmorales          #+#    #+#             */
/*   Updated: 2024/03/26 00:30:15 by cmorales         ###   ########.fr       */
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
        unsigned int _fd;
        std::string _request;
        bool        _finishReq;
    public:
        Client();
        Client(int fd);
        ~Client();
        
        Client& operator=(const Client& src);
        
        int handleRecv();
        int sendResponse(Response res);
        
        int getSocket();
        bool getfinishReq();
        std::string getRequest();
};

#endif