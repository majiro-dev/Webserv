/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outputMacros.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:33:43 by manujime          #+#    #+#             */
/*   Updated: 2024/02/02 00:26:54 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUTMACROS_HPP
# define OUTPUTMACROS_HPP

#define ERROR_ARGC "Error: wrong number of arguments.\n"

#define ERROR_FILE_READ "Error: file is not readable or does not exist.\n"
#define ERROR_FILE_EMPTY "Error: file is empty.\n"
#define ERROR_FILE_EXTENSION "Error: file is not a .conf file.\n"
#define ERROR_FILE_VALID "Error: not a valid .conf file.\n"

#define ERROR_SOCKET_CREATE "Error: failed to create socket.\n"
#define ERROR_SOCKET_BIND "Error: failed to bind socket.\n"

#define ERROR_SOCKET_LISTEN "Error: failed to listen on socket.\n"
#define ERROR_SOCKET_READ "Error: failed to read from socket.\n"
#define ERROR_SOCKET_ACCEPT "Error: failed to accept connection.\n"


#define WAITING_CONNECTION "Waiting for incoming connections...\n"
#define REQUEST_RECEIVED "Request received.\n"


#define BUFFER_SIZE 30000

#endif