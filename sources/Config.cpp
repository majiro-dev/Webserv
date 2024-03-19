/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:02:10 by manujime          #+#    #+#             */
/*   Updated: 2024/03/19 11:29:09 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config.hpp"

Config::Config(void)
{
    this->_port = 8080;
    //this->_host = inet_addr("127.0.0.1");
    this->_root = "";
    this->_client_max_body_size = 0;
    this->_autoindex = false;
    this-> _allow_methods.resize(3);
    for (int i = 0; i < 3; i++)
        this->_allow_methods[i] = 0;
    return ;
}

Config::Config(const Config &src)
{
    this->_ports = src._ports;
    this->_host = src._host;
    this->_server_name = src._server_name;
    this->_root = src._root;
    this->_client_max_body_size = src._client_max_body_size;
    this->_index = src._index;
    this->_locations = src._locations;
    this->_error_pages = src._error_pages;
    this->_allow_methods = src._allow_methods;
    this->_autoindex = src._autoindex;
    this->_cgi_pass = src._cgi_pass;
    this->_cgi_extension = src._cgi_extension;
    this->_error_pages = src._error_pages;
    return ;
}

Config::~Config(void)
{
    this->_locations.clear();
    this->_cgis.clear();
    return ;
}

uint16_t Config::GetPort(void)
{
    return (this->_port);
}

std::vector<uint16_t> Config::GetPorts(void)
{
    return (this->_ports);
}

in_addr_t Config::GetHost(void)
{
    return (this->_host);
}

std::string Config::GetServerName(void)
{
    return (this->_server_name);
}

std::string Config::GetRoot(void)
{
    return (this->_root);
}

size_t Config::GetClientMaxBodySize(void)
{
    return (this->_client_max_body_size);
}

std::string Config::GetIndex(void)
{
    return (this->_index);
}

std::vector<Config> Config::GetLocations(void)
{
    return (this->_locations);
}

std::map<int, std::string> Config::GetErrorPages(void)
{
    return (this->_error_pages);
}

std::string Config::GetCgiPass(void)
{
    return (this->_cgi_pass);
}

std::string Config::GetCgiExtension(void)
{
    return (this->_cgi_extension);
}

std::string Config::GetRedirect(void)
{
    return (this->_redirect);
}

bool Config::GetAutoindex(void)
{
    return (this->_autoindex);
}

std::vector<bool> Config::GetAllowMethods(void)
{
    return (this->_allow_methods);
}

void Config::SetPort(std::string port)
{
    try
    {
        std::string port_str = _trim(port);
        int aux = Utils::StringToInt(port_str);
        if (aux < 0 || aux > 65535) //maybe 1024 to 49151 for server reserved ports
        {
            std::string error = "Invalid port: " + port_str;
            Utils::exceptWithError(error);
        }
        uint16_t port_int = Utils::StringToUint16(port);
        port_int = Utils::StringToUint16(port_str);
        this->_ports.push_back(port_int);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Config::SetHost(std::string host)
{
    try
    {
        std::string host_str = _trim(host);
        std::stringstream ss(host_str);
        std::string segment;
        std::vector<std::string> segvec;

        while (std::getline(ss, segment, '.'))
            segvec.push_back(segment);
        if (segvec.size() != 4)
        {
            std::string error = "Invalid host 2: " + host_str;
            Utils::exceptWithError(error.c_str());
        } 
        for (int i = 0; i < 4; i++)
        {
            if (Utils::StringToInt(segvec[i]) < 0 || Utils::StringToInt(segvec[i]) > 255)
            {
                std::string error = "Invalid host 1: " + host_str;
                Utils::exceptWithError(error);
            }
        }
        this->_host = inet_addr(host_str.c_str());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void Config::SetServerName(std::string server_name)
{
    this->_server_name = _trim(server_name);
}

void Config::SetRoot(std::string root)
{
    try
    {
        this->_root = _trim(root);
        if (Utils::DirIsValid(this->_root) == false)
        {
            std::string error = "Invalid root directory: " + this->_root;
            Utils::exceptWithError(error);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void Config::SetClientMaxBodySize(std::string client_max_body_size)
{
    this->_client_max_body_size = Utils::StringToSizeT(client_max_body_size);
}

void Config::SetIndex(std::string index)
{

    try
    {
        std::string indexPath = this->_root + "/" + _trim(index);
        if (Utils::FileIsReadable(indexPath) == false)
        {
            std::string error = "Invalid index file: " + indexPath;
            Utils::exceptWithError(error);
        }
        this->_index = _trim(index);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void Config::AddLocation(Config location)
{
    this->_locations.push_back(location);
}

void Config::AddErrorPage(std::string line)
{

    std::pair<int, std::string> error_page = _errorPageTrim(line);
    this->_error_pages.insert(error_page);
}

void Config::SetAutoindex(std::string autoindex)
{
    std::string autoindex_str = _trim(autoindex);
    if (autoindex_str == "on")
        this->_autoindex = true;
    else
        this->_autoindex = false;
}

void Config::SetAllowMethods(std::string allow_methods)
{
    std::string methods[3] = {"GET", "POST", "DELETE"};
    for (int i = 0; i < 3; i++)
    {
        if (allow_methods.find(methods[i]) != std::string::npos)
            this->_allow_methods[i] = 1;
    }
}

void Config::SetCgiPass(std::string cgi_pass)
{
    try
    {
        std::vector<std::string> tokens = Utils::Tokenize(cgi_pass, " \t;");
        if (tokens.size() != 2)
        {
            std::string error = "Invalid cgi directory: " + cgi_pass;
            Utils::exceptWithError(error);
        }
        for (size_t i = 1; i < tokens.size(); i++)
        {
            if (Utils::DirIsValid(tokens[i]) == false)
            {
                std::string error = "Invalid cgi directory: " + tokens[i];
                Utils::exceptWithError(error);
            }
            Cgi *cgi = new Cgi();
            cgi->SetCgiPath(tokens[i]);
            this->_cgis.push_back(*cgi);
            delete cgi;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void Config::SetCgiExtension(std::string cgi_extension)
{
    try
    {
        std::vector <std::string> tokens = Utils::Tokenize(cgi_extension, " \t;");
        if (tokens.size() != _cgis.size() + 1)
        {
            std::string error = "cgi_extension and cgi_pass mismatch";
            Utils::exceptWithError(error);
        }
        for (size_t i = 1; i < tokens.size(); i++)
        {
            this->_cgis[i - 1].SetCgiExtension(tokens[i]);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void Config::SetRedirect(std::string redirect)
{
    this->_redirect = _trim(redirect);
}

void Config::SetRootAsLocation(std::string location)
{
    try
    {
        this->_root = location;
        std::cout << "LOCATION ROOT: " << this->_root << std::endl;
        if (Utils::DirIsValid(this->_root) == false)
        {
            std::string error = "Invalid location directory: " + this->_root;
            Utils::exceptWithError(error);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

std::string Config::_trim(std::string str)
{
    size_t start = str.find_last_of(" \t") + 1;
    size_t end = str.find_last_not_of(";");
    std::string aux = str.substr(start, end - start + 1);
    return (aux.substr(0, aux.find_last_not_of(" \t") + 1));
}

std::pair<int, std::string> Config::_errorPageTrim(std::string str)
{
    size_t start = str.find_first_of("0123456789");
    size_t end = str.find_last_of(" \t");
    std::string error_code_str = str.substr(start, end - start + 1);
    std::string error_page = str.substr(end + 1, str.length() - end);
    return (std::make_pair(Utils::StringToInt(error_code_str), error_page));
}

void Config::PrintConfig(void)
{
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Host: " << inet_ntoa(*(in_addr*)&this->_host) << std::endl;
    std::cout << "Server Name: " << this->_server_name << std::endl;
    std::cout << "Root: " << this->_root << std::endl;
    std::cout << "Client Max Body Size: " << this->_client_max_body_size << std::endl;
    std::cout << "Index: " << this->_index << std::endl;
    /*std::cout << "Redirect: " << this->_redirect << std::endl;
    std::cout << "Cgi Pass: " << this->_cgi_pass << std::endl;
    std::cout << "Cgi Extension: " << this->_cgi_extension << std::endl;
    std::cout << "Autoindex: " << this->_autoindex << std::endl;
    std::cout << "GET: " << this->_allow_methods[0] << std::endl;
    std::cout << "POST: " << this->_allow_methods[1] << std::endl;
    std::cout << "DELETE: " << this->_allow_methods[2] << std::endl;

    std::map<int, std::string>::iterator it = this->_error_pages.begin();
    while (it != this->_error_pages.end())
    {
        std::cout << "Error Page: " << it->first << " " << it->second << std::endl;
        it++;
    }
    std::vector<uint16_t>::iterator it2 = this->_ports.begin();
    while (it2 != this->_ports.end())
    {
        std::cout << "PortV: " << *it2 << std::endl;
        it2++;
    }*/
    std::vector<Config>::iterator it3 = this->_locations.begin();
    while (it3 != this->_locations.end())
    {
        std::cout << "Location: " << std::endl;
        std::cout << "HHHHHHHHHHHHHHHHHHHHHHH" << std::endl;
        it3->PrintConfig();
        it3++;
    }
    std::cout << "+++++++++++++++++++++++++++++++++++" << std::endl;
    
}

void Config::ClearLocations(void)
{
    this->_locations.clear();
}