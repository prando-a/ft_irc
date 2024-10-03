/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:42:57 by prando-a          #+#    #+#             */
/*   Updated: 2024/09/27 16:42:57 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H
# include "ft_irc.hpp"

# define N_REQ	-1

enum command_type
{
	NICK,
	USER,
	QUIT,
	JOIN,
	PART,
	KICK,
	INVITE,
	TOPIC,
	MODE,
	INVALID = -1
};

enum command_errno
{
	WRONG_CMD,
	WRONG_ARGC,
	MISSING_TRAILING,
	INVALID_TRAILING
};

class command
{
	private:
		int							type;
		std::vector<std::string>	params;
		int							has_trailing;
		std::string					trailing;
		void						setMembers(const std::string&);
		int 						setType(std::string);
		void						parse(void);
		void						checkArgC(int);

	public:
		command(std::string);
		command(const command &src);
		command &operator=(const command &src);
		~command();
		int							getType(void);
		std::vector<std::string>	getParams(void);
		std::string					getTrailing(void);
};

void command::setMembers(const std::string& str)
{
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
}

command::command(std::string cmd)
{
	setMembers(cmd);

	//IMPRIMIR PARAMETROS
	std::cout << "\nCommand: \n";
	for (int i = 0; i < this->params.size(); i++)
	{
		std::cout << this->params[i] << "\n";
	}
	std::cout << "Trailing: " << this->trailing << "\n";
	std::cout << "Vector size: " << this->params.size() << "\n";

	try
	{
		//parse();	
	}
	catch (int err) /*La string debe ser lanzada al cliente*/
	{
		switch (err)
		{
			case WRONG_CMD:
				std::cout << "Error: Invalid command" << "\n"; //throw;
					break;
			case WRONG_ARGC:
				std::cout << "Error: Invalid number of arguments" << "\n"; //throw;
					break;
			case MISSING_TRAILING:
				std::cout << "Error: Missing trailing" << "\n"; //throw;
					break;
			case INVALID_TRAILING:
				std::cout << "Error: Invalid trailing" << "\n"; //throw;
					break;
		};
	}
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
		"KICK",
		"INVITE",
		"TOPIC",
		"MODE"
	};

	for (int i = 0; i < sizeof(commands); i++)
	{
		if (commands[i] == cmd)
			return (i);
	}
	return (INVALID);
}

void command::parse(void)
{
	switch (this->type)
	{
		case NICK:
			checkArgC(2);
				break;
		case USER:
			checkArgC(5);
				break;
		case QUIT:
			checkArgC(1);
				break;
		case JOIN:
			checkArgC(2);
				break;
		case PART:
			checkArgC(2);
				break;
		case KICK:
			checkArgC(3);
				break;
		case INVITE:
			checkArgC(3);
				break;
		case TOPIC:
			checkArgC(3);
				break;
		case MODE:
			checkArgC(N_REQ);
				break;
		default:
			throw WRONG_CMD;
			break;
	}
}

void command::checkArgC(int req)
{
	if (req == N_REQ && this->params.size() < 1)
		throw WRONG_ARGC;
	else
		return ;

	if (this->params.size() != req)
		throw WRONG_ARGC;
}

int command::getType(void) 							{ return (this->type); 		}
std::vector<std::string> command::getParams(void)	{ return (this->params);	}
std::string command::getTrailing(void)				{ return (this->trailing);	}

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

# endif