/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:02:10 by manujime          #+#    #+#             */
/*   Updated: 2024/01/22 13:45:08 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config.hpp"

Config::Config(char *path)
{
    std::cout << path << std::endl;
    valid = parse(path);
    return ;
}

Config::~Config(void)
{
    return ;
}

bool   Config::parse(char *path)
{
    std::string pathStr(path);

    //if the file doesn't exist or is not readable return false
    std::ifstream   file(path);
    if (!file)
    {
        std::cout << ERROR_FILE_EXISTS;
        return (false);
    }
    //if the file is not a .conf file returns false
    std::string     ext = pathStr.substr(pathStr.find_last_of(".") + 1);
    if (ext != "conf")
    {
        std::cout << ERROR_FILE_EXT;
        return (false);
    }
    //if the file is empty returns false
    std::stringstream  buffer;
    buffer << file.rdbuf();
    std::string         contents(buffer.str());
    if (contents.empty())
    {
        std::cout << ERROR_FILE_EMPTY;
        return (false);
    }

    //if the file is not a valid .conf file returns false

    return (true);
}