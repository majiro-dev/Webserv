/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:02:17 by cmorales          #+#    #+#             */
/*   Updated: 2024/02/26 21:05:32 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP


# include "Server.hpp"

enum AllowMethods
{
	GET,
	POST,
	DELETE,
};

class Request
{
    private:
        std::string _method;
        std::string _uri;
        std::string _protocol;
        std::string _body;
        std::string _resource;
        std::map<std::string, std::string> _headers;
        std::multimap<std::string, std::string> _querys;
    public:
        Request(const std::string &msg);
        ~Request();
        int parseFirstLine(std::string &req);
        void print();
        void parseHeader(std::string line);
        void parseUri();
        //void parseBody();
        std::string getHeader(std::string find);
        std::string getMethod();
        std::string getBody();
        std::string getProtocol();
};

#endif