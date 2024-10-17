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
#include <algorithm>


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
	(void)cmd;
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	unregisterUser(sock);
}

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
		if (!cli->isInvited(cmd.getParams()[0]) && ((!ch->getPass().empty() && cmd.getParams().size() < 2)
			|| (!ch->getPass().empty() && ch->getPass() != cmd.getParams()[1])))
			throw ERR_PASSWDMISMATCH;
		if (ch->getInviteOnly() && !cli->isInvited(cmd.getParams()[0]))
			throw ERR_INVITEONLYCHAN;
		else if (cli->isInvited(cmd.getParams()[0]))
			cli->deleteChannel(cmd.getParams()[0]);
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
	ch->sendToChannel(confirm, -1);

	std::string res = ":" + this->getHostname() + " "
					+ intToStr(332) + " " + cli->getNickName() + " "
					+ cmd.getParams()[0] + " :" + ch->getTopic() + "\r\n";
	ch->sendToChannel(res, -1);
}

void server::cmdPART(command cmd, int sock) //por terminar
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	//client	*cli = getClientbySock(sock);
	channel	*ch = getChannelbyName(cmd.getParams()[0]);
	if (!ch->isRegistered(sock))
		throw ERR_USERNOTINCHANNEL;
	if (ch->isOperator(sock))
		ch->deleteOp(sock);
	ch->deleteUser(sock);
	std::cout << RED <<  ch->getUList().size() << std::endl;
	if (!ch->getUList().size())
	{
		std::cout << GREEN << "No users in the channel, deleting ..." << RESET << std::endl;
		this->deleteChannel(ch->getName());
		throw ERR_NOSUCHCHANNEL;
	}
	if (!ch->getOpList().size())
		ch->setOperator(*ch->getUList().begin());
}

void	server::cmdWHO(command cmd, int sock)
{
	(void)cmd;
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	
}

void server::cmdPRIVMSG(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	channel *ch;
	try
	{
		ch = getChannelbyName(cmd.getParams()[0]);
	}
	catch(...)
	{
		ch = NULL;
	}
	client *sender = getClientbySock(sock);
	if (ch) // for message to a channel
	{
		if (std::find(ch->getUList().begin(), ch->getUList().end(), sock) == ch->getUList().end())
			throw ERR_USERNOTINCHANNEL;
		std::string msg = ":" + sender->getNickName() + "!" + sender->getNickName() + "@" + getHostname() +" PRIVMSG " + cmd.getParams()[0] + " :" + cmd.getTrailing() + "\r\n";
		ch->sendToChannel(msg, sock);
	}
	else if (client *cl = getClientbyNick(cmd.getParams()[0])) // for message to a person
	{
		std::string priv_msg = ":" + sender->getNickName() + "!" + sender->getNickName() + "@" + getHostname() +" PRIVMSG " + cmd.getParams()[0] + " :" + cmd.getTrailing() + "\r\n";
		send(cl->getSocket(), priv_msg.c_str(), priv_msg.length(), 0);
	}
	else
		throw
			ERR_NOSUCHNICK;
}

void server::cmdKICK(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	channel *ch = getChannelbyName(cmd.getParams()[0]);
	if (!ch->isOperator(sock))
		throw ERR_CHANOPRIVSNEEDED;
	// comprobar que sea op
	std::vector<int>::iterator it;
	if (std::find(ch->getUList().begin(), ch->getUList().end(), sock) == ch->getUList().end())
		throw ERR_USERNOTINCHANNEL;
	client	*cli = getClientbySock(sock);
	for (it = ch->getUList().begin(); it < ch->getUList().end() && ch->getUList()[std::distance(ch->getUList().begin(), it)] != 
		getClientbyNick(cmd.getParams()[1])->getSocket(); ++it)
		;
	if (ch->getUList().end() == it)
		throw ERR_NOSUCHNICK;
	ch->getUList().erase(it);
	
	std::string kick = ":" + cli->getNickName() + "!" + cli->getUserName() + "@" + cli->getHostName() 
                           + " KICK " + cmd.getParams()[0] + " " + cmd.getParams()[1] + " :" + cmd.getTrailing() + "\r\n";
	ch->sendToChannel(kick, -1);
}

void server::cmdINVITE(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	client *inviter = getClientbySock(sock);
	client *guest = getClientbyNick(cmd.getParams()[0]);
	if (!guest)
		throw ERR_NOSUCHNICK;
	channel *ch;
	ch = getChannelbyName(cmd.getParams()[1]);
	guest->setChannel(cmd.getParams()[1]);
	std::string confirm = ":" + inviter->getNickName() + " INVITE "
					+ guest->getNickName()+ " :" + cmd.getParams()[1] + "\r\n";
	send(guest->getSocket(), confirm.c_str(), confirm.length(), 0);
	ch->sendToChannel(confirm, -1);
}

