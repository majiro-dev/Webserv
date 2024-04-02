/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:40:55 by manujime          #+#    #+#             */
/*   Updated: 2024/04/01 23:34:41 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Methods.hpp"

void Methods::HandleGet(std::string path, std::string &response, int &status, Config &config)
{
    std::string body = "";
    std::string line;
    std::ifstream file(path.c_str());
    std::vector<Cgi> cgis = config.GetCgis();
    if (Cgi::IsCgi(path))
    {
        Cgi cgi;
        for (std::vector<Cgi>::iterator it = cgis.begin(); it != cgis.end(); it++)
        {
            if (it->GetCgiExtension() == path.substr(path.find_last_of('.')))
            {
                cgi = *it;
                break;
            }
        }
        if (cgi.GetCgiPath().empty())
        {
            status = 500;
            return;
        }
        if (cgi.ExecuteCgi(NULL, NULL))
        {
            response = cgi.GetResult();
            status = 200;
            return;
        }
        status = 500;
        return;
    }
    if (file.is_open())
    {
        while (getline(file, line))
        {
            body += line + "\n";
        }
        file.close();
        response = body;
        status = 200;
    }
    else
    {
        response = "File not found";
        status = 404;
    }
}

void Methods::HandleDelete(std::string path, std::string &response, int &status)
{
    if (remove(path.c_str()) == 0)
    {
        response = "File deleted";
        status = 200;
    }
    else
    {
        response = "File not found";
        status = 404;
    }
}


