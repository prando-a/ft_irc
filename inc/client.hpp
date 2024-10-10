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
		int 						socket;
		struct sockaddr_in			address;
		socklen_t					addr_len;
		char						ip_str[INET_ADDRSTRLEN];
		struct hostent*				host_entry;

		unsigned long				uID;
		bool						isRegistered;
		std::string					nick;
		std::string					realname;

		void						setAddress(void);

	public:
		client(int);
		~client();
		client(const client &src);
		client &operator=(const client &src);
};
 
#endif