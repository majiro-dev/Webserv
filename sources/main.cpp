/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:11:11 by manujime          #+#    #+#             */
/*   Updated: 2024/04/04 22:49:02 by cmorales         ###   ########.fr       */
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
    //atexit(leaks);
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
    
    //Server server = Server(config);
    
    return (0);
}

