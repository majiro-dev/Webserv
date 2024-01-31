/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 21:02:34 by manujime          #+#    #+#             */
/*   Updated: 2024/01/31 13:43:58 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGUTILS_HPP
# define CONFIGUTILS_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include <list>
# include <map>
# include <netinet/in.h>

# include "outputMacros.hpp"

enum AllowMethods
{
	GET,
	POST,
	DELETE,
};

class ConfigUtils
{
	public:
	static bool FileIsReadable(std::string path);
	static bool FileIsEmpty(std::string path);
	static bool FileIsConf(std::string path);
	static bool FileIsValid(std::string path);

};

#endif
