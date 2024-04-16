/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 20:59:16 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/16 13:09:36 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
}

Client::Client(int fd, sockaddr_in socketaddr)
   : _fd(fd), _socketaddr(socketaddr)
{
}

Client::~Client(){}


Client& Client::operator=(const Client& src)
{
    if(this != &src)
    {
        this->_fd = src._fd;
        this->_finishReq = src._finishReq;
        this->_request = src._request;
    }
    return *this;
}


int Client::getSocket()
{
    return this->_fd;
}

bool Client::getfinishReq()
{
    return this->_finishReq;
}


int Client::handleRecv()
{
    if(this->_finishReq == true)
        return 0;
    int bytesReceived = 0;
    char buffer[BUFFER_SIZE + 1] = {0};
    
    bytesReceived = recv(this->getSocket(), buffer, BUFFER_SIZE, 0);
    if (bytesReceived <= 0)
    {
        close(this->getSocket());
        return -1;
    } 
    this->_request += buffer;

    std::string method = _request.substr(0,  _request .find(" "));
    if (method == "POST")
    {
        this->_finishReq = true;
        return 0;
    }
    
    size_t pos = this->_request.find("\r\n\r\n");
    if(pos == std::string::npos)
    {
        this->_finishReq = false;
        return 1;
    }
    
    size_t contentPos = this->_request.find("Content-Length: ");
    if(contentPos != std::string::npos)
    {
        size_t endline = this->_request.find("\n", contentPos); 
        std::string contentRes = _request.substr((contentPos + 16), (endline - (contentPos + 16) ));
        size_t nRes = Utils::StringToInt(contentRes);

        if (this->_request.size() >= nRes + pos + 4) {
            this->_finishReq = true;
        } else {
            this->_finishReq = false;
        }
    }
    else if(this->_request.find("Transfer-Encoding: chunked") != std::string::npos)
    {
        if(this->_request.substr(_request.size() - 5) == "0\r\n\r\n")
            this->_finishReq = true;
        else
            this->_finishReq = false;
    }
    else
        this->_finishReq = true;
    return 0;
}



int Client::sendResponse(Response res)
{
    std::string response = res.build_response();
    const char* data = response.c_str();
    size_t totalBytesSent = 0;
    size_t bytesRemaining = response.size();
    size_t maxChunkSize = 1024;

    while (bytesRemaining > 0) {
        size_t bytesToSend = std::min(bytesRemaining, maxChunkSize);
        ssize_t bytesSent = write(this->getSocket(), data + totalBytesSent, bytesToSend);
        
        if (bytesSent == -1) {
            Utils::logger("Failed to send response", ERROR);
            close(this->getSocket());
            return -1;
        }

        totalBytesSent += bytesSent;
        bytesRemaining -= bytesSent;
    }

    Utils::logger("Response sent successfully.", LOG);
    close(this->getSocket());
    return 0;
}


std::string Client::getRequest()
{
    return this->_request;
}

sockaddr_in Client::getSocketaddr()
{
    return this->_socketaddr;
}