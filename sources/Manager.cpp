/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 19:27:57 by manujime          #+#    #+#             */
/*   Updated: 2024/02/07 15:01:38 by manujime         ###   ########.fr       */
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
	std::ifstream file(_path.c_str());
	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("server {") != std::string::npos)
		{
			Config *config = new Config();
			_parseServerBlock(&file, &line, config);
			_configs.push_back(*config);
		}
	}
	Utils::log("Parsed " + Utils::IntToString(_configs.size()) + " server blocks");
	return true;
}

void 	Manager::_parseServerBlock(std::ifstream *file, std::string *line, Config *config)
{
	std::string toFind[] = {"listen ", "host" };
	while (std::getline(*file, *line))
	{
		if (line->find("}") != std::string::npos)
		{
			break;
		}		
		for (int i = 0; i < 5; i++)
		{
			if (line->find(toFind[i]) != std::string::npos)
			{
				std::string value = line->substr(toFind[i].length());
				switch (i)
				{
					case 0:
						config->SetPort(value);
						break;
					case 1:
						config->SetHost(value);
						break;
				}
			}
		}
	}
	Utils::log("Parsed server block");
	Utils::log("Port: " + Utils::IntToString(config->GetPort()));
	Utils::log("Host: " + Utils::IntToString(config->GetHost()));
} 