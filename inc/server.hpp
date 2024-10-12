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
		int 					_socket;
		struct sockaddr_in		address;
		socklen_t				addrlen;
		int						sockopt;
		std::string				hostname;


		std::vector<client>		clientList;
		std::vector<channel>	channelList;
		unsigned long			latestChID;
		unsigned long			latestUsrID;


		typedef void			(server::*funcPTR)(command, int);
		funcPTR					func[12];
		void 					cmdNICK(command, int);
		void 					cmdUSER(command, int);
		void 					cmdQUIT(command, int);
		void 					cmdJOIN(command, int);
		void 					cmdPART(command, int);
		void 					cmdPRIVMSG(command, int);
		void 					cmdKICK(command, int);
		void 					cmdINVITE(command, int);
		void 					cmdTOPIC(command, int);
		void 					cmdMODE(command, int);
		void 					cmdCAP(command, int);
		void 					cmdPASS(command, int);
		void					sendResponse(std::string, int);
		bool					isRegistered(int);
		void 					registerUser(int);
		void 					unregisterUser(int);

		void 					setCommandPTRs(void);
		void 					setSocket(void);
		void 					setHostname(void);
		std::string				getPass(void) const;


	public:
		server(std::string, int);
		int						getSocket(void) const;
		std::string				getHostname(void) const;
		
		void					useCommand(command, int);
		int 					acceptConnection(void);	
		~server();
		server(const server &src);
		server &operator=(const server &src);
};

#endif