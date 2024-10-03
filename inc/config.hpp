/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:14:06 by prando-a          #+#    #+#             */
/*   Updated: 2024/09/02 19:14:06 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include "ft_irc.hpp"

class config
{
	private:
		std::string		pass;
		int				port;
		int 			socket_fd;

};

#endif