/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:00:02 by cmorales          #+#    #+#             */
/*   Updated: 2024/03/13 21:04:14 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP


# include "Client.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "Utils.hpp"
# include <poll.h>

class Client
{
    private:
        unsigned int _fd;
        std::string _request;
        Response     _response;
        bool        _finishReq;
    public:
        Client();
        Client(int fd);
        ~Client();
        
        Client& operator=(const Client& src);
        
        void handleRecv();
        Response hadleRequest(Request &request);
        void generateReponse();
        void sendResponse();
        
        int getSocket();
        bool getfinishReq();
        //std::stri
};

#endif