/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:09:21 by prando-a          #+#    #+#             */
/*   Updated: 2024/09/02 19:09:21 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//   /$$$$$$   /$$          /$$                     //
//  /$$__  $$ | $$         |__/                     //
// | $$  \__//$$$$$$        /$$  /$$$$$$   /$$$$$$$ //
// | $$$$   |_  $$_/       | $$ /$$__  $$ /$$_____/ //
// | $$_/     | $$         | $$| $$  \__/| $$       //
// | $$       | $$ /$$     | $$| $$      | $$       //
// | $$       |  $$$$/     | $$| $$      |  $$$$$$$ //
// |__/        \___//$$$$$$|__/|__/       \_______/ //
//                 |______/           by            //
//                                       prando-a   //
//                                       jose-mgo   //

#ifndef IRC_H
# define IRC_H

# include <iostream>
# include <cstdio>
# include <string>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sstream>
# include <vector>
# include <map>
# include <poll.h>
# include <fcntl.h>
# include <signal.h>
# include <cerrno>
# include <ctime>

# define RED     	"\x1b[31m"
# define GREEN   	"\x1b[32m"
# define YELLOW  	"\x1b[33m"
# define BLUE    	"\x1b[34m"
# define MAGENTA	"\x1b[35m"
# define CYAN    	"\x1b[36m"
# define WHITE		"\033[37m"
# define BOLD		"\033[1m"
# define RESET   	"\x1b[0m"
# define CLEAR		"\033[2J"

enum commandType
{
	NICK,
	USER,
	QUIT,
	JOIN,
	PART,
	PRIVMSG,
	KICK,
	INVITE,
	TOPIC,
	MODE,
	CAP,
	PASS,
    WHO,
	INVALID = -1
};

enum IrcErrCode {

     // Respuestas generales de error
    ERR_PASSWDMISMATCH = 464, // :Password incorrect
    ERR_UNKNOWNCOMMAND = 421, // <command> :Unknown command
	ERR_TOOMANYTARGETS = 407, // <target> :<error code> recipients. <abort message>
    ERR_NEEDMOREPARAMS = 461, // <command> :Not enough parameters
    ERR_NOTREGISTERED = 451, // :You have not registered
    ERR_CHANOPRIVSNEEDED = 482, // <channel> :You're not channel operator (segun gpt tmb se puede usar para cuando le das op a un user con op)
    ERR_NOTONCHANNEL = 442,  // <channel> :You're not on that channel
    ERR_USERNOTINCHANNEL = 441, // <nick> <channel> :They aren't on that channel // error al echar a alguien que no está en el canal
    ERR_NOSUCHNICK = 401,    // <nickname> :No such nick/channel
    ERR_NOSUCHCHANNEL = 403, // <channel> :No such channel
    ERR_UNKNOWNMODE = 472,   // <char> :is unknown mode char to me for <channel>
    ERR_ALREADYREGISTRED = 462, // :You may not reregister
    ERR_NICKNAMEINUSE = 433, // <nick> :Nickname is already in use
    ERR_BADCHANMASK = 476,
    ERR_INVITEONLYCHAN = 473, // invite only channel
    ERR_USERONCHANNEL = 443, // user is already in the channel


    RPL_TOPIC = 332,         // <channel> :<topic> // éxito al unirse a un canal
    // Respuestas básicas
    RPL_WELCOME = 001,       // :Welcome to the Internet Relay Network <nick>!<user>@<host>
    RPL_YOURHOST = 002,      // :Your host is <servername>, running version <version>
    RPL_CREATED = 003,       // :This server was created <date>
    RPL_MYINFO = 004,        // <servername> <version> <available user modes> <available channel modes>
    RPL_BOUNCE = 005,        // :Try server <server name>, port <port number>

    // Respuestas al comando JOIN
    RPL_NAMREPLY = 353,      // = <channel> :<nick> *( " " <nick> )
    RPL_ENDOFNAMES = 366,    // <channel> :End of /NAMES list



    // Respuestas al comando PRIVMSG/NOTICE

    ERR_CANNOTSENDTOCHAN = 404, // <channel> :Cannot send to channel
    ERR_NOTEXTTOSEND = 412,  // :No text to send

    // Respuestas al comando NICK
    ERR_NONICKNAMEGIVEN = 431, // :No nickname given
    ERR_ERRONEUSNICKNAME = 432, // <nick> :Erroneous nickname


    // Respuestas al comando USER




    // Respuestas al comando MODE
    RPL_CHANNELMODEIS = 324, // <channel> <mode>
   
    ERR_USERSDONTMATCH = 502 // :Cannot change mode for other users
};


class server;
class channel;
class client;
class command;

# include "server.hpp"
# include "channel.hpp"
# include "client.hpp"
# include "command.hpp"

std::string intToStr(int n);
void sendErrResponse(std::string host, int rplCode, int sock);

#endif
