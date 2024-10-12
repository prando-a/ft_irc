/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 17:41:39 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/06 17:41:39 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/client.hpp"

client::client(int socket) : socket(socket)
{
	this->nickName = "";
	this->realName = "";
	this->userName = "";
	setAddress();
}

void client::setAddress(void)
{
	if (getpeername(this->socket, (struct sockaddr*)&this->address, &this->addr_len) == -1)
	host_entry = gethostbyaddr(&address.sin_addr, sizeof(address.sin_addr), AF_INET);
	inet_ntop(AF_INET, &address.sin_addr, ip_str, INET_ADDRSTRLEN);
}

void	client::setNickName(std::string nickName)	{ this->nickName = nickName;}
void	client::setUserName(std::string userName)	{ this->userName = userName;}
void	client::setRealName(std::string realName)	{ this->realName = realName;}

int		client::getSocket(void){ return (this->socket); }

client::~client()
{
}

client::client(const client &src)
{
	*this = src;
}

client &client::operator=(const client &src)
{
	if (this == &src)
		return (*this);
	return (*this);
}