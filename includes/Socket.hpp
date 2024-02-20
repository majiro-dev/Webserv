/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 19:33:51 by cmorales          #+#    #+#             */
/*   Updated: 2024/02/20 17:52:03 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <netinet/in.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
#include <arpa/inet.h>

# include <iostream>

# define PORT 8080
# define DEFAULT_PORT 80
# define BUFFER 4096
# define MAX_CLIENTS 1000


#endif