/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:36:13 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/04 12:36:13 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"

command::command(std::string cmd, int socket)
{
	if (cmd.empty() || cmd.length() <= 0) throw "\n";
	setMembers(cmd, socket);

	std::cout << "Socket del comando: " << this->getSock() << std::endl;
	//IMPRIMIR PARAMETROS (DEBUG)
	// --------------------------------------------------------
	//std::cout << "\nCommand: \n";
	//for (int i = 0; i < this->params.size(); i++)
	//{
	//	std::cout << this->params[i] << "\n";
	//}
	//std::cout << "Trailing: " << this->trailing << "\n";
	//std::cout << "Vector size: " << this->params.size() << "\n\n\n";
	// --------------------------------------------------------

	parse();
}

void command::setMembers(const std::string& str, int socket)
{
	this->socket = socket;
	this->has_trailing = false;
	this->trailing = "";
	std::stringstream ss(str);
	std::string word;

	while (ss >> word)
	{
		if (word[0] == ':')
		{
			this->trailing = str.substr(str.find(word) + 1);
			this->has_trailing = true;
			break;
		}
		this->params.push_back(word);
	}

	this->type = setType(this->params[0]);
	this->params.erase(this->params.begin());
}

int command::setType(std::string cmd)
{
	std::string commands[] =
	{
		"NICK",
		"USER",
		"QUIT",
		"JOIN",
		"PART",
		"PRIVMSG",
		"KICK",
		"INVITE",
		"TOPIC",
		"MODE",
		"CAP",
		"PASS"
	};

	for (int i = 0; i < 12; i++)
	{
		if (commands[i] == cmd)
			return (i);
	}
	return (INVALID);
}

int 						command::getType(void) 		{ return (this->type); 		}
std::vector<std::string>	command::getParams(void)	{ return (this->params);	}
std::string					command::getTrailing(void)	{ return (this->trailing);	}
int							command::getSock(void)		{ return (this->socket);	}


command::~command(){}

command::command(const command &src)
{
	this->params = src.params;
	this->type = src.type;
}

command &command::operator=(const command &src)
{
	this->params = src.params;
	this->type = src.type;
	return (*this);
}
