/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outputMacros.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:33:43 by manujime          #+#    #+#             */
/*   Updated: 2024/02/06 20:39:35 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUTMACROS_HPP
# define OUTPUTMACROS_HPP

# define RESET			"\033[0m"
# define RED			"\033[31m"				/* Red */
# define GREEN			"\033[32m"				/* Green */
# define YELLOW			"\033[33m"				/* Yellow */
# define BLUE			"\033[34m"				/* Blue */
# define MAGENTA		"\033[35m"				/* Magenta */
# define CYAN			"\033[36m"				/* Cyan */

#define ERROR_ARGC "\033[31mError: wrong number of arguments.\033[0m\n"

#define ERROR_FILE_READ "\033[31mError: file is not readable or does not exist.\033[0m\n"
#define ERROR_FILE_EMPTY "\033[31m Error: file is empty.\033[0m\n"
#define ERROR_FILE_EXTENSION "\033[31mError: file is not a .conf file.\033[0m\n"
#define ERROR_FILE_VALID "\033[31mError: not a valid .conf file.\033[0m\n"

#define ERROR_SOCKET_CREATE "\033[31mError: failed to create socket.\033[0m\n"
#define ERROR_SOCKET_BIND "\033[31mError: failed to bind socket.\033[0m\n"

#define ERROR_SOCKET_LISTEN "\033[31mError: failed to listen on socket.\033[0m\n"
#define ERROR_SOCKET_READ "\033[31mError: failed to read from socket.\033[0m\n"
#define ERROR_SOCKET_ACCEPT "\033[31mError: failed to accept connection.\033[0m\n"


#define WAITING_CONNECTION "\033[36m\n+++++++ Waiting for new connection ++++++++\033[0m\n\n"
#define REQUEST_RECEIVED "Request received.\n"


#define BUFFER_SIZE 30000

#endif