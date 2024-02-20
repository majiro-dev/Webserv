/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 21:02:34 by manujime          #+#    #+#             */
/*   Updated: 2024/02/20 17:54:10 by cmorales         ###   ########.fr       */
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
# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <string>
# include <iostream>
# include <sstream>
# include <unistd.h>
# include <stdlib.h>

# include "outputMacros.hpp"



enum AllowMethods
{
	GET,
	POST,
	DELETE,
};

class Utils
{
	public:
		static bool FileIsReadable(std::string path);
		static bool FileIsEmpty(std::string path);
		static bool FileIsConf(std::string path);
		static bool FileIsValid(std::string path);

		static void log(const std::string &message, const char *color);
		static void exceptWithError(const char *message);

		static std::string IntToString(int number);
};

class MyError : public std::exception
{
	private:
		const char *_msg;
	public:
		MyError(const char *msg);
		virtual const char * what() const throw();
};

#endif
