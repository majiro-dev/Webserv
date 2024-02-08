/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:11:11 by manujime          #+#    #+#             */
/*   Updated: 2024/02/08 16:03:30 by manujime         ###   ########.fr       */
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
    Manager manager = Manager(std::string(argv[1]));
    if (!manager.parseConfig())
        return (1);
    Utils::log("init server");
    Server server = Server("0.0.0.0", 8080);
    server.startListen();
    Utils::log("Server is running");
    return (0);
}

