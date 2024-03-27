/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 22:07:00 by cmorales          #+#    #+#             */
/*   Updated: 2024/03/27 01:18:33 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP


# include "Utils.hpp"


# define HTTP_PROTOCOL "HTTP/1.1"

class Response
{
    private:
        unsigned int _statusCode;
        std::string _protocol;
        std::string _body;
        unsigned int _bodyLen;
        std::map<unsigned int, std::string>_code_msgs;
        std::multimap<std::string, std::string>_headers;
        
        void init_code_message();
    public:
        Response();
        Response(unsigned int code);
        ~Response();
        
        Response& operator=(const Response& src);
        
        std::string getProtocol();
        std::string getBody();
        unsigned int getStatusCode();
        std::string getStatusMsg();
        
        void addHeaders(std::string header_key, std::string header_value);
        void setBody(std::string body);
        void setStatusCode(const unsigned int newCode);
        
        std::string build_response();
        std::string buildErrorPage();
};



#endif