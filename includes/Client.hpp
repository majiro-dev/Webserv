/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:00:02 by cmorales          #+#    #+#             */
/*   Updated: 2024/03/26 00:17:45 by cmorales         ###   ########.fr       */
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
        Response     _response;
        bool        _finishReq;
    public:
        Client();
        Client(int fd);
        ~Client();
        
        Client& operator=(const Client& src);
        
        Response hadleRequest(Request &request);
        void generateReponse();
        int handleRecv();
        int sendResponse();
        
        int getSocket();
        bool getfinishReq();
};

#endif