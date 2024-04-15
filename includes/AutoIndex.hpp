/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:02:24 by manujime          #+#    #+#             */
/*   Updated: 2024/04/14 20:23:27 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "../includes/Utils.hpp"

class AutoIndex
{
    public:
        static std::string  GetAutoIndex(std::string path, const std::string &locationPath, std::string uri);
};

#endif