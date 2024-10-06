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

client::client()
{
}

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