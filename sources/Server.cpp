/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/04/21 18:59:53 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(Config config):
    _config(config)
{
    std::stringstream ss;
    std::string name = config.GetServerName();
    
    this->_name = name.size() ? name : "server";
    this->_ports = config.GetPorts();
    this->_host = config.GetHost();
    this->_maxBodySize = config.GetClientMaxBodySize();
    this->_locations = config.GetLocations();
    addSocketsServer();
    ss << "New server started => " << '[' << this->_name << ']';
    Utils::logger(ss.str(), INFO);
    
}

Server::Server(const Server &src)
{
    *this = src;
}

Server::~Server(void)
{
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

Response Server::getResponse()
{
    return this->_response;
}

std::string Server::getName()
{
    return this->_name;
}

void Server::removeClient(Client *client)
{
    std::vector<Client*>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), client);
    if(it != this->_clients.end())
    {
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
        
        if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) 
            Utils::exceptWithError("Error setting socket to non-blocking");

        if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
            Utils::exceptWithError("Error failed to set socket options");
            
        memset(&sockaddr, 0, sizeof(sockaddr));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = this->_host;
        sockaddr.sin_port = htons(this->_ports[i]);
        
        if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
            Utils::exceptWithError(ERROR_SOCKET_BIND);
            
        if (listen(sock, MAX_CLIENTS) < 0)
            Utils::exceptWithError(ERROR_SOCKET_LISTEN);
            
        
        this->_sockets.push_back(sock);
        this->_sockaddrs.push_back(sockaddr);
    }
}

void Server::addErrorPage(Config &config)
{
    this->_errorPages.clear();
    std::map<int, std::string> errorPages = config.GetErrorPages();
    std::map<int, std::string>::iterator it = errorPages.begin();
    
    for(; it != errorPages.end(); it++)
    {
        if(it->first < 0)
        {
            Utils::logger("Invalid status code for the error page", ERROR);
            it++;
        }
        this->_errorPages.insert(std::make_pair(it->first, it->second));
        //Utils::logger("Add with code " + Utils::IntToString(it->first) + " the page with the root" + it->second, INFO);
    }
}

bool checkAllowMethods(Config *location, const std::string& method)
{
    std::map<std::string, bool> allowedMethods;

    allowedMethods.insert(std::make_pair("GET", location->GetAllowMethods()[0]));
    allowedMethods.insert(std::make_pair("POST", location->GetAllowMethods()[1]));
    allowedMethods.insert(std::make_pair("DELETE", location->GetAllowMethods()[2]));
    for(std::map<std::string, bool>::iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++)
    {
        if(it->first == method)
        {
            if(it->second == true)
                return true;
        }
    }
    return false;
}


Config *Server::getLocation(Request &request)
{
    std::string location = "";
    unsigned long len = 0;
    Config *loc = NULL;
    unsigned long other_loc = 0;
    std::vector<Config>::iterator it = this->_locations.begin();

    for(; it != this->_locations.end(); it++)
    {
        std::string req_path = request.getResource();
        location = it->GetLocationName();
        len = location.size();
        if(location[len - 1] == '/')
        {
            len -= 1;
            location = location.substr(0, len);
        } 
        req_path = req_path.substr(0, len);
        if(location.compare(req_path) == 0)
        {
            if(loc == NULL || len > other_loc)
            {
                other_loc = len;
                loc = &(*it);
            }
        }
    }
    return loc;
}

std::string getFilePath(Config *location, Request &request)
{
    size_t len = location->GetLocationName().size();
    std::string root = location->GetRoot();
    std::string path = request.getResource();
  
    path = path.substr(len);
    if(!path.size())
        return root;
    if(root[root.size() - 1] != '/')
        root += "/";
    if(path[0] == '/')
        root += path.substr(1);
    else 
        root += path;
        
    return root;
}

void Server::generateResponse(const std::string& request, sockaddr_in socketaddr)
{
    try
    {
        std::cout << std::endl << MAGENTA  << request << std::endl << RESET;
        Request req(request);
        Config *location = this->getLocation(req);
        if(location == NULL)
            Utils::exceptWithError("No location");
        addErrorPage(*location);

        char clientSock[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(socketaddr.sin_addr), clientSock, INET_ADDRSTRLEN);
        uint16_t clientPort = ntohs(socketaddr.sin_port);
        Utils::logger(std::string(clientSock) + ":" + Utils::IntToString(clientPort) + " -> " + req.getMethod() + " " + req.getResource(), INFO);
            
        this->_response = hadleRequest(req, location);
    }
    catch(const std::exception& e)
    {
        Utils::logger(e.what(), ERROR);   
        this->_response = Response(404);
        addErrorPage(this->_config);
    }
    if(this->_response.getStatusCode() >= 400)
        putErrorPage(this->_response);
}

std::string giveContenType(Response &response, std::string path)
{
    std::string mime_type;
    size_t len = path.find(".");

    if((int)len == -1)
        return response.getMimeType("default");
    path = path.substr(len);
    mime_type = response.getMimeType(path);
    return mime_type;
}

Response Server::hadleRequest(Request &request, Config *location)
{
    Response response;
    if(request.getProtocol() != "HTTP/1.1")
    {
        Utils::logger("Invalid protocol", ERROR);
        return Response(505);
    }
    if (request.getMethod() != "GET" && request.getMethod() != "POST" && request.getMethod() != "DELETE")
    {
        Utils::logger("Unknown method: " + request.getMethod(), ERROR);
        return Response(501);
    }
    if(checkAllowMethods(location, request.getMethod()) == false)
    {
        Utils::logger("Method is not allowed", ERROR);
        return Response(405);
    }
    if(request.getBody().size() > location->GetClientMaxBodySize())
        {
            response.setStatusCode(413);
            this->_response.setBody("The size of the request body exceeds the allowed limit");
            Utils::logger("Request body exceeds the allowed limit", ERROR);
            return response;
    }
    if(request.getHeader("Content-Length") == "" && request.getBody().size() != 0)
    {
        Utils::logger("Request without a defined Content- Length", ERROR);
        return Response(411);
    }
    if(request.getProtocol() != "HTTP/1.1")
    {
        Utils::logger("Invalid protocol", ERROR);
        return Response(505);
    }
	response.addHeaders("Server", this->_name);
    std::string redirect = location->GetRedirect();
    if(!redirect.empty())
    {
        response.setStatusCode(301);
        response.addHeaders("Date", Utils::giveDate());
        response.addHeaders("Location", redirect);
        return response;
    }
    std::string path = getFilePath(location, request);
    if(request.getMethod() == "GET")
    {
        response = Methods::HandleGet(path, *location, request);
        response.addHeaders("Content-type", giveContenType(response, path));

    }
    else if (request.getMethod() == "POST")
    {
        response = Methods::HandlePost(path, request, *location);
    }
    else if(request.getMethod() == "DELETE")
    {
        response = Methods::HandleDelete(path);
    }
    else
    {
        response.setStatusCode(404);
        Utils::logger("This method was not found", ERROR);
    }
    response.addHeaders("Date", Utils::giveDate());
    response.addHeaders("Connection", "close");
    return response;
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
        body = response.buildErrorPage();
    else
    {
        path = path.substr(0, path.size() -1);
        std::ifstream file(path.c_str());
        std::stringstream ss;
        if(file.is_open())
        {
            ss << file.rdbuf();
            file.close();
            body = ss.str();
        }
        else
        {
            Utils::logger("PAGE Path can not opened", ERROR);
            body = response.buildErrorPage();    
        }
    }
    response.setBody(body);
    response.addHeaders("Content-type", "text/html");
}
