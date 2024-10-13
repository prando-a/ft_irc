/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 15:11:56 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/13 15:11:56 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.hpp"

// Todas las funciones relacionadas a búsqueda y verificación de usuarios y canales
// Faltan funciones de verificación de canales.

bool server::isRegistered(int sock)
{
	for (int i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList[i]->getSocket() == sock)
			return (true);
	}
	return (false);
}

bool server::nickExists(std::string nick)
{
	for (int i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList[i]->getNickName() == nick)
			return (true);
	}
	return (false);
}

client *server::getClientbySock(int sock)
{
	for (int i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList[i]->getSocket() == sock)
			return (this->clientList[i]);
	}
	return (NULL);
}

client *server::getClientbyNick(std::string nick)
{
	for (int i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList[i]->getNickName() == nick)
			return (this->clientList[i]);
	}
	return (NULL);
}

channel *server::getChannelbyName(std::string name)
{
	for (int i = 0; i < this->channelList.size(); i++)
	{
		if (this->channelList[i]->getName() == name)
			return (this->channelList[i]);
	}
	throw ERR_NOSUCHCHANNEL;
	return (NULL);
}

