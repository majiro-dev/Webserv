/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manujime <manujime@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:35:35 by manujime          #+#    #+#             */
/*   Updated: 2024/02/08 18:02:58 by manujime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Utils.hpp"


class Location
{
	public:
		Location(void);
		~Location(void);

		bool	IsValid;

		std::string 	GetRoot(void);
		bool 			GetAutoindex(void);
		std::list<int> 	GetAllowMethods(void);
		std::string 	GetIndex(void);
		size_t 			GetClientMaxBodySize(void);
		std::string 	GetCgiExtension(void);
		std::string 	GetCgiPath(void);


		void 	SetRoot(std::string root);
		void 	SetAutoindex(std::string autoindex);
		void 	SetAllowMethods(std::list<int> allow_methods);
		void 	SetIndex(std::string index);
		void 	SetClientMaxBodySize(size_t client_max_body_size);
		void 	SetCgiExtension(std::string cgi_extension);
		void 	SetCgiPath(std::string cgi_path);

	private:
		std::string 	root;
		bool 			autoindex;
		std::list<int> 	allow_methods;
		std::string 	index;
		size_t 			client_max_body_size;
		std::string 	cgi_extension;
		std::string 	cgi_path;
};

#endif