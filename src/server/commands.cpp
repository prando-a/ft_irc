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

void server::cmdNICK(command cmd)
{
	std::cout << "\tfuncion NICK" << std::endl;
}

void server::cmdUSER(command cmd)
{
	std::cout << "\tfuncion USER" << std::endl;
}

void server::cmdQUIT(command cmd)
{
	
}

void server::cmdJOIN(command cmd)
{
	
}

void server::cmdPART(command cmd)
{
	
}

void server::cmdPRIVMSG(command cmd)
{
	
}

void server::cmdKICK(command cmd)
{
	
}

void server::cmdINVITE(command cmd)
{

}

void server::cmdTOPIC(command cmd)
{

}

void server::cmdMODE(command cmd)
{
	
}

void server::cmdCAP(command cmd)
{
	(void)cmd;
	std::cout << "\tfuncion CAP" << std::endl;
}
