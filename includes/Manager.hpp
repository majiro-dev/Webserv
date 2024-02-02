/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 12:15:31 by manujime          #+#    #+#             */
/*   Updated: 2024/01/31 12:23:15 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
# define MANAGER_HPP

# include "Config.hpp"
# include "Server.hpp"

class Manager
{
	public:
		Manager(void);
		~Manager(void);
		

	private:
		std::list<Config>	_configs;
		std::list<Server>	_servers;
};

#endif