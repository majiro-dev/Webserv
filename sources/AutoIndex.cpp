/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
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
    std::cout << "PATHINDEx: " << path << std::endl;
    std::string html;
    std::string title = "Index of " + path;
    std::string header = "<html><head><title>" + title + "</title><style>\
                        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f5f5f5; margin: 0; padding: 0; }\
                        .container { max-width: 800px; margin: 20px auto; padding: 20px; background-color: #999494; border-radius: 10px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }\
                        h1 { color: #000; text-align: center; margin-bottom: 20px; }\
                        .file-list { list-style: none; padding: 0; }\
                        .file-item { background-color: #f9f9f9; padding: 10px; margin-bottom: 5px; border-radius: 5px; }\
                        .file-link { color: #007bff; text-decoration: none; }\
                        .file-link:hover { text-decoration: underline; }\
                        </style></head><body><div class=\"container\"><h1>" + title + "</h1><div class=\"file-list\">";
    std::string footer = "</div></div></body></html>";

    html = header;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) 
        {
            html += "<div class=\"file-item\"><a class=\"file-link\" href=\"";
            html += ent->d_name;
            html += "\">";
            html += ent->d_name;
            html += "</a></div>";
        }
        closedir(dir);
    } 
    else 
    {
        html += "<p>Error opening directory</p>";
    }
    html += footer;
    return html;
}
