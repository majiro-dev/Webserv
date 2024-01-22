/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:38:23 by manujime          #+#    #+#             */
/*   Updated: 2024/01/22 12:03:13 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "outputMacros.hpp"

# include <iostream>

class Config
{
    public:
        Config(void);
        Config(char *path);
        ~Config(void);
};

#endif