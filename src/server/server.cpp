/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:57:40 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/06 21:57:40 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.hpp"

server::server(std::string pass, int port) : pass(pass), port(port)
{
	this->latestChID = 		0;
	this->latestUsrID = 	0;
	this->func[NICK] =		&server::cmdNICK;
	this->func[USER] =		&server::cmdUSER;
	this->func[QUIT] =		&server::cmdQUIT;
	this->func[JOIN] =		&server::cmdJOIN;
	this->func[PART] =		&server::cmdPART;
	this->func[PRIVMSG] =	&server::cmdPRIVMSG;
	this->func[KICK] =		&server::cmdKICK;
	this->func[INVITE] =	&server::cmdINVITE;
	this->func[TOPIC] =		&server::cmdTOPIC;
	this->func[MODE] =		&server::cmdMODE;
	this->func[CAP] =		&server::cmdCAP;
}

void server::useCommand(command cmd)
{
	(this->*func[cmd.getType()])(cmd);
}

server::~server()
{
}

server::server(const server &src)
{
	*this = src;
}

server &server::operator=(const server &src)
{
	if (this == &src)
		return (*this);
	return (*this);
}
