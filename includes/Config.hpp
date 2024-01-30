/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:38:23 by manujime          #+#    #+#             */
/*   Updated: 2024/01/30 22:46:56 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "outputMacros.hpp"
# include "ConfigUtils.hpp"

# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include <list>
# include <map>
# include <netinet/in.h>


class Config
{
    public:
        Config(char *path);
        ~Config(void);

        bool    valid;

    private:
        bool    parse(char *path);
        
        uint16_t     _port;
        in_addr_t    _host;
        std::string  _server_name;
        std::string  _root;
        size_t       _client_max_body_size;
        std::string  _index;
        
        
        std::map<int, std::string>  _error_pages;
};

#endif