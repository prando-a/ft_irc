/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 16:58:05 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/06 16:58:05 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_H
# define CHANNEL_H

# include "ft_irc.hpp"

# define NO_LIMIT -1

class channel
{
	private:
		std::string					name;
		std::string					topic;
		std::string					key;
		std::string					password;
		std::vector<int>			uList;
		std::vector<int>			opList;
		int							usrLimit;
		bool 						inviteOnly;
		bool						topicLock;

	public:
		channel(std::string, std::string, int);
		bool		isRegistered(int);
		bool		isOperator(int);
		void		setOperator(int);
		void		setTopic(std::string, int);
		void		setPass(std::string password);
		void		setInviteOnly(bool b);
		void		addUser(int);
		std::string	getName(void) const;
		std::string	getTopic(void) const;
		std::string	getPass(void) const;
		bool		sendToChannel(std::string to_send, int exc);
		void		deleteOp(int sock);
		void		deletePass();
		void		deleteUser(int sock);
		void		setTopicLock(bool b);
		bool		getTopicLock() const;
		bool		getInviteOnly() const;
		std::vector<int>		&getUList();
		std::vector<int> getOpList() const;
		~channel();
		channel(const channel &src);
		channel &operator=(const channel &src);
};


#endif