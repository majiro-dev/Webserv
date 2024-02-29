/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 22:16:53 by cmorales          #+#    #+#             */
/*   Updated: 2024/02/29 12:23:36 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

Response::Response()
    : _statusCode(200), _protocol(HTTP_PROTOCOL), _body("")
{
    init_code_message();
}

Response::Response(unsigned int code)
    : _statusCode(code), _protocol(HTTP_PROTOCOL), _body("")
{
    init_code_message();
    std::map<unsigned int, std::string>::iterator it = this->_code_msgs.find(code);
    if(it == this->_code_msgs.end())
        this->_statusCode = 500;
}


Response::~Response() {}

unsigned int Response::getStatusCode()
{
    return this->_statusCode;
}


std::string Response::getStatusMsg(const unsigned int code)
{
    std::map<unsigned int, std::string>::iterator it;

    it = this->_code_msgs.find(code);
    if(it == this->_code_msgs.end())
        return "";
    return it->second;
}


std::string Response::getProtocol()
{
    return this->_protocol;
}

std::string Response::getBody()
{
    return this->_body;
}


void Response::init_code_message()
{
    this->_code_msgs.insert(std::make_pair(200, "OK"));
    this->_code_msgs.insert(std::make_pair(400, "Bad Request"));
    this->_code_msgs.insert(std::make_pair(500, "Internal Server Error"));
}


void Response::addHeaders(std::string header_key, std::string header_value)
{
    this->_headers.insert(std::make_pair(header_key, header_value));
}

std::string Response::build_response()
{
    std::stringstream ss;

    ss << getProtocol() << ' ' << getStatusCode() << ' ' << getStatusMsg(this->_statusCode) << "\r\n";
    std::cout << "RESPONSE LINE: " << ss.str() << std::endl;

    return ss.str();
}