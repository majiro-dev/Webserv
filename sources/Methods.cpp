/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:40:55 by manujime          #+#    #+#             */
/*   Updated: 2024/04/08 14:01:36 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Methods.hpp"

Response Methods::HandleGet(std::string path, Config &config)
{
    std::cout << "PATH: " << path << std::endl;
    
    std::string body = "";
    std::string line;
    std::ifstream file(path.c_str());
    std::vector<Cgi> cgis = config.GetCgis();
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
}

Response Methods::HandleDelete(std::string path, Config &config)
{
    Response response;

    std::ifstream file(path.c_str());
    if (!file.is_open())
        return Response(404);
    if (remove(path.c_str()) == 0)
        return response;
    return Response(500);

    (void)config;
}

Response Methods::HandlePost(std::string path, std::string requestText, Config &config)
{
    Response response;
    std::string filename;
    std::string extension;
    size_t startPos;
    size_t endPos;

    while (true)
    {
        startPos = requestText.find("filename=\"", endPos) + 10;
        endPos = requestText.find("\"", startPos);
        if (startPos == std::string::npos || endPos == std::string::npos)
            break;
        filename = requestText.substr(startPos, endPos - startPos);
        // std::cout << "FILENAME: " << filename << std::endl;
        extension = filename.substr(filename.find_last_of('.'));
        // std::cout << "EXTENSION: " << extension << std::endl;
        if (filename.empty() || extension.empty())
            return Response(400); //Bad Request
        startPos = requestText.find("\r\n\r\n", endPos) + 4;
        endPos = requestText.find("\r\n------", startPos);
        if (startPos == std::string::npos || endPos == std::string::npos)
            return Response(400);
        std::ofstream file(path + filename + extension);
        if (!file.is_open())
            return Response(500);
        file << requestText.substr(startPos, endPos);
        file.close();
        return response;
    }

}