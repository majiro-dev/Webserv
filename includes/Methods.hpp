/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:38:42 by manujime          #+#    #+#             */
/*   Updated: 2024/04/01 17:43:21 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
# define METHODS_HPP

#include "Utils.hpp"
#include "Request.hpp"
#include "Config.hpp"
#include "Utils.hpp"

class Methods
{
    public:
        static void HandleGet(std::string path, std::string &response, int &status, Config &config); 
        static void HandleDelete(std::string path, std::string &response, int &status);
        static void HandlePost(std::string path, std::string &response, int &status);
};

#endif