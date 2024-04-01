/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/04/01 17:43:57 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(Config config):
    _config(config)
{
    //config.PrintConfig();
    std::stringstream ss;
    std::string name = config.GetServerName();
    
    this->_name = name.size() ? name : "server";
    this->_ports = config.GetPorts();
    this->_host = config.GetHost();
    this->_maxBodySize = config.GetClientMaxBodySize();
    this->_allowMethods = config.GetAllowMethods();
    std::cout << "ROOT:" << config.GetRoot() << std::endl;
    std::cout << "INDEX:" << config.GetIndex() << std::endl;
    std::vector<Config> locations = config.GetLocations();
    std::vector<Config>::iterator it = locations.begin();
    std::cout << "LOCATIONS : " << std::endl;
    for(; it != locations.end(); it++)
    {
        it->PrintConfig();
    }
    checkErrorPage();
    addSocketsServer();
    ss << "New server started => " << '[' << this->_name << ']';
    Utils::logger(ss.str(), INFO);
    
}

Server::~Server(void)
{
    for(size_t i = 0; i < this->_ports.size(); i++)
    {
        close(this->_ports[i]);
    }
    for(size_t i = 0; i < this->_clients.size(); i++)
    {
        delete(this->_clients[i]);
    }
}

std::vector<uint16_t> Server::getSockets()
{
    return this->_sockets;
}

std::vector<sockaddr_in> Server::getSockaddrs()
{
    return this->_sockaddrs;
}

std::vector<Client *> Server::getClients()
{
    return this->_clients;
}


Config Server::getConfig()
{
    return this->_config;
}

Response Server::getReponse()
{
    return this->_response;
}

void Server::removeClient(Client *client)
{
    std::vector<Client*>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), client);
    if(it != this->_clients.end())
    {
        close(client->getSocket());
        delete *it;
        this->_clients.erase(it);
    }
}

void Server::addClient(Client *client)
{
    this->_clients.push_back(client);
}

void Server::addSocketsServer()
{   
    int on = 1;
    int sock;
    struct sockaddr_in sockaddr;
    for(size_t i = 0; i < this->_ports.size(); i++)
    {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            Utils::exceptWithError(ERROR_SOCKET_CREATE);

        if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
            Utils::exceptWithError("Error failed to set socket options");
            
        memset(&sockaddr, 0, sizeof(sockaddr));
        //**ECHAR UN OJO EXCEPTCION CON BIND => SEGMENTATION FAULT
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = this->_host;
        sockaddr.sin_port = htons(this->_ports[i]);
        
        if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
            Utils::exceptWithError(ERROR_SOCKET_BIND);
            
        if (listen(sock, MAX_CLIENTS) < 0)
            Utils::exceptWithError(ERROR_SOCKET_LISTEN);
            
        if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) 
            Utils::exceptWithError("Error setting socket to non-blocking");
        this->_sockets.push_back(sock);
        this->_sockaddrs.push_back(sockaddr);
    }
}

void Server::checkErrorPage()
{
    std::map<int, std::string> errorPages = _config.GetErrorPages();
    std::map<int, std::string>::iterator it = errorPages.begin();
    
    for(; it != errorPages.end(); it++)
    {
        if(it->first < 0)
        {
            Utils::logger("Invalid status code for the error page", ERROR);
            it++;
        }
        this->_errorPages.insert(std::make_pair(it->first, it->second));
        Utils::logger("Add with code " + Utils::IntToString(it->first) + " the page with the root" + it->second, INFO);
    }
}


Response Server::hadleRequest(Request &request)
{
    //Cambiar con la configuracion
    Response response;

    std::map<std::string, bool> allowedMethods;
    allowedMethods.insert(std::make_pair("GET", this->_allowMethods[0]));
    allowedMethods.insert(std::make_pair("POST", this->_allowMethods[1]));
    allowedMethods.insert(std::make_pair("DELETE", this->_allowMethods[2]));

    std::map<std::string, bool>::iterator it = allowedMethods.begin();

    for(; it != allowedMethods.end(); it++)
    {
        if(it->first == request.getMethod())
        {
            if(it->second)
            {
                std::string uri = request.getUri();
                if(uri == "/")
                    return Response(404);
                Utils::logger("Aplicar metodo " + it->first, LOG);
                return response;
            }
            else
            {
                Utils::logger("Method is not allowed", ERROR);
                return Response(405);
            }
        }
    }
    response.setStatusCode(404);
    Utils::logger("This method was not found", ERROR);
    //response.setBody("This method was not found");
    return response;
}


void Server::generateReponse(const std::string& request)
{
    bool isBrowser = false;
    (void)isBrowser;
    try
    {
        //std::cout << request << std::endl;
        Request req(request);
        if(req.getBody().size() > this->_maxBodySize)
        {
            this->_response = Response(413);
            this->_response.setBody("The size of the request body exceeds the allowed limit");
            Utils::logger("Request body exceeds the allowed limit", ERROR);
            return ;
        } 
        if(req.getHeader("User-Agent") != "")
            isBrowser = true;
        this->_response = hadleRequest(req);
    }
    catch(const std::exception& e)
    {
        Utils::logger(e.what(), ERROR);        
        this->_response = Response(404);
    }
    if(this->_response.getStatusCode() >= 400)
    {
        putErrorPage(this->_response);
       /*  if(isBrowser)
            this->_response.setBody(buildErrorPage();
        else
            this->_response.setBody(_response.getStatusMsg()); */
    }
}

void Server::putErrorPage(Response &response)
{
    std::string path;
    std::string body;
    try
    {
        path = this->_errorPages.at(response.getStatusCode());
    }
    catch(const std::exception& e)
    {
        Utils::logger("Page not found for this status error", ERROR);
    }
    if(!path.size())
    {
        std::cout << "ENTRA2 " << std::endl;
        std::cout << "HOLA" << std::endl;
        body = response.buildErrorPage();
    }
    else
    {
        std::ifstream file(path.c_str());
        std::stringstream ss;
        std::cout << path << std::endl;
        if(file.good())
        {
            std::cout << "ENTRA " << std::endl;
            ss << file.rdbuf();
            file.close();
            body = ss.str();
        }
        else
        {
            std::cout << "ENTRA3 " << std::endl;
            Utils::logger("Path can not opened", ERROR);
            body = response.buildErrorPage();    
        }
    }
    response.setBody(body);
}
