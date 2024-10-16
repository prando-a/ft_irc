/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:36:37 by prando-a          #+#    #+#             */
/*   Updated: 2024/09/02 15:36:37 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

int	ft_atoi(const char *str)
{
	int	i;
	int	sig;
	int	res;

	if (!str)
		return (0);
	i = -1;
	sig = 1;
	res = 0;
	while (str[++i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sig = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
		res = res * 10 + (str[i++] - '0');
	return (res * sig);
}

int sToPort(std::string port)
{
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
            throw "Invalid port number.";
    }
    return (ft_atoi(port.c_str()));
}

std::string intToStr(int n)
{
    std::string str;
    std::stringstream ss;
    ss << n;
    ss >> str;
    return (str);
}

std::string itor(int rplCode)
{
    switch (rplCode)
    {
        case ERR_PASSWDMISMATCH:
            return "Password incorrect";
            break;
        case ERR_NEEDMOREPARAMS:
            return "Not enough parameters";
            break;
        case ERR_TOOMANYTARGETS:
            return "Too many targets";
            break;
        case ERR_UNKNOWNCOMMAND:
            return "Unknown command";
            break;
        case ERR_NOTREGISTERED:
            return "You have not registered";
            break;
        case ERR_NOTONCHANNEL:
            return "You're not on that channel";
            break;
        case ERR_NOSUCHNICK:
            return "No such nick";
            break;
        case ERR_NOSUCHCHANNEL:
            return "No such channel";
            break;
        case ERR_CHANOPRIVSNEEDED:
            return "You're not channel operator";
            break;
        case ERR_USERNOTINCHANNEL:
            return "They aren't on that channel";
            break;
        case ERR_UNKNOWNMODE:
            return "Unknown mode char to me for this channel";
            break;
        case ERR_ALREADYREGISTRED:
            return "You may not reregister";
            break;
        case ERR_NICKNAMEINUSE:
            return "Nickname is already in use";
            break;
        case ERR_BADCHANMASK:
            return "Bad channel name";
            break;
        case ERR_INVITEONLYCHAN:
            return "Cannot join channel";
            break;
        default:
            break;
    }
    return "If you can see this, I deserve dying slowly...";
}

void sendErrResponse(std::string host, int rplCode, int sock)
{
	std::string response = ":";
                response += host + " ";
                response += intToStr(rplCode) + " : ";
                response += itor(rplCode) + "\r\n";

    std::cout << "Enviando:\n\t" << response << std::endl;
	send(sock, response.c_str(), response.length(), 0);
}

/*

FUNCIONES PERMITIDAS

Everything in C++ 98.
socket, close, setsockopt, getsockname,
getprotobyname, gethostbyname, getaddrinfo,
freeaddrinfo, bind, connect, listen, accept, htons,
htonl, ntohs, ntohl, inet_addr, inet_ntoa, send,
recv, signal, sigaction, lseek, fstat, fcntl, poll
(or equivalent)

*/
std::vector<std::string> split(std::string str)
{
	std::vector<std::string> params;
	std::string tmp;
	std::stringstream ss(str);
	while (std::getline(ss, tmp, '\n'))
		params.push_back(tmp);
	return params;
}

// Prompt para probar comandos infinitamente. Descomenta también
//  los couts del constructor de command para debugear
//
//void command_tester(void)
//{
//	std::string buffer;
//	while ((std::cout << BOLD "tester" GREEN ">  " RESET)
//	&& (std::getline(std::cin, buffer)) && !(buffer == "EXIT"))
//	{
//		command cmd(buffer);
//	}
//}

int main(int argc, char **argv)
{
    if (argv[1] == NULL|| !strcmp(argv[1], "-h"))
        return std::cout << "./ircserv <port> <password>" << std::endl, 0;
	try
	{
		if (argc != 3 || !argv[1][0])
			throw "Error: No arguments given.";
        server server(argv[2], sToPort(argv[1]));
        while (true)
        {
            //./server/core.cpp
            server.manageSockets();
            server.acceptConnection();
            server.readData();
        }
	}
	catch (const char *e)
		{ std::cerr << RED << e << "\n" << RESET; return (1); }
	return (0);
}
