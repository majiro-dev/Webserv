/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:02:17 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/19 13:34:08 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP


# include "Utils.hpp"

enum AllowMethods
{
	GET,
	POST,
	DELETE,
};

class Request
{
    private:
        std::string                             _req_msg;
        std::string                             _method;
        std::string                             _uri;
        std::string                             _protocol;
        std::string                             _body;
        std::string                             _resource;
        std::map<std::string, std::string>      _headers;
        std::multimap<std::string, std::string> _querys;
        
        int                                     parseFirstLine(std::string &req);
        void                                    parseHeader(std::string line);
        void                                    parseBody();
    public:
        Request();
        Request(const std::string &msg);
        Request(const Request &src);
        ~Request();
        
        void                                    parseQuerys(std::string &uri);
        void                                    print();
        std::string                             getHeader(std::string find);
        std::string                             getMethod();
        std::string                             getUri();
        std::string                             getBody();
        std::string                             getProtocol();
        std::string                             getResource();
        std::multimap<std::string, std::string> getQuerys();
};

#endif