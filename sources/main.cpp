/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:11:11 by manujime          #+#    #+#             */
/*   Updated: 2024/04/16 17:01:31 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "../includes/outputMacros.hpp"
#include "../includes/Server.hpp"
#include "../includes/Config.hpp"
#include "../includes/Manager.hpp"
#include "../includes/Cluster.hpp"


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

    std::list<Config> configs = manager.getConfigs();
    Cluster cluster(configs); 
    signal(SIGINT, handleSignal);
    if(cluster.init() == true)
        cluster.run();
    
    return (0);
}

