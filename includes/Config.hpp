/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:38:23 by manujime          #+#    #+#             */
/*   Updated: 2024/04/22 12:45:15 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "outputMacros.hpp"
# include "Utils.hpp"
# include "Socket.hpp"
# include "Cgi.hpp"


class Config
{
    public:
        Config(void);
        Config(const Config &src);
        ~Config(void);

        uint16_t                    GetPort(void);
        std::vector<uint16_t>       GetPorts(void);
        in_addr_t                   GetHost(void);
        std::string                 GetServerName(void);
        std::string                 GetRoot(void);
        size_t                      GetClientMaxBodySize(void);
        std::string                 GetIndex(void);
        std::string                 GetRedirect(void);
        std::vector<Cgi>            GetCgis(void);
        bool                        GetAutoindex(void);
        std::vector<bool>           GetAllowMethods(void);
        std::vector<Config>         GetLocations(void);
        std::map<int, std::string>  GetErrorPages(void);
        
        Config                      *GetParent(void);

        std::string                 GetLocationName(void);
        std::string                 GetProjectPath(void);

        void                        SetPort(std::string port);
        void                        SetHost(std::string host);
        void                        SetServerName(std::string server_name);
        void                        SetRoot(std::string root);
        void                        SetClientMaxBodySize(std::string client_max_body_size);
        void                        SetIndex(std::string index);
        void                        SetRedirect(std::string redirect);
        void                        AddErrorPage(std::string line);

        void                        AddLocation(Config location);
        
        void                        SetAutoindex(std::string autoindex);
        void                        SetAllowMethods(std::string allow_methods);
        void                        SetCgiPass(std::string cgi_pass);
        void                        SetCgiExtension(std::string cgi_extension);
        void                        SetRootAsLocation(std::string location);

        void                        PrintConfig(void);
        void                        ClearLocations(void);
        bool                        IsValid(void);
        void                        SetParent(Config *parent);

        void                        SetLocationName(std::string locationName);
        void                        SetProjectPath(std::string projectPath);

        bool valid;
    private:
        Config *                        _parent;
        std::vector<uint16_t>           _ports;
        uint16_t                        _port;
        in_addr_t                       _host;
        std::string                     _server_name;
        std::string                     _root;
        size_t                          _client_max_body_size;
        std::string                     _index;
        std::string                     _redirect;

        std::vector<Cgi>                _cgis;
        bool                            _autoindex;
        std::vector<bool>               _allow_methods;
        
        std::string                     _LocationName;
        std::string                     _projectPath;
        
        std::vector<Config>             _locations;
        std::map<int, std::string>      _error_pages;

        

        std::string                     _trim(std::string str);
        std::pair<int, std::string>     _errorPageTrim(std::string str);

        void                            _reParse(void);
};

#endif