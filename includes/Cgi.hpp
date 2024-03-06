/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:02:49 by manujime          #+#    #+#             */
/*   Updated: 2024/03/06 20:17:40 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "Utils.hpp"

class Cgi
{
    public:
        Cgi(void);
        ~Cgi(void);

        void    SetCgiPath(std::string path);
        void    SetCgiExtension(std::string extension);
        std::string GetCgiPath(void);
        std::string GetCgiExtension(void);
        std::string GetResult(void);

        static bool IsCgi(std::string path);
        bool ExecuteCgi(char **env, char **argv);

    private:
        std::string cgiPath;
        std::string cgiExtension;
        std::string result;
};

#endif