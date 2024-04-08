/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:38:42 by manujime          #+#    #+#             */
/*   Updated: 2024/04/08 14:06:00 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
# define METHODS_HPP

#include "Utils.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "Utils.hpp"
#include "fstream"


class Methods
{
    public:
        static Response HandleGet(std::string path, Config &config);
        static Response HandleDelete(std::string path, Config &config);
        static Response HandlePost(std::string path, std::string requestText, Config &config);
};

#endif