/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 17:00:11 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/06 17:00:11 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/channel.hpp"

channel::channel()
{

}

void channel::setOperator(unsigned long uID)
{
	for (int i = 0 ; i < this->opList.size() ; i++)
	{
		if (this->opList[i] == uID)
			throw "This user is already an operator";
	}
	this->opList.push_back(uID);
}

void channel::setTopic(std::string topic)
{
	this->topic = topic;
}

channel::~channel()
{
}

channel::channel(const channel &src)
{
	*this = src;
}

channel &channel::operator=(const channel &src)
{
	if (this == &src)
		return (*this);
	return (*this);
}