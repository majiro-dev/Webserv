/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:11:11 by manujime          #+#    #+#             */
/*   Updated: 2024/02/27 18:41:54 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "../includes/outputMacros.hpp"
#include "../includes/Server.hpp"
#include "../includes/Config.hpp"
#include "../includes/Manager.hpp"

int main(int argc, char **argv)
{
    std::string path;
    if (argc != 2)
    {
        std::cout << ERROR_ARGC;
        return (1);
    }
    path = std::string(argv[1]);
    if (!Utils::FileIsValid(path))
        return (1);
    Config  config = Config();
    Manager manager = Manager(std::string(argv[1]));
    if (!manager.parseConfig())
        return (1);
    //Server server = Server("0.0.0.0", 8080);
    Server server = Server(config);
    server.runServer();
    Utils::log("Server is running", RESET);
    return (0);
}

