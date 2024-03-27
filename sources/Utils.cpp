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


void Utils::log(const std::string &message, const char* color)
{
    std::cout << color << message << RESET << std::endl;
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

SError::SError(const std::string &msg)
    :_msg(msg)
{}


const char * SError::what() const throw()
{
    return _msg.c_str();
}

SError::~SError() throw() 
{}

void Utils::exceptWithError(const std::string &message)
{
    throw SError(message);
}

int Utils::StringToInt(std::string str)
{
    std::stringstream ss(str);
    int number;
    ss >> number;
    return number;
}

uint16_t Utils::StringToUint16(std::string str)
{
    std::stringstream ss(str);
    uint16_t number;
    ss >> number;
    return number;
}

size_t Utils::StringToSizeT(std::string str)
{
    std::stringstream ss(str);
    size_t size;
    ss >> size;
    return size;
}

void MyStrcpy(char *dst, const char *src)
{
    while (*src)
    {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
}

char **Utils::MultimapToCharMatrix(std::multimap<std::string, std::string> &map)
{
    char **env = new char*[map.size() + 1];
    int i = 0;
    for (std::multimap<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
    {
        std::string str = it->first + "=" + it->second;
        env[i] = new char[str.length() + 1];
        MyStrcpy(env[i], str.c_str());
        i++;
    }
    env[i] = NULL;
    return env;
}

bool Utils::DirIsValid(std::string path)
{
    struct stat info;
    if (stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
        return true;
    return false;
}

std::vector<std::string> Utils::Tokenize(const std::string &str, const std::string &delimiters)
{
    std::vector<std::string> tokens;
    size_t start = str.find_first_not_of(delimiters);
    size_t end = 0;
    while ((end = str.find_first_of(delimiters, start)) != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delimiters, end);
    }
    if (start != std::string::npos)
        tokens.push_back(str.substr(start));
    return tokens;
}

static std::string logDataTine()
{
    std::stringstream ss;

    std::time_t currentTime = std::time(nullptr);

    std::tm* localTime = std::localtime(&currentTime);

    ss << localTime->tm_year + 1900 << '-' << localTime->tm_mon + 1 << '-' << localTime->tm_mday << ' ';
    ss << localTime->tm_hour << ':' << localTime->tm_min << ':' << std::setw(2) << std::setfill('0') << localTime->tm_sec;
    return ss.str();
}

void Utils::logger(const std::string &msg, int mode)
{
    std::stringstream ss;

    const char *color;
    std::string text = "";
    if(mode == LOG)
    {
        color = GREEN;
        text = "LOG";
    }
    else if(mode == INFO)
    {
        color = CYAN;
        text = "INF";
    }else if(mode == ERROR)
    {
        color = RED;
        text = "ERR";
    }


    ss << '[' << logDataTine() << ']' << "  "<< '[' << color << text  << RESET << ']' << ":  " << color << msg << RESET;

    std::cout << ss.str() << std::endl;
}


void handleSignal(int signal) 
{
	(void)signal;

    std::cout << std::endl;
    Utils::logger("Webserv closed!", INFO);
	std::exit(0);
}

void leaks(void)
{
	system("leaks -q webserv");
}


std::string Utils::Uint16ToString(uint16_t number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}