/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 19:27:57 by manujime          #+#    #+#             */
/*   Updated: 2024/03/19 11:29:41 by manujime         ###   ########.fr       */
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
	for (std::list<Config>::iterator it = _configs.begin(); it != _configs.end(); it++)
	{
		_configs.erase(it);
	}
}

bool	Manager::parseConfig()
{
	try
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
				delete config;
			}
		}
		for (std::list<Config>::iterator it = _configs.begin(); it != _configs.end(); it++)
			it->PrintConfig();
		Utils::log("Parsed " + Utils::IntToString(_configs.size()) + " server blocks", RESET);
		return true;
	}
	catch (std::exception &e)
	{
		Utils::log(e.what(), RED);
		return false;
	}
}

std::string locationPath(std::string path)
{
	std::string location = path.substr(path.find("location ") + 9);
	location = location.substr(0, location.find(" "));
	return location;
}

void 	Manager::_parseServerBlock(std::ifstream *file, std::string *line, Config *config)
{
	std::string toFind[] = {"listen ", "host ", "server_name ", "root ", "client_max_body_size ",
							"autoIndex " , "error_page ", "index ", "allow_methods ", "cgi_pass ", "cgi_extension ",
							"return "};
	while (std::getline(*file, *line))
	{
		if (line->find("}") != std::string::npos)
			break;	
		for (unsigned int i = 0; i < sizeof(toFind) / sizeof(toFind[0]); i++)
		{
			if (line->find(toFind[i]) != std::string::npos)
				_assignConfValues(line, config, i);  
			else if (line->find("location ") != std::string::npos)
			{
				std::string locationLine = *line;
				Config *location = new Config(*config);
				location->ClearLocations();
				_parseLocationBlock(file, line, location);
				if (location->GetRoot() != config->GetRoot())
					location->SetRootAsLocation(location->GetRoot() + locationPath(locationLine));
				config->AddLocation(*location);
				delete location;
			}
		}
	}
}

void 	Manager::_assignConfValues(std::string *line, Config *config, int i)
{
	std::string value = *line;
	void (Config::*setters[])(std::string) = {&Config::SetPort, &Config::SetHost, &Config::SetServerName,
				&Config::SetRoot, &Config::SetClientMaxBodySize, &Config::SetAutoindex, &Config::AddErrorPage,
				&Config::SetIndex, &Config::SetAllowMethods, &Config::SetCgiPass, &Config::SetCgiExtension, 
				&Config::SetRedirect};
	(config->*setters[i])(value);
	//std::cout << "Assigned " << value << " to " << i << std::endl;
	//Utils::log("BREAKPOINTE TEST");
}

void  Manager::_parseLocationBlock(std::ifstream *file, std::string *line, Config *location)
{
	std::string toFind[] = {"listen ", "host ", "server_name ", "root ", "client_max_body_size ",
							"autoIndex " , "error_page ", "index ", "allow_methods ", "cgi_pass ", "cgi_extension ",
							"return "};
	
	std::string locationRoot = "";
	while (std::getline(*file, *line))
	{
		if (line->find("}") != std::string::npos)
			break;
		for (unsigned int i = 0; i < sizeof(toFind) / sizeof(toFind[0]); i++)
		{
			if (line->find(toFind[i]) != std::string::npos)
				_assignConfValues(line, location, i);
		}
	}
}

