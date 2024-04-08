/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:38:42 by manujime          #+#    #+#             */
/*   Updated: 2024/04/06 19:49:52 by cmorales         ###   ########.fr       */
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

class Methods
{
    public:
        static Response HandleGet(std::string &path, Config &config);
        static void HandleDelete(std::string path, std::string &response, int &status);
        static void HandlePost(std::string path, std::string &response, int &status);
};

#endif