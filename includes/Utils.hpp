/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 21:02:34 by manujime          #+#    #+#             */
/*   Updated: 2024/03/17 21:06:13 by manujime         ###   ########.fr       */
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
# include <vector>
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
# include <sys/wait.h>
# include <sys/stat.h>

# include "outputMacros.hpp"

class Utils
{
	public:
		static bool FileIsReadable(std::string path);
		static bool FileIsEmpty(std::string path);
		static bool FileIsConf(std::string path);
		static bool FileIsValid(std::string path);

		static bool DirIsValid(std::string path);

		static void log(const std::string &message, const char *color);
		static void exceptWithError(const char *message);

	static std::string IntToString(int number);
	static int StringToInt(std::string str);
	static uint16_t StringToUint16(std::string str);
	static size_t StringToSizeT(std::string str);
	static char **MultimapToCharMatrix(std::multimap<std::string, std::string> &map);
	static std::vector<std::string> Tokenize(const std::string &str, const std::string &delimiters);
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
