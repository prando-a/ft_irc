/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:14:06 by prando-a          #+#    #+#             */
/*   Updated: 2024/09/02 19:14:06 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "ft_irc.hpp"

class server
{
	private:
		std::string				pass;
		int						port;
		int 					socket_fd;
		std::vector<channel>	channelList;
		std::vector<client>		clientList;
		unsigned long			latestChID;
		unsigned long			latestUsrID;
		typedef void			(server::*funcPTR)(command);
		funcPTR					func[11];
		void 					cmdNICK(command);
		void 					cmdUSER(command);
		void 					cmdQUIT(command);
		void 					cmdJOIN(command);
		void 					cmdPART(command);
		void 					cmdPRIVMSG(command);
		void 					cmdKICK(command);
		void 					cmdINVITE(command);
		void 					cmdTOPIC(command);
		void 					cmdMODE(command);
		void 					cmdCAP(command);

	public:
		server(std::string, int);
		void useCommand(command);
		~server();
		server(const server &src);
		server &operator=(const server &src);
};

#endif