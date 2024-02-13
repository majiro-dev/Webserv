/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 12:35:49 by manujime          #+#    #+#             */
/*   Updated: 2024/02/01 18:45:17 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

bool Utils::FileIsReadable(std::string path)
{
	std::ifstream   file(path.c_str());
    if (!file)
    {
        std::cout << ERROR_FILE_READ;
        return (false);
    }
	return true;
}

bool Utils::FileIsConf(std::string path)
{
    std::string     ext = path.substr(path.find_last_of(".") + 1);
    if (ext != "conf")
    {
        std::cout << ERROR_FILE_EXTENSION;
        return (false);
    }
	return true;
}

bool Utils::FileIsEmpty(std::string path)
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

bool Utils::FileIsValid(std::string path)
{
	if (!FileIsReadable(path))
		return false;
	if (!FileIsConf(path))
		return false;
	if (FileIsEmpty(path))
		return false;
	return true;
}


void Utils::log(const std::string &message)
{
    std::cout << message << std::endl;
}


std::string Utils::IntToString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

MyError::MyError(const char *msg)
    :_msg(msg)
{}

const char * MyError::what() const throw()
{
    return _msg;
}

void Utils::exceptWithError(const char* message)
{
    throw MyError(message);
}