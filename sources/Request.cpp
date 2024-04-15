/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatas-p <jmatas-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:22:56 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/15 11:17:23 by jmatas-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"
#include <string>

Request::Request()
    : _req_msg(""),_method(""), _uri(""),_protocol(""), _body("")
{}

Request::Request(const std::string &msg)
    : _req_msg(msg),_method(""), _uri(""),_protocol(""), _body("")
{
    
    size_t len = 0;
    std::string request;
    std::string header;
        
    if(this->_req_msg.empty())
        Utils::exceptWithError("Request empty");
            
        
    if((len = this->_req_msg.find("\r\n")) == std::string::npos)
        Utils::exceptWithError("Dont have \\r\\n");

    request = this->_req_msg.substr(0, len);
    if(parseFirstLine(request) == 1)
        Utils::exceptWithError("Invalid format");
        
    header = this->_req_msg.substr(len + 2);
    if((len = this->_req_msg.find("\r\n\r\n")) == std::string::npos)
        Utils::exceptWithError("Unexpected final - \\r\\n\\r\\n");
        
    len = header.find("\r\n");
    while(len != std::string::npos)
    {
        parseHeader(header.substr(0, len + 2));
        header = header.substr(len + 2);
        len = header.find("\r\n");
        if(len <= 2)
            break;
    }
    if((this->_req_msg.size() - (this->_req_msg.find("\r\n\r\n") + 4)) > 0)
        this->_body = this->_req_msg.substr(this->_req_msg.find("\r\n\r\n") + 4);
        
    parseBody();
}

Request::~Request(){}

int Request::parseFirstLine(std::string &req)
{
    size_t f_len = 0;
    f_len = req.find(' ');
    if(f_len != std::string::npos)
    {
        this->_method = req.substr(0, f_len);
        size_t s_len = req.find(' ', f_len + 1);
        
        if(s_len != std::string::npos)
        {
            this->_uri = req.substr(f_len + 1, s_len - f_len -1);
            parseQuerys(this->_uri);
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

//Mejorarlo****
void Request::parseQuerys(std::string &uri)
{
    std::string key;
    std::string value;
    std::string aux;
    std::string querys;
    size_t len = 0;
    size_t equal = 0;
    
    len = uri.find('?');
    if(len == std::string::npos)
        return ;
    //Obtener recursos
    this->_resource = uri.substr(0, len);
    //Obtner cadenas de consultas
    querys = uri.substr(len + 1);
    
    len = querys.find('&');
    while(len != std::string::npos)
    {
        std::cout << "Entrra" << std::endl;
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

static bool checkNumString(std::string& s)
{
    bool val = true;
    
   for(size_t i = 0; i < s.size(); i++)
   {
        if(!isdigit(s[i]))
            return false;
   }
   return val;
}

void Request::parseBody()
{
    std::string headerSize = this->getHeader("Content-Length");

    if(headerSize != "")
    {
        if(!checkNumString(headerSize))
            Utils::exceptWithError("Content-Length value is not a number");
        int size = Utils::StringToInt(headerSize);
        std::cout << "Content-Length: " << size << std::endl;
        if(size < 0)
            Utils::exceptWithError("Content-Length value is an invalid number");
        std::cout << "Body size: " << this->_body.size() << std::endl;
        if((size_t)size < this->_body.size())
            Utils::exceptWithError("Content-Length is smaller than body");
    }
    else if(this->getHeader("Transfer-Encoding") == "chuncked")
        return ;
    else
        this->_body = "";
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

std::string Request::getUri()
{
    return this->_uri;    
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