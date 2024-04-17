/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:02:10 by manujime          #+#    #+#             */
/*   Updated: 2024/04/17 15:25:00 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config.hpp"

Config::Config(void)
{
    this->_parent = NULL;
    this->_root = "";
    this->_index = "";
    this->_client_max_body_size = 0;
    this->_autoindex = false;
    this->_redirect = "";
    this-> _allow_methods.resize(3);
    for (int i = 0; i < 3; i++)
        this->_allow_methods[i] = 0;
    this->_projectPath = "";
    return ;
}

Config::Config(const Config &src)
{
    this->_parent = NULL;
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
    this->_error_pages = src._error_pages;
    this->_LocationName = src._LocationName;
    for (std::vector<Cgi>::const_iterator it = src._cgis.begin(); it != src._cgis.end(); it++)
    {
        Cgi *cgi = new Cgi();
        cgi->SetCgiPath(it->GetCgiPath());
        cgi->SetCgiExtension(it->GetCgiExtension());
        this->_cgis.push_back(*cgi);
        delete cgi;
    }
    this->_redirect = src._redirect;
    this->_projectPath = src._projectPath;
    return ;
}

Config::~Config(void)
{
    /*this->_locations.clear();
    this->_cgis.clear();
    this->_error_pages.clear();*/
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

std::vector<Cgi> Config::GetCgis(void)
{
    return (this->_cgis);
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
        uint16_t port_int = Utils::StringToUint16(port_str);
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
    this->_client_max_body_size = Utils::StringToSizeT(_trim(client_max_body_size));
}

void Config::SetIndex(std::string index)
{

    this->_index = _trim(index);
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
    for (int i = 0; i < 3; i++)
        this->_allow_methods[i] = 0;
    std::string methods[3] = {"GET", "POST", "DELETE"};
    for (int i = 0; i < 3; i++)
    {
        if (allow_methods.find(methods[i]) != std::string::npos)
            this->_allow_methods[i] = 1;
    }
}

void Config::SetCgiPass(std::string cgi_pass)
{
    std::vector<std::string> tokens = Utils::Tokenize(cgi_pass, " \t;");
    if (tokens.size() < 2)
    {
        std::string error = "Invalid cgi directory: " + cgi_pass;
        Utils::log(error, RED);
    }
    for (size_t i = 1; i < tokens.size(); i++)
    {
        Cgi *cgi = new Cgi();
        cgi->SetCgiPath(tokens[i]);
        this->_cgis.push_back(*cgi);
        delete cgi;
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
    std::cout << "Redirect: " << this->_redirect << std::endl;
    std::cout << "Autoindex: " << this->_autoindex << std::endl;
    std::cout << "GET: " << this->_allow_methods[0] << std::endl;
    std::cout << "POST: " << this->_allow_methods[1] << std::endl;
    std::cout << "DELETE: " << this->_allow_methods[2] << std::endl;
    std::cout << "LOCATION NAME: " << this->_LocationName << std::endl;

    std::vector<Cgi>::iterator it4 = this->_cgis.begin();
    while (it4 != this->_cgis.end())
    {
        std::cout << "Cgi: " << it4->GetCgiPath() << std::endl;
        std::cout << "Cgi Extension: " << it4->GetCgiExtension() << std::endl;
        it4++;
    }

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
    }
    std::vector<Config>::iterator it3 = this->_locations.begin();
    while (it3 != this->_locations.end())
    {
        std::cout << "Location: " << std::endl;
        it3->PrintConfig();
        it3++;
    }
    std::cout << "+++++++++++++++++++++++++++++++++++" << std::endl;
    
}

void Config::ClearLocations(void)
{
    this->_locations.clear();
}

bool Config::IsValid(void)
{
    if (this->_ports.size() == 0)
    {
        std::string error = "No port specified";
        Utils::log(error, RED);
        return (false);
    }

    if (this->_server_name == "")
    {
        std::string error = "No server name specified";
        Utils::log(error, RED);
        return (false);
    }
    
    if (this->_host == INADDR_NONE)
    {
        std::string error = "Invalid host: " + std::string(inet_ntoa(*(in_addr*)&this->_host));
        Utils::log(error, RED);
        return (false);
    }
    
    if (this->_index != "")
    {
        if (Utils::FileIsReadable(this->_root + "/" + this->_index) == false)
        {
            std::string error = "Invalid index file: " + this->_root + "/" + this->_index;
            Utils::log(error, RED);
            return (false);
        }
    }
    if (this->_projectPath == "" || !Utils::DirIsValid(this->_projectPath))
    {
        std::string error = "Invalid project path: " + this->_projectPath;
        Utils::log(error, RED);
        return (false);
    }
    if (this->_ports.size() != 0)
    {
        for (std::vector<uint16_t>::iterator it = this->_ports.begin(); it != this->_ports.end(); it++)
        {
            if (*it < 1024 || *it > 49151)
            {
                std::string error = "Invalid port: " + Utils::Uint16ToString(*it);
                Utils::log(error, RED);
                return (false);
            }
        }
    }
    if (this->_host == INADDR_NONE)
    {
        std::string error = "Invalid host: " + std::string(inet_ntoa(*(in_addr*)&this->_host));
        Utils::log(error, RED);
        return (false);
    }
    if (this->_root != "")
    {
        if (Utils::DirIsValid(this->_root) == false)
        {
            std::string error = "Invalid root directory: " + this->_root;
            Utils::log(error, RED);
            return (false);
        }
    }
    if (this->_index != "")
    {
        if (Utils::FileIsReadable(this->_root + "/" + this->_index) == false)
        {
            std::string error = "Invalid index file: " + this->_root + "/" + this->_index;
            Utils::log(error, RED);
            return (false);
        }
    }

    if (this->_cgis.size() != 0)
    {
        for (std::vector<Cgi>::iterator it = this->_cgis.begin(); it != this->_cgis.end(); it++)
        {
            if (Utils::FileIsReadable(it->GetCgiPath()) == false)
            {
                std::string error = "Invalid cgi path" + it->GetCgiPath();
                Utils::log(error, RED);
                return (false);
            }
        } 
    }
    
    if (this->_locations.size() != 0)
    {
        int base = 0;
        for (std::vector<Config>::iterator it = this->_locations.begin(); it != this->_locations.end(); it++)
        {
            if (it->IsValid() == false)
                return (false);
            if (it->GetLocationName() == "/")
                base++;
        }
        if (base != 1)
        {
            std::string error = "No base location found";
            Utils::log(error, RED);
            return (false);
        }
         for (std::vector<Config>::iterator it = this->_locations.begin(); it != this->_locations.end(); it++)
         {
            int instances = 0;
            std::string aux = it->GetLocationName();
            for (std::vector<Config>::iterator it2 = this->_locations.begin(); it2 != this->_locations.end(); it2++)
            {
                if (it2->GetLocationName() == aux)
                    instances++;
            }
            if (instances > 1)
            {
                std::string error = "Location " + aux + " has more than one instance";
                Utils::log(error, RED);
                return (false);
            }
         }
    }
    

    return (true);
}

void Config::_reParse(void)
{
}

void Config::SetParent(Config *parent)
{
    this->_parent = parent;
}

Config *Config::GetParent(void)
{
    return (this->_parent);
}

void Config::SetLocationName(std::string locationName)
{
    this->_LocationName = locationName;
}

std::string Config::GetLocationName(void)
{
    return (this->_LocationName);
}

void Config::SetProjectPath(std::string projectPath)
{
    this->_projectPath = _trim(projectPath);
}

std::string Config::GetProjectPath(void)
{
    return (this->_projectPath);
}