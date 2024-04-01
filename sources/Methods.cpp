/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:40:55 by manujime          #+#    #+#             */
/*   Updated: 2024/04/01 17:54:49 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Methods.hpp"

void Methods::HandleGet(std::string path, std::string &response, int &status, Config &config)
{
    std::string body = "";
    std::string line;
    std::ifstream file(path.c_str());

    if (Cgi::IsCgi(path))
    {
        Cgi cgi;
        for (std::vector<Cgi>::iterator it = config.GetCgis().begin(); it != config.GetCgis().end(); it++)
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

