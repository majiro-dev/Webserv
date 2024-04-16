/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:02:49 by manujime          #+#    #+#             */
/*   Updated: 2024/04/16 12:56:33 by cmorales         ###   ########.fr       */
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
        Cgi(Cgi const & src);

        void        SetCgiPath(std::string path);
        void        SetCgiExtension(std::string extension);
        std::string GetCgiPath(void) const;
        std::string GetCgiExtension(void) const;
        std::string GetResult(void) const;

        static bool IsCgi(std::string path);
        bool        ExecuteCgi(char **env, char **argv, std::string projectPath);

    private:
        std::string cgiPath;
        std::string cgiExtension;
        std::string result;
};

#endif