/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:22:47 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/06 22:22:47 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.hpp"

void server::cmdNICK(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
	std::cout << "\tfuncion NICK" << std::endl;
}

void server::cmdUSER(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
	std::cout << "\tfuncion USER" << std::endl;
}

void server::cmdQUIT(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
}

void server::cmdJOIN(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}	
}

void server::cmdPART(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
}

void server::cmdPRIVMSG(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
}

void server::cmdKICK(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
}

void server::cmdINVITE(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
}

void server::cmdTOPIC(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
}

void server::cmdMODE(command cmd, int sock)
{
	if (isRegistered(sock) == false)
	{
		sendResponse("451 ERR_NOTREGISTERED", sock);
		return ;
	}
}

void server::cmdCAP(command cmd, int sock)
{
	if (cmd.getParams().size() == 1 && cmd.getParams()[0] == "END")	
		return ;
	sendResponse("CAP * LS", sock);
}

void server::cmdPASS(command cmd, int sock)
{
	if (cmd.getParams()[0] != this->getPass())
	{
		sendResponse("464 ERR_PASS", sock);
		return ;
	}
	registerUser(sock);
}