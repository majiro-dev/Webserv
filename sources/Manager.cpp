/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 19:27:57 by manujime          #+#    #+#             */
/*   Updated: 2024/02/07 21:07:13 by manujime         ###   ########.fr       */
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
	std::string toFind[] = {"listen ", "host ", "server_name ", "root ", "client_max_body_size ", "index " };
	while (std::getline(*file, *line))
	{
		if (line->find("}") != std::string::npos)
			break;	
		for (unsigned int i = 0; i < toFind->length(); i++)
		{
			if (line->find(toFind[i]) != std::string::npos)
			{
				_assignConfValues(line, config, i);
			}
		}
	}
	config->PrintConfig();
}

void 	Manager::_assignConfValues(std::string *line, Config *config, int i)
{
	std::string value = *line;
	switch (i)
	{
		case 0:
			config->SetPort(value);
			break;
		case 1:
			config->SetHost(value);
			break;
		case 2:
			config->SetServerName(value);
			break;
		case 3:
			config->SetRoot(value);
			break;
		case 4:
			config->SetClientMaxBodySize(value);
			break;
		case 5:
			config->SetIndex(value);
			break;
		default:
			break;
	}
}