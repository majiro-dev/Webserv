/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:40:55 by manujime          #+#    #+#             */
/*   Updated: 2024/04/03 18:00:37 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Methods.hpp"

static bool isDir(const std::string &path)
{
    struct stat info;

    if(stat(path.c_str(), &info) != 0)
        return false;
    return S_ISDIR(info.st_mode);
}

Response Methods::HandleGet(std::string path, Config &location)
{
    std::cout << "PATH: " << path << std::endl;
    std::string body = "";
    std::string line;
    std::vector<Cgi> cgis = location.GetCgis();
    Response response;
    
    if (Cgi::IsCgi(path))
    {
        std::cout << "ENTRA1" << std::endl;
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
            return Response(500);
        if (cgi.ExecuteCgi(NULL, NULL))
        {
            response.setBody(cgi.GetResult());
            return response;
        }
        return Response(500);
    }
    if(isDir(path))
    {
        std::cout << "DIRECTORIO" << path[path.size() - 1] << std::endl;
        if(path[path.size()] == '/')
        {
            std::cout <<"Entra" << std::endl;
            path += location.GetIndex();
        }
        else
        {
            path += "/";
            path += location.GetIndex();
        }
        std::cout << "PATH: " << path << std::endl;
    }
    std::ifstream file(path.c_str());
    if (file.is_open())
    {
        
        std::cout << "ENTRA2" << std::endl;
        while (getline(file, line))
        {
            body += line + "\n";
        }
        file.close();
        response.setBody(body);
        return response;
    }
    else
    {
        std::cout << "ENTRA3" << std::endl;
        //response = "File not found";
        return Response(404);
    }
    return response;
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


