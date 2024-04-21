/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 17:29:40 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/21 17:46:02 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include "Utils.hpp"
# include "Config.hpp"
# include "Manager.hpp"
# include "Server.hpp"

class Cluster
{
    private:
        std::vector<Config>_configs;
        std::vector<Server*>_servers;
        std::vector<pollfd>_pollfds;
        
        void removeClient(Client *client);
    public:
        Cluster(std::list<Config> &configs);
        ~Cluster();

        int    addSocketsPoll(struct pollfd *fds);
        int     acceptConnection(unsigned int sock, int idxServer, sockaddr_in servSockAdrr);
        void    checkServerSockets(struct pollfd *fds);
        void    checkClientSockets(struct pollfd *fds);
        bool    init();
        void    run();
};

# endif