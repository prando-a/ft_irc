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

channel::channel(std::string name, std::string topic, int sock) : name(name), topic(topic)
{
	this->usrLimit = NO_LIMIT;
	this->inviteOnly = false;
	this->topicLock = false;
	this->uList.push_back(sock);
	this->opList.push_back(sock);
	if (topic.length() == 0)
		this->topic = "No topic set";
}

bool channel::isRegistered(int sock)
{
	for (int i = 0 ; i < this->uList.size() ; i++)
	{
		if (this->uList[i] == sock)
			return (true);
	}
	return (false);
}

bool channel::isOperator(int sock)
{
	for (int i = 0 ; i < this->opList.size() ; i++)
	{
		if (this->opList[i] == sock)
			return (true);
	}
	return (false);
}

void channel::addUser(int sock)
{
	if (isRegistered(sock))
		throw "Error: User is already in channel";
	this->uList.push_back(sock);
}

void channel::setOperator(int sock)
{
	if (isOperator(sock))
		throw "Error: User is already operator";
	this->opList.push_back(sock);
}

void		channel::setTopicLock(bool b)
{
	topicLock = b;
}

void channel::deleteOp(int sock)
{
	std::vector<int>::iterator it;

	for (it = opList.begin(); it != opList.end(); ++it)
		if (opList[std::distance(opList.begin(), it)] == sock)
			opList.erase(it);
}

void channel::setTopic(std::string topic, int sock)
{
	if (topic.empty())
		throw "Error: Empty topic";
	if (this->topicLock)
	{
		for (int i = 0 ; i < this->opList.size() ; i++)
		{
			if (this->opList[i] == sock)
				break;
			if (i == this->opList.size() - 1)
				throw "Error: Topic is locked";
		}
	}
	this->topic = topic;
}

std::vector<int> &channel::getUList()
{
	return this->uList;
}

bool		channel::sendToChannel(std::string to_send)
{
	std::vector<int>::iterator it;

	for (it = uList.begin(); it != this->uList.end(); ++it)
		if (send(uList[std::distance(uList.begin(), it)], to_send.c_str(), to_send.length(), 0) == -1)
			return false;
	return true;
}

bool		channel::getTopicLock() const
{
	return this->topicLock;
}

std::string channel::getName(void)  const { return (this->name);  }
std::string channel::getTopic(void) const { return (this->topic); }

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