/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:02:10 by manujime          #+#    #+#             */
/*   Updated: 2024/02/07 21:27:15 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config.hpp"

Config::Config(void)
{
    //this->_port = 8080;
    //this->_host = inet_addr("127.0.0.1");
    return ;
}

Config::~Config(void)
{
    return ;
}

uint16_t Config::GetPort(void)
{
    return (this->_port);
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

std::list<Location> Config::GetLocations(void)
{
    return (this->_locations);
}

std::map<int, std::string> Config::GetErrorPages(void)
{
    return (this->_error_pages);
}

void Config::SetPort(std::string port)
{
    std::string port_str = _trim(port);
    this->_port = Utils::StringToUint16(port_str);
}

void Config::SetHost(std::string host)
{
    std::string host_str = _trim(host);
    this->_host = inet_addr(host_str.c_str());
}

void Config::SetServerName(std::string server_name)
{
    this->_server_name = _trim(server_name);
}

void Config::SetRoot(std::string root)
{
    this->_root = _trim(root);
}

void Config::SetClientMaxBodySize(std::string client_max_body_size)
{
    this->_client_max_body_size = Utils::StringToSizeT(client_max_body_size);
}

void Config::SetIndex(std::string index)
{
    this->_index = _trim(index);
}

void Config::AddLocation(Location location)
{
    this->_locations.push_back(location);
}

void Config::AddErrorPage(int error_code, std::string error_page)
{
    this->_error_pages[error_code] = error_page;
}

std::string Config::_trim(std::string str)
{
    size_t start = str.find_last_of(" \t") + 1;
    size_t end = str.find_last_not_of(";");
    std::string aux = str.substr(start, end - start + 1);
    return (aux.substr(0, aux.find_last_not_of(" \t") + 1));
}


void Config::PrintConfig(void)
{
    std::cout << "Port: " << this->_port << std::endl;
    std::cout << "Host: " << inet_ntoa(*(in_addr*)&this->_host) << std::endl;
    std::cout << "Server Name: " << this->_server_name << std::endl;
    std::cout << "Root: " << this->_root << std::endl;
    std::cout << "Client Max Body Size: " << this->_client_max_body_size << std::endl;
    std::cout << "Index: " << this->_index << std::endl;
}