/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatas-p <jmatas-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:38:42 by manujime          #+#    #+#             */
/*   Updated: 2024/04/15 12:40:27 by jmatas-p         ###   ########.fr       */
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
        static Response HandlePost(std::string path, Request requestText, Config &config);
        static Response HandleDelete(std::string path);
};

#endif