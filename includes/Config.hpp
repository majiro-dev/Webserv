/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:38:23 by manujime          #+#    #+#             */
/*   Updated: 2024/02/15 12:57:38 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "outputMacros.hpp"
# include "Utils.hpp"
# include "Location.hpp"


class Config
{
    public:
        Config(void);
        ~Config(void);

        bool        isValid;

        std::vector<uint16_t>  GetPorts(void);
        in_addr_t   GetHost(void);
        std::string GetServerName(void);
        std::string GetRoot(void);
        size_t      GetClientMaxBodySize(void);
        std::string GetIndex(void);
        
        std::vector<Location>         GetLocations(void);
        std::map<int, std::string>  GetErrorPages(void);

        void   SetPort(std::string port);
        void   SetHost(std::string host);
        void   SetServerName(std::string server_name);
        void   SetRoot(std::string root);
        void   SetClientMaxBodySize(std::string client_max_body_size);
        void   SetIndex(std::string index);

        void   AddLocation(Location location);
        void   AddErrorPage(std::string line);

        void  PrintConfig(void);

    private:
        std::vector<uint16_t>  _ports;
        in_addr_t    _host;
        std::string  _server_name;
        std::string  _root;
        size_t       _client_max_body_size;
        std::string  _index;
        
        std::vector<Location>         _locations;
        std::map<int, std::string>  _error_pages;

        std::string  _trim(std::string str);
        std::pair<int, std::string> _errorPageTrim(std::string str);
};

#endif