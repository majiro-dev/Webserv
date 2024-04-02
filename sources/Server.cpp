/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:01:39 by manujime          #+#    #+#             */
/*   Updated: 2024/04/02 22:33:12 by cmorales         ###   ########.fr       */
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
    this->_locations = config.GetLocations();
   /*  std::vector<Config>::iterator it = this->_locations.begin();
    for(; it != this->_locations.end(); *it++)
    {
        std::cout << "QPQP: " << it->GetRoot() << std::endl;
    } */
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
    (void)client;
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
        //Utils::logger("Add with code " + Utils::IntToString(it->first) + " the page with the root" + it->second, INFO);
    }
}

Config *Server::getLocation(Request &request)
{
    std::string req_path = request.getUri();
    std::string location = "";
    size_t len = 0;
    Config *loc = NULL;

    std::vector<Config>::iterator it = this->_locations.begin();

    for(; it != this->_locations.end(); it++)
    {
        std::cout << "IT: " << it->GetRoot() << std::endl;
        location = it->GetRoot().substr(it->GetRoot().find_last_of('/'));
        len = location.size();
        std::cout << "Location: " << location << std::endl;
        //std::cout << "Location: " << len << std::endl;
        if(location[len - 1] == '/')
        {
            len -= 1;
            location = location.substr(0, len);
        }
        std::cout << "Location2: " << location << std::endl;
        std::cout << "Req_paht: " << req_path << std::endl;
        req_path = req_path.substr(0, len);
        //std::cout << "Req_paht: " << req_path << std::endl;
        std::cout << "Req_paht2: " << req_path << std::endl;
        if(location.compare(req_path) == 0)
        {
            if(loc == NULL)
                loc = &(*it);
        }
    }
    return loc;
}

std::string getFilePath(Config *location, Request &request)
{
    size_t len = location->GetRoot().find_last_of('/');
    std::string root = location->GetRoot().substr(0, len);
    std::string path = request.getUri();
    std::cout << "ROOT: " << root << std::endl;
    std::cout << "PATH_UNIT: " << path << std::endl;
    
    if(!path.size())
        return root;
    if(root[root.size() - 1] != '/')
        root += "/";
    if(path[0] == '/')
        root += path.substr(1);
    else
        root + path;
    return root;
} 



Response Server::hadleRequest(Request &request)
{
    //Cambiar con la configuracion
    Response response;
    Config *location = this->getLocation(request);
    //std::string path = getFilePath(location, request);
    if(location == NULL)
    {
        std::cout << "No entro" << std::endl;
        return Response(404);
    }
    //conf->PrintConfig();
   /*  std::map<std::string, bool> allowedMethods;
    allowedMethods.insert(std::make_pair("GET", this->_allowMethods[0]));
    allowedMethods.insert(std::make_pair("POST", this->_allowMethods[1]));
    allowedMethods.insert(std::make_pair("DELETE", this->_allowMethods[2])); */

    if(request.getMethod() == "GET")
    {
        std::cout << "Entro" << std::endl;
        //response = Methods::HandleGet(path, *location);
        return response;
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
        Utils::logger("ERROR PAGE Page not found for this status error", ERROR);
    }
    if(!path.size())
    {
        std::cout << "ERROR PAGE ENTRA2 " << std::endl;
        std::cout << "ERROR PAGE HOLA" << std::endl;
        body = response.buildErrorPage();
    }
    else
    {
        std::ifstream file(path.c_str());
        std::stringstream ss;
        if(file.good())
        {
            std::cout << "ERROR PAGE ENTRA " << std::endl;
            ss << file.rdbuf();
            file.close();
            body = ss.str();
        }
        else
        {
            //std::cout << "ENTRA3 " << std::endl;
            Utils::logger("ERROR PAGE Path can not opened", ERROR);
            body = response.buildErrorPage();    
        }
    }
    response.setBody(body);
}
