/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 19:33:51 by cmorales          #+#    #+#             */
/*   Updated: 2024/01/31 20:08:36 by cmorales         ###   ########.fr       */
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
# define SERVER_PORT 80
# define BUFFER 4096

#endif