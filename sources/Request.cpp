/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:22:56 by cmorales          #+#    #+#             */
/*   Updated: 2024/02/28 19:08:46 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"
#include <string>

Request::Request(const std::string &msg)
    : _method(""), _uri(""),_protocol(""), _body("")
{
    try
    {
        size_t len = 0;
        std::string request;
        std::string header;
        
        if(msg.empty())
            Utils::exceptWithError("Request empty");
            
        
        if((len = msg.find("\r\n")) == std::string::npos)
            Utils::exceptWithError("Dont have \\r\\n");

        request = msg.substr(0, len);
        if(parseFirstLine(request) == 1)
            Utils::exceptWithError("Invalid format");
        
        header = msg.substr(len + 2);
        if((len = msg.find("\r\n\r\n")) == std::string::npos)
            Utils::exceptWithError("No good final - \\r\\n\\r\\n");
        
        len = header.find("\r\n");
        while(len != std::string::npos)
        {
            parseHeader(header.substr(0, len + 2));
            header = header.substr(len + 2);
            len = header.find("\r\n");
            if(len <= 2)
                break;
        }
        if((msg.size() - (msg.find("\r\n\r\n") + 4)) > 0)
            this->_body = msg.substr(msg.find("\r\n\r\n") + 4);
        
        print();
    }
    catch(const MyError& e)
    {
        //Error 400
        std::cerr << RED << e.what() << '\n' << RESET;
    }
    
}

Request::~Request(){}

int Request::parseFirstLine(std::string &req)
{
    size_t f_len = 0;
    f_len = req.find(' ');
    if(f_len != std::string::npos)
    {
        this->_method = req.substr(0, f_len);
        if(_method != "GET" && _method != "POST" && _method != "DELETE")
            Utils::exceptWithError("No method available");
        size_t s_len = req.find(' ', f_len + 1);
        
        if(s_len != std::string::npos)
        {
            this->_uri = req.substr(f_len + 1, s_len - f_len -1);
            parseUri();
            if(s_len != std::string::npos)
                this->_protocol = req.substr(s_len + 1);
            return 0;
        }
    }
    return 1;
}

void Request::parseHeader(std::string line)
{
    std::string key;
    std::string value;

    size_t len = 0;
    if((len = line.find("\r\n")) == std::string::npos)
            Utils::exceptWithError("Dont have \\r\\n");
        
    if((len = line.find(':')) == std::string::npos)
        Utils::exceptWithError("No find : in the header format");

    
    key = line.substr(0, len);
    line = line.substr(len + 1);
    len = 0;
    if(line[len] == ' ')
        if(line[len + 1] == ' ')
            Utils::exceptWithError("More that one space found");
            
    line = line.substr(len + 1);
    len = line.find("\r\n");
    value = line.substr(0, len);
    
    this->_headers.insert(std::make_pair(key, value)); 
}

/*
Parse body? preguntar por 
Transfer-Encoding: chuncked
Content-Length: 123
Hace falta parsearlo
*/

//Mejorarlo
void Request::parseUri()
{
    std::string key;
    std::string value;
    std::string aux;
    std::string querys;
    size_t len = 0;
    size_t equal = 0;
    
    len = this->_uri.find('?');
    //Obtener recursos
    this->_resource = this->_uri.substr(0, len);
    //Obtner cadenas de consultas
    querys = this->_uri.substr(len + 1);
    
    len = querys.find('&');
    while(len != std::string::npos)
    {
        aux =  querys.substr(0, len);
        equal = aux.find('=');
        key = aux.substr(0, equal);
        value = aux.substr(equal + 1, len);
        
        this->_querys.insert(std::make_pair(key, value));
        
        querys = querys.substr(len + 1);
        len = querys.find('&');
    }
    aux =  querys.substr(0, len);
    equal = aux.find('=');
    key = aux.substr(0, equal);
    value = aux.substr(equal + 1, len);
    this->_querys.insert(std::make_pair(key, value));
}


std::string Request::getHeader(std::string find)
{
    if(this->_headers.find(find) == this->_headers.end())
        return "";
    return this->_headers[find];
}


std::string Request::getMethod()
{
    return this->_method;    
}

std::string Request::getBody()
{
    return this->_body; 
}

std::string Request::getProtocol()
{
    return this->_protocol;
}


void Request::print()
{
    std::map<std::string, std::string>::iterator it = this->_headers.begin();
    std::multimap<std::string, std::string>::iterator it2 = this->_querys.begin();
    std::cout << "==================== Request ====================" << std::endl;
    std::cout << "METHOD: " << this->_method << std::endl << std::endl;
    
    std::cout << "URI: " << this->_uri << std::endl << std::endl;
    std::cout << "RESOURCE: " << this->_resource << std::endl << std::endl;
    std::cout << "QUERYS: " << std::endl;
    for(; it2 != this->_querys.end(); it2++){
        std::cout << it2->first << "=" << it2->second << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "PROTOCOL: " << this->_protocol << std::endl << std::endl;
    
    std::cout << "HEADERS: "<< std::endl;
    for(; it != this->_headers.end(); it++){
        std::cout << it->first << ":" << it->second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "BODY: " << this->_body << std::endl;
    std::cout << "================== Request-end ==================" << std::endl;
}