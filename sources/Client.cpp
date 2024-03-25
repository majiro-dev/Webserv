/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 20:59:16 by cmorales          #+#    #+#             */
/*   Updated: 2024/03/23 20:06:05 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
}

Client::Client(int fd)
   : _fd(fd)
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
    //std::cout << "REC: " << this->getSocket() << std::endl;
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
            //std::cout << "Solicitud completa. Cuerpo del mensaje: " << this->_request.substr(pos + 4, nRes) << std::endl;
        } else {
            this->_finishReq = false;
            //std::cout << "Solicitud parcial. Esperando más datos." << std::endl;
        }
    }
    else if(this->_request.find("Transfer-Encoding: chunked") != std::string::npos)
    {
        std::cout << "HOAL" << std::endl;
        if(this->_request.substr(_request.size() - 5) == "0\r\n\r\n")
            this->_finishReq = true;
        else
            this->_finishReq = false;
    }
    else 
    {
        this->_finishReq = true;
        //std::cout << "Solicitud sin Content-Length. Cuerpo del mensaje: " << this->_request.substr(pos + 4) << std::endl;
    }
    return 0;
}

Response Client::hadleRequest(Request &request)
{
    //Cambiar con la configuracion
    Response response;
    
    //Meter metodos devulva un reponse
    if(request.getMethod() == "GET")
        Utils::logger("Aplicar metodo GET", LOG);
    else if (request.getMethod() == "POST")
        Utils::logger("Aplicar metodo POST", LOG);
    else if (request.getMethod() == "DELETE")
        Utils::logger("Aplicar metodo DELETE", LOG);
    else
        response.setStatusCode(404);
    return response;
}


void Client::generateReponse()
{
    bool isBrowser = false;
    
    try
    {
        Request req(this->_request);
        if(req.getHeader("User-Agent") != "")
            isBrowser = true;
        this->_response = hadleRequest(req);
    }
    catch(const std::exception& e)
    {
        Utils::logger(e.what(), ERROR);        
        this->_response = Response(400);
    }
    if(this->_response.getStatusCode() >= 400)
    {
        if(isBrowser)
            this->_response.setBody(buildErrorPage(this->_response));
        else
            this->_response.setBody(_response.getStatusMsg());
    }
}

int Client::sendResponse()
{
    unsigned long bytesSenT;
    std::string response = this->_response.build_response();
    bytesSenT = write(this->getSocket(), response.c_str(), response.size());
    if (bytesSenT == response.size())
        Utils::logger("Response sent successfully.", LOG);
    else
    {
        Utils::logger("Failed to send response", ERROR);
        return -1;
    }
    close(this->getSocket());
    return 0;
}