/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:04:18 by manujime          #+#    #+#             */
/*   Updated: 2024/04/05 13:32:19 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AutoIndex.hpp"
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>

std::string AutoIndex::GetAutoIndex(std::string path)
{
    std::string html;
    std::string title = "Index of " + path;
    std::string header = "<html><head><title>" + title + "</title></head><body><h1>" + title + "</h1><hr><pre>";
    std::string footer = "</pre><hr></body></html>";

    html = header;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) 
        {
            html += "<a href=\"";
            html += ent->d_name;
            html += "\">";
            html += ent->d_name;
            html += "</a><br>";
        }
        closedir(dir);
    } 
    else 
    {
        html += "Error opening directory";
    }
    html += footer;
    return html;
}