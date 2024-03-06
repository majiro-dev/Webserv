/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:03:00 by manujime          #+#    #+#             */
/*   Updated: 2024/03/06 17:54:23 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cgi.hpp"
#include "../includes/Utils.hpp"

Cgi::Cgi(void)
{
    this->cgiPath = "";
    this->cgiExtension = "";
}

Cgi::~Cgi(void)
{
}

void    Cgi::SetCgiPath(std::string path)
{
    this->cgiPath = path;
}

void    Cgi::SetCgiExtension(std::string extension)
{
    this->cgiExtension = extension;
}

std::string Cgi::GetCgiPath(void)
{
    return (this->cgiPath);
}

std::string Cgi::GetCgiExtension(void)
{
    return (this->cgiExtension);
}

//statics

bool Cgi::IsCgi(std::string path)
{
    if (path.find(".php") != std::string::npos)
        return (true);
    if (path.find(".py") != std::string::npos)
        return (true);
    if (path.find(".pl") != std::string::npos)
        return (true);
    if (path.find(".rb") != std::string::npos)
        return (true);
    if (path.find(".sh") != std::string::npos)
        return (true);
    return (false);
}

bool ExecuteCgi(char **env, char **argv, std::string &response, int socket)
{
    pid_t pid;
    int fd[2];
    int status;

    if (pipe(fd) == -1)
    {
        response = "500 Internal Server Error";
        return (false);
    }
    pid = fork();
    if (pid == 0)
    {
        //chdir("path"); //change directory of the child process for relative path file access

        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        alarm(5);
        execve(argv[0], argv, env);
        alarm(0);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        response = "500 Internal Server Error";
        return (false);
    }
    else
    {
        close(fd[1]);
        char buffer[4096];
        int bytesRead;
        while ((bytesRead = read(fd[0], buffer, 4096)) > 0)
        {
            response.append(buffer, bytesRead);
        }
        close(fd[0]);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 0)
                return (true);
        }
    }
    response = "500 Internal Server Error";
    return (false);
}