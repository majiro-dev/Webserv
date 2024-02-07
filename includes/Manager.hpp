/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 12:15:31 by manujime          #+#    #+#             */
/*   Updated: 2024/02/07 21:07:15 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
# define MANAGER_HPP

# include "Config.hpp"
# include "Server.hpp"

class Manager
{
	public:
		Manager(std::string path);
		Manager();
		~Manager(void);
			
		bool	parseConfig();
		std::list<Config>	getConfigs(void) const;

	private:
		std::string			_path;
		std::list<Config>	_configs;
		std::list<Server>	_servers;

		void	_parseServerBlock(std::ifstream *file, std::string *line, Config *config);
		void	_assignConfValues(std::string *line, Config *config, int i);
};

#endif