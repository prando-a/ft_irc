 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 17:33:57 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/06 17:33:57 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "ft_irc.hpp"

class client
{
	private:
		int 						socket_fd;
		unsigned long				uID;
		std::string					nick;
		std::string					realname;

	public:
		client();
		~client();
		client(const client &src);
		client &operator=(const client &src);
};

#endif