/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 21:02:34 by manujime          #+#    #+#             */
/*   Updated: 2024/04/09 15:53:59 by manujime         ###   ########.fr       */
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
# include <stdio.h>
# include <string>
# include <iostream>
# include <sstream>
# include <unistd.h>
# include <ctime>

# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <stdlib.h>
# include <poll.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <algorithm>
# include <iomanip>

# include "outputMacros.hpp"

enum text{
	LOG = 0,
	INFO = 1,
	ERROR = 2,
};

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
		static void exceptWithError(const std::string &message);

	static std::string IntToString(int number);
	static int StringToInt(std::string str);
	static uint16_t StringToUint16(std::string str);
	static std::string Uint16ToString(uint16_t number);
	static size_t StringToSizeT(std::string str);
	static std::vector<std::string> Tokenize(const std::string &str, const std::string &delimiters);
	static char **MultimapToCharMatrix(std::multimap<std::string, std::string> &map);
	static void logger(const std::string &msg, int mode);
	
	static std::string slashCleaner(const std::string path);
};

class MyError : public std::exception
{
	private:
		const char *_msg;
	public:
		MyError(const char *msg);
		virtual const char * what() const throw();
};

class SError : public std::exception
{
	private:
		const std::string _msg;
	public:
		SError(const std::string &msg);
		virtual ~SError() throw();
		virtual const char * what() const throw();
};

void handleSignal(int signal);
void leaks(void);

#endif
