/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 12:35:49 by manujime          #+#    #+#             */
/*   Updated: 2024/01/31 13:45:32 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigUtils.hpp"

bool ConfigUtils::FileIsReadable(std::string path)
{
    std::ifstream   file(path);
	if (!file.good())
	{
		std::cout << ERROR_FILE_READ;
		return (false);
	}
	return (file.good());
}

bool ConfigUtils::FileIsConf(std::string path)
{
	std::string ext = path.substr(path.find_last_of(".") + 1);
	if (ext != "conf")
	{
		std::cout << ERROR_FILE_EXTENSION;
		return (false);
	}
	return (true);
}

bool ConfigUtils::FileIsEmpty(std::string path)
{
	std::ifstream   file(path);
	std::stringstream  buffer;
    buffer << file.rdbuf();
    std::string         contents(buffer.str());
    if (contents.size() == 0)
    {
        std::cout << ERROR_FILE_EMPTY;
        return (true);
    }
	return (false);
}

bool ConfigUtils::FileIsValid(std::string path)
{
	if (!FileIsReadable(path))
		return (false);
	if (!FileIsConf(path))
		return (false);
	if (FileIsEmpty(path))
		return (false);
	return (true);
}