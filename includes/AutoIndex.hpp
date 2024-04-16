/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:02:24 by manujime          #+#    #+#             */
/*   Updated: 2024/04/16 13:08:23 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "../includes/Utils.hpp"

class AutoIndex
{
    public:
        static std::string  GetAutoIndex(std::string path, std::string uri);
};

#endif