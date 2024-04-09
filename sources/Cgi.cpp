/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:03:00 by manujime          #+#    #+#             */
/*   Updated: 2024/04/09 18:06:01 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cgi.hpp"
#include "../includes/Utils.hpp"

Cgi::Cgi(void)
{
    this->cgiPath = "";
    this->cgiExtension = "";
    this->result = "";
}

Cgi::~Cgi(void)
{
}

Cgi::Cgi(Cgi const & src)
{
    this->cgiPath = src.GetCgiPath();
    this->cgiExtension = src.GetCgiExtension();
    this->result = src.GetResult();
}

void    Cgi::SetCgiPath(std::string path)
{
    this->cgiPath = path;
}

void    Cgi::SetCgiExtension(std::string extension)
{
    this->cgiExtension = extension;
}

std::string Cgi::GetCgiPath(void) const
{
    return (this->cgiPath);
}

std::string Cgi::GetCgiExtension(void) const
{
    return (this->cgiExtension);
}

//statics

bool Cgi::IsCgi(std::string path)
{
    std::string valids[] = {".sh", ".py"};
    
    if (path.find_last_of('.') != std::string::npos)
    {
        std::string extension = path.substr(path.find_last_of('.'));
        for (unsigned int i = 0; i < sizeof(valids) / sizeof(valids[0]); i++)
        {
            if (extension == valids[i])
                return (true);
        }
    }
    return (false);
}

bool Cgi::ExecuteCgi(char **env, char **argv)
{
    pid_t pid;
    int fd[2];
    int status;

    std::cout << "EXECUTING CGI" << std::endl;

    if (pipe(fd) == -1)
        return (false);
    pid = fork();
    if (pid == 0)
    {
        if (chdir(this->cgiPath.c_str()) == -1)
        {
            //change directory of the child process for relative path file access
            std::cout << "could not change directory to " << this->cgiPath << std::endl;
            return (false);
        }
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        alarm(5);
        execve(argv[0], argv, env);
        alarm(0);
        exit(EXIT_FAILURE);
    }
    //else if (pid < 0)
     //   return (false);
    else
    {
        std::cout << "PARENT PROCESS" << std::endl;
        close(fd[1]);
        char buffer[4096];
        int bytesRead;
        while ((bytesRead = read(fd[0], buffer, 4096)) > 0)
        {
            this->result.append(buffer, bytesRead);
            std::cout <<"CGI RESULT ASSIGNED:";
            std::cout << this->result << std::endl;
        }
        close(fd[0]);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 0)
                return (true);
        }
    }
    return (false);
}

std::string Cgi::GetResult(void) const
{
    return (this->result);
} 