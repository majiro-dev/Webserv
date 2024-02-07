/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:02:10 by manujime          #+#    #+#             */
/*   Updated: 2024/02/07 15:11:35 by manujime         ###   ########.fr       */
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
    std::string port_str = port.substr(port.find_last_of(" ") + 1, port.length() - port.find_last_of(";") - 1);
    this->_port = Utils::StringToUint16(port_str);
}

void Config::SetHost(std::string host)
{
    std::string host_str = host.substr(host.find_last_of(" ") + 1, host.length() - host.find_last_of(";") - 1);
    this->_host = inet_addr(host_str.c_str());
}

void Config::SetServerName(std::string server_name)
{
    this->_server_name = server_name;
}

void Config::SetRoot(std::string root)
{
    this->_root = root;
}

void Config::SetClientMaxBodySize(std::string client_max_body_size)
{
    this->_client_max_body_size = Utils::StringToSizeT(client_max_body_size);
}

void Config::SetIndex(std::string index)
{
    this->_index = index;
}

void Config::AddLocation(Location location)
{
    this->_locations.push_back(location);
}

void Config::AddErrorPage(int error_code, std::string error_page)
{
    this->_error_pages[error_code] = error_page;
}

