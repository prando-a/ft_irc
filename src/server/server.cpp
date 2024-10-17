/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:57:40 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/06 21:57:40 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.hpp"

server::server(std::string pass, int port) : pass(pass), port(port)
{
	if (pass.length() == 0) std::cout << YELLOW <<
		"[INFO] Setting server without password \n" << RESET;
	setCommandPTRs();
	setSocket();
	setHostname();
	this->latestChID = 0;
	this->latestUsrID = 0;

}

void server::setSocket(void)
{
	for (int i = 0; i < 512; i++)
		this->sockets[i] = 0;
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == -1)
		throw "Error: No se pudo crear el socket.";

	this->sockopt = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->sockopt, sizeof(this->sockopt)))
		throw "Error: No se pudo configurar el socket.";

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);
	this->addrlen = sizeof(this->address);

	if (bind(this->_socket, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
		throw "Error: No se pudo enlazar el socket.";

	fcntl(this->_socket, F_SETFL, O_NONBLOCK);

	if (listen(this->_socket, 3) < 0)
		throw "Error: No se pudo escuchar el socket.";

	std::cout << "Socket configurado correctamente. " << std::endl;
	std::cout << "Servidor escuchando en el puerto " << this->port << "..." << std::endl;
}

void server::setHostname()
{
    std::vector<char> buffer(256);

	if (gethostname(buffer.data(), buffer.size()) == -1)
		throw "Error al obtener el hostname";

    while (strlen(buffer.data()) == buffer.size() - 1)
	{
		buffer.resize(buffer.size() * 2);
        if (gethostname(buffer.data(), buffer.size()) == -1)
			throw "Error al obtener el hostname";
    }

    this->hostname = std::string(buffer.data());
	std::cout << "Server hostname: " << this->hostname << std::endl;
}

void server::setCommandPTRs(void)
{
	this->func[NICK] =		&server::cmdNICK;
	this->func[USER] =		&server::cmdUSER;
	this->func[QUIT] =		&server::cmdQUIT;
	this->func[JOIN] =		&server::cmdJOIN;
	this->func[PART] =		&server::cmdPART;
	this->func[PRIVMSG] =	&server::cmdPRIVMSG;
	this->func[KICK] =		&server::cmdKICK;
	this->func[INVITE] =	&server::cmdINVITE;
	this->func[TOPIC] =		&server::cmdTOPIC;
	this->func[MODE] =		&server::cmdMODE;
	this->func[CAP] =		&server::cmdCAP;
	this->func[PASS] =		&server::cmdPASS;
	this->func[WHO] =		&server::cmdWHO;
}

void server::useCommand(command cmd, int sock)
{
	(this->*func[cmd.getType()])(cmd, sock);
}

void server::sendResponse(std::string content, int sock)
{
	std::string response = ":" + this->hostname + " " + content + " :\r\n";
	send(sock, response.c_str(), response.length(), 0);
}

void server::registerUser(int sock)
{
	if (isRegistered(sock))
		throw ERR_ALREADYREGISTRED;
	client* newClient = new client(sock);
	this->clientList.push_back(newClient);
}

channel *server::createChannel(std::string name, std::string topic, int sock)
{
	channel *ch = new channel(name, topic, sock);
	this->channelList.push_back(ch);
	return (ch);
}

void	server::deleteChannel(std::string _name)
{
	std::vector<channel *>::iterator it;

	it = this->channelList.begin();
	while (it != channelList.end())
	{
		if (channelList[std::distance(channelList.begin(), it)]->getName() == _name)
		{
			delete channelList[std::distance(channelList.begin(), it)];
			it = channelList.erase(it);
		}
		else
			++it;
	}
}

//falta eliminar el usuario de los canales en los que esté. Sí, debe ser aquí.
void server::unregisterUser(int sock)
{
	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList[i]->getSocket() == sock)
		{
			delete this->clientList[i];
			this->clientList.erase(this->clientList.begin() + i);
			break;
		}
	}
}

int 		server::getSocket(void) const 	{ return (this->_socket);	}
std::string	server::getPass(void) const		{ return (this->pass);		}
std::string	server::getHostname(void) const	{ return (this->hostname);	}



server::~server()
{
	close(this->_socket);
	std::cout << "Servidor cerrado." << std::endl;
}

server::server(const server &src)
{
	*this = src;
}

server &server::operator=(const server &src)
{
	if (this == &src)
		return (*this);
	return (*this);
}
