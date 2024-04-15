/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:38:42 by manujime          #+#    #+#             */
/*   Updated: 2024/04/15 11:54:51 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
# define METHODS_HPP

#include "Utils.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "AutoIndex.hpp"
#include "Utils.hpp"
#include "fstream"


class Methods
{
    public:
        static Response HandleGet(std::string &path, Config &config, Request &req);
        static Response HandlePost(std::string path, Request requestText);
        static Response HandleDelete(std::string path);
};

#endif