/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:46:56 by manujime          #+#    #+#             */
/*   Updated: 2024/02/08 18:02:21 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"

Location::Location(void)
{
}

Location::~Location(void)
{
}

void 	Location::SetRoot(std::string root)
{
	this->root = root;
}

void 	Location::SetAutoindex(std::string autoindex)
{
	if (autoindex == "on")
		this->autoindex = true;
	else
		this->autoindex = false;
}