void server::cmdTOPIC(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	client *cli = getClientbySock(sock);
	channel *ch = getChannelbyName(cmd.getParams()[0]);
	if (ch->getTopicLock() && !ch->isOperator(sock)) //Si esta bloqueado y no es op no se puede cambiar
		throw ERR_CHANOPRIVSNEEDED;
	std::cout << "topic lock = " << ch->getTopicLock() << " is op = " << ch->isOperator(sock) <<  std::endl;
	if (std::find(ch->getUList().begin(), ch->getUList().end(), sock) == ch->getUList().end())
		throw ERR_USERNOTINCHANNEL;
	ch->setTopic(cmd.getTrailing(), sock);
	std::string cf = "TOPIC " + ch->getName() + " :" + cmd.getTrailing() + "\r\n";
	if (!ch->sendToChannel(cf, -1))
		return ;
	std::string to_send = ":" + this->getHostname() + " "
					+ intToStr(332) + " "
					+ cmd.getParams()[0] + " :" + cmd.getTrailing() + "\r\n";
	ch->sendToChannel(to_send, -1);

	std::string topic_info = ":" + this->getHostname() + " "
                         + intToStr(333) + " " + cli->getNickName() + " "
                         + cmd.getParams()[0] + " " + getClientbySock(sock)->getUserName() + " "
                         + intToStr(std::time(NULL)) + "\r\n";
	ch->sendToChannel(topic_info, -1);
}

void server::cmdMODE(command cmd, int sock)
{
	if (isRegistered(sock) == false)
		throw ERR_NOTREGISTERED;
	channel *ch = getChannelbyName(cmd.getParams()[0]);
	client	*cl = getClientbySock(sock);
	if (!ch->isOperator(sock))
		throw ERR_CHANOPRIVSNEEDED;
	if (cmd.getParams().size() > 1 && cmd.getParams()[1] == "+t") //For topic lock
	{
		channel *ch = getChannelbyName(cmd.getParams()[0]);
		ch->setTopicLock(true);
		ch->sendToChannel(":" + cl->getNickName() + " MODE " + " +t\r\n", -1);
	}
	else if (cmd.getParams().size() > 1 && cmd.getParams()[1] == "-t")
	{
		channel *ch = getChannelbyName(cmd.getParams()[0]);
		ch->setTopicLock(false);
		ch->sendToChannel(":" + cl->getNickName() + " MODE " + " -t\r\n", -1);
	}
	else if (cmd.getParams().size() > 2 && cmd.getParams()[1] == "+o") //Make a user op
	{
		channel *ch = getChannelbyName(cmd.getParams()[0]);
		ch->setOperator(getClientbyNick(cmd.getParams()[2])->getSocket());
		ch->sendToChannel(":" + cl->getNickName() + " MODE " + ch->getName() + " +o " + cmd.getParams()[2] + "\r\n", -1);
	}
	else if (cmd.getParams().size() > 2 && cmd.getParams()[1] == "-o")
	{
		channel *ch = getChannelbyName(cmd.getParams()[0]);
		ch->deleteOp(getClientbyNick(cmd.getParams()[2])->getSocket());
		ch->sendToChannel(":" + cl->getNickName() + " MODE " + cmd.getParams()[2] + "\r\n", -1);
	}
	else if (cmd.getParams().size() > 2 && cmd.getParams()[1] == "+k") //Put password
	{
		channel *ch = getChannelbyName(cmd.getParams()[0]);
		ch->setPass(cmd.getParams()[2]);
		ch->sendToChannel(":" + cl->getNickName() + " MODE " + cmd.getParams()[2] + "\r\n", -1);
	}
	else if (cmd.getParams().size() > 2 && cmd.getParams()[1] == "-k")
	{
		channel *ch = getChannelbyName(cmd.getParams()[0]);
		ch->deletePass();
		ch->sendToChannel(":" + cl->getNickName() + " MODE " + cmd.getParams()[2] + "\r\n", -1);
	}
	else if (cmd.getParams().size() > 1 && cmd.getParams()[1] == "+i") //For invite only channel
	{
		channel *ch = getChannelbyName(cmd.getParams()[0]);
		ch->setInviteOnly(true);
		ch->sendToChannel(":" + cl->getNickName() + " MODE " + "\r\n", -1);
	}
	else if (cmd.getParams().size() > 1 && cmd.getParams()[1] == "-i")
	{
		channel *ch = getChannelbyName(cmd.getParams()[0]);
		ch->setInviteOnly(false);
		ch->sendToChannel(":" + cl->getNickName() + " MODE " + "\r\n", -1);
	}
	else
		throw ERR_UNKNOWNMODE;
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