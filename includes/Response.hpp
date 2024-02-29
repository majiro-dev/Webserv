/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 22:07:00 by cmorales          #+#    #+#             */
/*   Updated: 2024/02/29 12:20:22 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP


# include "Server.hpp"


# define HTTP_PROTOCOL "HTTP/1.1"

class Response
{
    private:
        unsigned int _statusCode;
        std::string _protocol;
        std::string _body;
        //unsigned int _bodyLen;
        std::map<unsigned int, std::string>_code_msgs;
        std::multimap<std::string, std::string>_headers;
    public:
        Response();
        Response(unsigned int code);
        ~Response();
        
        std::string getProtocol();
        std::string getBody();
        unsigned int getStatusCode();
        std::string getStatusMsg(const unsigned int code);
        
        void init_code_message();
        void addHeaders(std::string header_key, std::string header_value);
        std::string build_response();
};

#endif