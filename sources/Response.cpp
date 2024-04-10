/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 22:16:53 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/10 11:49:01 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

Response::Response()
    : _statusCode(200), _protocol(HTTP_PROTOCOL), _body("")
{
    init_code_message();
    init_mime_types();
}

Response::Response(unsigned int code)
    : _statusCode(code), _protocol(HTTP_PROTOCOL), _body("")
{
    init_code_message();
    init_mime_types();
    std::map<unsigned int, std::string>::iterator it = this->_code_msgs.find(code);
    if(it == this->_code_msgs.end())
        this->_statusCode = 500;
}


Response::~Response() {}

unsigned int Response::getStatusCode()
{
    return this->_statusCode;
}

Response& Response::operator=(const Response& src)
{
    if(this != &src)
    {
        this->_statusCode = src._statusCode;
        this->_code_msgs = src._code_msgs;
        this->_protocol = src._protocol;
        this->_headers = src._headers;
        this->_body = src._body;
        this->_bodyLen = src._bodyLen;
    }
    return *this;
}

void Response::init_code_message()
{
    this->_code_msgs.insert(std::make_pair(200, "OK"));
    this->_code_msgs.insert(std::make_pair(201, "Created"));
    this->_code_msgs.insert(std::make_pair(202, "Acepted"));
    this->_code_msgs.insert(std::make_pair(300, "Multiple Choices"));
    this->_code_msgs.insert(std::make_pair(301, "Moved Permanently"));
    this->_code_msgs.insert(std::make_pair(400, "Bad Request"));
    this->_code_msgs.insert(std::make_pair(401, "Unauthorized"));
    this->_code_msgs.insert(std::make_pair(402, "Payment Required"));
    this->_code_msgs.insert(std::make_pair(403, "Forbiddent"));
    this->_code_msgs.insert(std::make_pair(404, "Not Found"));
    this->_code_msgs.insert(std::make_pair(405, "Method Not Allowed"));
    this->_code_msgs.insert(std::make_pair(413, "Exceeds Body Limit"));
    this->_code_msgs.insert(std::make_pair(500, "Internal Server Error"));
    this->_code_msgs.insert(std::make_pair(505, "HTTP Version Not Supported"));
}

void Response::init_mime_types()
{
    this->_mime_types.insert(std::make_pair("default", "text/html"));
    this->_mime_types.insert(std::make_pair(".html", "text/html"));
    this->_mime_types.insert(std::make_pair(".htm", "text/html"));
    this->_mime_types.insert(std::make_pair(".css", "text/css"));
    this->_mime_types.insert(std::make_pair(".bmp", "image/bmp"));
    this->_mime_types.insert(std::make_pair(".gif", "image/gif"));
    this->_mime_types.insert(std::make_pair(".ico", "image/x-icon"));
    this->_mime_types.insert(std::make_pair(".jpg", "image/jpeg"));
    this->_mime_types.insert(std::make_pair(".jpeg", "image/jpeg"));
    this->_mime_types.insert(std::make_pair(".png", "image/png"));
    this->_mime_types.insert(std::make_pair(".txt", "text/plain"));
    this->_mime_types.insert(std::make_pair(".avi", "video/x-msvideo"));
    this->_mime_types.insert(std::make_pair(".mp4", "video/mp4"));
    this->_mime_types.insert(std::make_pair(".mp3", "audio/mp3"));
    this->_mime_types.insert(std::make_pair(".pdf", "application/pdf"));
}

void Response::setStatusCode(const unsigned int newCode)
{
    this->_statusCode = newCode;
}

void Response::addHeaders(std::string header_key, std::string header_value)
{
    this->_headers.insert(std::make_pair(header_key, header_value));
}

void Response::setBody(std::string body)
{
    this->_body += body;
    this->_bodyLen = body.size();
    addHeaders("Content-lent", Utils::IntToString(this->_bodyLen));
}

std::string Response::getStatusMsg()
{
    std::map<unsigned int, std::string>::iterator it;

    it = this->_code_msgs.find(this->getStatusCode());
    if(it == this->_code_msgs.end())
        return "";
    return it->second;
}

std::string Response::getMimeType(const std::string &key)
{
    std::map<std::string, std::string>::iterator it;

    it = this->_mime_types.find(key);
    if(it == this->_mime_types.end())
        return this->_mime_types["default"];
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

std::string Response::buildErrorPage()
{
    std::stringstream html;
    
    html << "<html>\n"
                  "<head><title>" << this->getStatusCode() << " " << this->getStatusMsg() << "</title></head>\n"
                  "<body style=\"text-align:center;\">"
                  "<h1>" << this->getStatusCode() << " " << this->getStatusMsg()<< "</h1>\n"
                  "<hr>\n"
                  "<p>" << "Webserver 1" << "</p>\n"
                  "</body>\n"
                  "</html>";
    return html.str();
}


std::string Response::build_response()
{
    std::stringstream ss;
    std::multimap<std::string, std::string>::iterator it;

    ss << getProtocol() << ' ' << getStatusCode() << ' ' << getStatusMsg() << "\r\n";
    for(it = this->_headers.begin(); it != this->_headers.end(); it++){
        ss << it->first << ": " << it->second << "\r\n";    
    }
    ss << "\r\n";
    if(this->_body.size())
        ss << this->_body;
    return ss.str();
}
void Response::setFile(std::string &file)
{
    this->_file = file;
}
