/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:38:23 by manujime          #+#    #+#             */
/*   Updated: 2024/02/20 17:51:48 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "outputMacros.hpp"
# include "Utils.hpp"
# include "Socket.hpp"
# include "Location.hpp"


class Config
{
    public:
        Config(void);
        ~Config(void);

        bool        isValid;

        uint16_t    GetPort(void);
        in_addr_t   GetHost(void);
        std::string GetServerName(void);
        std::string GetRoot(void);
        size_t      GetClientMaxBodySize(void);
        std::string GetIndex(void);
        
        std::list<Location>         GetLocations(void);
        std::map<int, std::string>  GetErrorPages(void);

        void   SetPort(uint16_t port);
        void   SetHost(in_addr_t host);
        void   SetServerName(std::string server_name);
        void   SetRoot(std::string root);
        void   SetClientMaxBodySize(size_t client_max_body_size);
        void   SetIndex(std::string index);

        void   AddLocation(Location location);
        void   AddErrorPage(int error_code, std::string error_page);

    private:
        uint16_t     _port;
        in_addr_t    _host;
        std::string  _server_name;
        std::string  _root;
        size_t       _client_max_body_size;
        std::string  _index;
        
        std::list<Location>         _locations;
        std::map<int, std::string>  _error_pages;
};

#endif