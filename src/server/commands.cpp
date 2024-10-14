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


//completa y funcional
void server::cmdNICK(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	client *cli = getClientbySock(sock);
	if (nickExists(cmd.getParams()[0]))
		throw ERR_NICKNAMEINUSE;
	cli->setNickName(cmd.getParams()[0]);
}

//completa y funcional
void server::cmdUSER(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	client *cli = getClientbySock(sock);
	if (cli->getUserName().length() > 0)
		throw ERR_ALREADYREGISTRED;
	if (cli->getNickName().length() == 0)
		throw ERR_NOTREGISTERED;
	cli->setUserName(cmd.getParams()[0]);
	cli->setRealName(cmd.getTrailing());
	cli->setHostName();
}

//necesita pruebas, hay que gestionar el mensaje de despedida en el trailing,
void server::cmdQUIT(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	unregisterUser(sock);
	
}

//ns pq el trailing no se pasa xd
void server::cmdJOIN(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	client *cli = getClientbySock(sock);
	channel *ch;
	std::cout << "Trailing: " << cmd.getTrailing() << std::endl;
	try
	{
		ch = getChannelbyName(cmd.getParams()[0]);
		ch->addUser(sock);
	}
	catch (IrcErrCode err)
	{
		if (err == ERR_NOSUCHCHANNEL)
			ch = createChannel(cmd.getParams()[0], cmd.getTrailing(), sock);
		else
			throw err;
	}
	std::string confirm = ":"	+ cli->getHostName() + " JOIN :"
								+ cmd.getParams()[0] + "\r\n";
	send(sock, confirm.c_str(), confirm.length(), 0);

	std::string res = ":" + this->getHostname() + " "
					+ intToStr(332) + " " + cli->getNickName() + " "
					+ cmd.getParams()[0] + " :" + ch->getTopic() + "\r\n";
	send(sock, res.c_str(), res.length(), 0);
}

void server::cmdPART(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
}

void server::cmdPRIVMSG(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
}

void server::cmdKICK(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	channel *ch = getChannelbyName(cmd.getParams()[0]);
	// comprobar que sea op
	client	*cli = getClientbySock(sock);
	std::vector<int>::iterator it;
	for (it = ch->getUList().begin(); it < ch->getUList().end() && ch->getUList()[std::distance(ch->getUList().begin(), it)] != 
		getClientbyNick(cmd.getParams()[1])->getSocket(); ++it)
		;
	if (ch->getUList().end() == it)
		throw ERR_NOSUCHNICK;
	ch->getUList().erase(it);
	
	std::string kick = ":" + cli->getNickName() + "!" + cli->getUserName() + "@" + cli->getHostName() 
                           + " KICK " + cmd.getParams()[0] + " " + cmd.getParams()[1] + " :" + cmd.getTrailing() + "\r\n";
	ch->sendToChannel(kick);
}

void server::cmdINVITE(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
}

void server::cmdTOPIC(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	client *cli = getClientbySock(sock);
	channel *ch = getChannelbyName(cmd.getParams()[0]);

	ch->setTopic(cmd.getTrailing(), sock);
	std::string cf = "TOPIC " + ch->getName() + " :" + cmd.getTrailing() + "\r\n";
	if (!ch->sendToChannel(cf))
		return ;
	std::string to_send = ":" + this->getHostname() + " "
					+ intToStr(332) + " "
					+ cmd.getParams()[0] + " :" + cmd.getTrailing() + "\r\n";
	send(sock, to_send.c_str(), to_send.length(), 0);

	std::string topic_info = ":" + this->getHostname() + " "
                         + intToStr(333) + " " + cli->getNickName() + " "
                         + cmd.getParams()[0] + " " + getClientbySock(sock)->getUserName() + " "
                         + intToStr(std::time(NULL)) + "\r\n";
	send(sock, topic_info.c_str(), topic_info.length(), 0);
}

void server::cmdMODE(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
}

void server::cmdCAP(command cmd, int sock)
{
	if (cmd.getParams().size() == 1 && cmd.getParams()[0] == "END")	
		return ;
	sendResponse("CAP * LS", sock);
}

void server::cmdPASS(command cmd, int sock)
{
	if (isRegistered(sock) == true)
		throw ERR_ALREADYREGISTRED;
	if (cmd.getParams()[0] != this->getPass())
		throw ERR_PASSWDMISMATCH;
	registerUser(sock);
}