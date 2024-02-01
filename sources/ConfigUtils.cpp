/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 12:35:49 by manujime          #+#    #+#             */
/*   Updated: 2024/02/01 18:43:17 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigUtils.hpp"

bool ConfigUtils::FileIsReadable(std::string path)
{
	std::ifstream   file(path.c_str());
    if (!file)
    {
        std::cout << ERROR_FILE_READ;
        return (false);
    }
	return true;
}

bool ConfigUtils::FileIsConf(std::string path)
{
    std::string     ext = path.substr(path.find_last_of(".") + 1);
    if (ext != "conf")
    {
        std::cout << ERROR_FILE_EXTENSION;
        return (false);
    }
	return true;
}

bool ConfigUtils::FileIsEmpty(std::string path)
{
	std::ifstream   file(path.c_str());
    std::stringstream  buffer;
    buffer << file.rdbuf();
    std::string         contents(buffer.str());
    if (contents.empty())
    {
        std::cout << ERROR_FILE_EMPTY;
        return (false);
    }
	return false;
}

bool ConfigUtils::FileIsValid(std::string path)
{
	if (!FileIsReadable(path))
		return false;
	if (!FileIsConf(path))
		return false;
	if (FileIsEmpty(path))
		return false;
	return true;
}

/*
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
}*/