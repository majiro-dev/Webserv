/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:02:24 by manujime          #+#    #+#             */
/*   Updated: 2024/04/03 10:32:17 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "../includes/Utils.hpp"

class AutoIndex
{
    public:
        static std::string  GetAutoIndex(std::string path);
};

#endif