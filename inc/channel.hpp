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
		unsigned long				cID;
		std::string					name;
		std::string					topic;
		std::string					key;
		std::vector<unsigned long>	clientList;
		std::vector<unsigned long>	opList;
		int							usrLimit;
		bool 						inviteOnly;
		bool						topicLock;

	public:
		channel();
		void		setOperator(unsigned long);
		void		setTopic(std::string);
		~channel();
		channel(const channel &src);
		channel &operator=(const channel &src);
};


#endif