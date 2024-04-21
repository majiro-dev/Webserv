/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:03:00 by manujime          #+#    #+#             */
/*   Updated: 2024/04/22 00:05:04 by manujime         ###   ########.fr       */
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

bool Cgi::IsCgi(std::string path)
{
    std::string valids[] = {".sh", ".py", ".php"};
    
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

static std::string relativePath(std::string projectPath, std::string exePath)
{
    std::string aux;
    aux = exePath.substr(0, exePath.find_last_of('/'));
    return (Utils::slashCleaner(projectPath + "/" + aux));
}

static std::string fixArgv1(std::string path)
{
    std::string fixedPath = path.substr(path.find_last_of('/') + 1);
    return (fixedPath);
}

bool Cgi::ExecuteCgi(char **env, char **argv, std::string projectPath)
{
    (void)env;
    (void)projectPath;
    pid_t pid;
    int fd[2];
    int status;
    std::string dirPath = this->cgiPath.substr(0, this->cgiPath.find_last_of('/'));
    
    if (pipe(fd) == -1)
        return (false);
    pid = fork();
    if (pid == 0)
    {
        if (chdir(relativePath(projectPath, argv[1]).c_str()) == -1)
        {
            Utils::logger("could not change directory to " + relativePath(projectPath, argv[1]), ERROR);
            return (false);
        }   

        std::string aux = fixArgv1(argv[1]);
        free(argv[1]);
        argv[1] = strdup(aux.c_str());

        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        alarm(5);


        if (execve(argv[0], argv, NULL) == -1)
        {
            Utils::logger("could not execute " + this->cgiPath, ERROR);
            return (false);
        }
        alarm(5);
        exit(EXIT_FAILURE);
    }
    else
    {
        close(fd[1]);
        fd[1] = -1;
        char buffer[4096];
        int bytesRead;

        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            if (!WEXITSTATUS(status))
            {
                while ((bytesRead = read(fd[0], buffer, 4096)) > 0)
                {
                    this->result.append(buffer, bytesRead);
                }
                close(fd[0]);
                return (true);
            }
        }
    }
    return (false);
}

std::string Cgi::GetResult(void) const
{
    return (this->result);
}
