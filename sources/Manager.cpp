/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 19:27:57 by manujime          #+#    #+#             */
/*   Updated: 2024/02/06 20:04:17 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Manager.hpp"

Manager::Manager(std::string path)
{
	_path = path;
}

Manager::Manager(void)
{

}

Manager::~Manager(void)
{
	
}

bool	Manager::parseConfig()
{
	//Reads line by line the config file c
	//Creates a new Config object for each server
	//Sets the values of the Config object
	//Adds the new Config object to the _configs list
	int serverCount = _serverCount();
	Utils::log("Server count: " + std::to_string(serverCount));
	for (int i = 0; i < serverCount; i++)
	{
		//_configs.push_back(new Config());
	}

	return true;
}

int 	Manager::_serverCount()
{
	//Returns the number of server blocks in the _path file
	//a block is defined by the "server" keyword
	int count = 0;
	std::ifstream file(_path.c_str());
	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("server {") != std::string::npos)
		{
			while (std::getline(file, line))
			{
				if (line.find("server {") != std::string::npos)
				{
					Utils::log("Error: nested server blocks are not allowed");
				}
				if (line.find("}") != std::string::npos)
					count++;
			}
		}
	}
	return count;
}