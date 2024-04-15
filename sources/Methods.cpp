/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:40:55 by manujime          #+#    #+#             */
/*   Updated: 2024/04/15 18:06:23 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Methods.hpp"
#include "../includes/Response.hpp"



char **makeArgs (std::string exepath, std::string filepath, std::multimap<std::string, std::string> querys)
{
    char **args = new char*[querys.size() + 3];
    args[0] = strdup(exepath.c_str());
    args[1] = strdup(filepath.c_str());
    int i = 2;
    for (std::multimap<std::string, std::string>::iterator it = querys.begin(); it != querys.end(); it++)
    {
        args[i] = strdup((it->second).c_str());
        i++;
    }
    args[i] = NULL;
    return args;
}

Response Methods::HandleGet(std::string &path, Config &location, Request &req)
{
    std::string body = "";
    std::string line;
    std::vector<Cgi> cgis = location.GetCgis();
    Response response;
    path = Utils::slashCleaner(path);
    //std::cout << "RRPATH: " << path << std::endl;
    if (Cgi::IsCgi(path) && cgis.size() > 0)
    {
        //std::cout << "ENTRA1" << std::endl;
        Cgi cgi;
        std::cout << "CGIS SIZE: " << cgis.size() << std::endl;
        for (std::vector<Cgi>::iterator it = cgis.begin(); it != cgis.end(); it++)
        {
            std::cout << "EXTENSION: " << it->GetCgiExtension() << std::endl;
            std::cout << "PATH: " << it->GetCgiPath() << std::endl;
            if (it->GetCgiExtension() == path.substr(path.find_last_of('.')))
            {
                cgi = *it;
                break;
            }
        }
        if (cgi.GetCgiPath().empty())
        {
            std::cout << "NO CGI PATH" << std::endl;
            return Response(500);
        }
        char **args = makeArgs(Utils::slashCleaner(cgi.GetCgiPath()), Utils::slashCleaner(path), req.getQuerys());
        for (int i = 0; args[i] != NULL; i++)
        {
            std::cout << "ARGS: " << args[i] << std::endl;
        }
        if (cgi.ExecuteCgi(NULL, args, location.GetProjectPath()))
        {
            std::cout << "CGI RESULT: " << cgi.GetResult() << std::endl;
            response.setBody(cgi.GetResult());
            return response;
        }
        std::cout << "CGI ERROR" << std::endl; 
        return Response(500);
    }
    if(Utils::DirIsValid(path))
    {
        
        //std::cout << "DIRECTORIO" << path[path.size() - 1] << std::endl;
        if(location.GetAutoindex() == true)
        {
            body = AutoIndex::GetAutoIndex(path, location.GetLocationName(), req.getResource());
            response.setBody(body);
            return response;
        }
        if(path[path.size() - 1] == '/')
        {
            path += location.GetIndex();
        }
        else
        {
            path += "/";
            std::cout << location.GetIndex() << std::endl;
            std::cout << location.GetLocationName() << std::endl;
            path += location.GetIndex();
        }
    }
    path = Utils::slashCleaner(path);
    std::ifstream file(path.c_str(), std::ios::binary);
    if (file.is_open())
    {
        
        //std::cout << "ENTRA2" << std::endl;
        std::ostringstream ss;
        ss << file.rdbuf();
        file.close();
        body = ss.str();
        response.setBody(body);
    }
    else
    {
        //std::cout << "ENTRA3" << std::endl;
        //response = "File not found";
        return Response(404);
    }
    return response;
}

Response Methods::HandleDelete(std::string path)
{
    Response response;

    std::ifstream file(path.c_str());
    if (!file.is_open())
        return Response(404);
    if (remove(path.c_str()) == 0)
        return response;
    return Response(500);

}

std::string GetExtension(std::string contentType) {
    if (contentType == "application/json")
        return ".json";
    else if (contentType == "text/plain")
        return ".txt";
    else if (contentType == "text/css")
        return ".css";
    else if (contentType == "text/html")
        return ".html";
    else if (contentType == "text/xml")
        return ".xml";
    else if (contentType == "text/sh")
        return ".sh";
    else if (contentType == "text/py")
        return ".py";
    return "";
}

Response CreateFile(std::string path, Request request) 
{
    std::string contentType = request.getHeader("Content-Type");
    std::string extension = GetExtension(contentType);
    
    if (extension.empty()) {
        return Response(400);
    }
    
    std::ofstream file(path);
    
    if (!file.is_open()) {
        return Response(500);
    }

    try {
        std::string requestBody = request.getBody();
        file << requestBody;
        file.close();
        return Response(200);
    } catch (const std::exception &e) {
        std::cerr << "Error writing to file: " << e.what() << std::endl;
        return Response(500);
    }
}

Response Methods::HandlePost(std::string path, Request requestText, Config &location) {
    std::vector<Cgi> cgis = location.GetCgis();
    Response response;
    std::string contentType = requestText.getHeader("Content-Type");
    std::cout << "Content-Type: " << contentType << std::endl;
    if (contentType.empty() || requestText.getHeader("Content-Length").empty()) {
        std::cerr << "Error: Missing Content-Type or Content-Length header" << std::endl;
        return Response(400);
    }

    std::string extension = GetExtension(contentType);
    if (extension.empty()) {
        std::cerr << "Error: Unsupported Content-Type" << std::endl;
        return Response(400);
    }

    if (access(path.c_str(), F_OK) == -1) {
        std::ofstream createFile(path);
        if (!createFile.is_open()) {
            std::cerr << "Error: Failed to create file" << std::endl;
            return Response(500);
        }
        createFile.close();
    }

    CreateFile(path, requestText);

    path = Utils::slashCleaner(path);
    if (Cgi::IsCgi(path)) {
        Cgi cgi;
        std::cout << "CGIS SIZE: " << cgis.size() << std::endl;
        for (std::vector<Cgi>::iterator it = cgis.begin(); it != cgis.end(); it++)
        {
            std::cout << "EXTENSION: " << it->GetCgiExtension() << std::endl;
            std::cout << "PATH: " << it->GetCgiPath() << std::endl;
            if (it->GetCgiExtension() == path.substr(path.find_last_of('.')))
            {
                cgi = *it;
                break;
            }
        }
        if (cgi.GetCgiPath().empty())
        {
            std::cout << "NO CGI PATH" << std::endl;
            return Response(500);
        }
        char **args = makeArgs(Utils::slashCleaner(cgi.GetCgiPath()), Utils::slashCleaner(path), requestText.getQuerys());
        std::cout << "ARGS: " << args[0] << " " << args[1] << std::endl;
        if (cgi.ExecuteCgi(NULL, args, location.GetProjectPath()))
        {
            std::cout << "CGI RESULT: " << cgi.GetResult() << std::endl;
            response.setBody(cgi.GetResult());
            return response;
        }
        std::cout << "CGI ERROR" << std::endl; 
        return Response(500);
    }

    return CreateFile(path, requestText);
}
