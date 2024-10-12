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

std::string itostr(int n)
{
    std::string str;
    std::stringstream ss;
    ss << n;
    ss >> str;
    return str;
}

std::string itor(int rplCode)
{
    switch (rplCode)
    {
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
        default:
            break;
    }
    return "If you can see this, I deserve dying slowly...";
}



void sendErrResponse(std::string host, int rplCode, int sock)
{
	std::string response = ":";
                response += host + " ";
                response += itostr(rplCode) + " : ";
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

#define MAX_CLIENTS 1024
#define BUFFER_SIZE 1024

int sToPort(std::string port)
{
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
            throw "Invalid port number.";
    }
    return (ft_atoi(port.c_str()));
}

std::vector<std::string> split(std::string str)
{
	std::vector<std::string> params;
	std::string tmp;
	std::stringstream ss(str);
	while (std::getline(ss, tmp, '\n'))
		params.push_back(tmp);
	return params;
}

void process_command(std::string buffer, server& server, int socket)
{
	std::cout << "Recibido: " << buffer << std::endl;
    if (buffer.length() == 0) return;
	command cmd(buffer);
	try
	{   cmd.parse();
		server.useCommand(cmd, socket);
	}
	catch (IrcErrCode replCode)
	    { sendErrResponse(server.getHostname(), replCode, socket); }
}

int multiserv()
{
    server server("6262", 6667);
    char buffer[BUFFER_SIZE];
    int new_socket;

    // Array de clientes
    int client_socket[MAX_CLIENTS] = {0};
    fd_set readfds;

    // Bucle principal
    while (true)
    {
        // Limpiar el conjunto de descriptores
        FD_ZERO(&readfds);

        // Agregar el socket del servidor al conjunto
        FD_SET(server.getSocket(), &readfds);
        int max_sd = server.getSocket();

        // Agregar los sockets de clientes al conjunto
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Esperar actividad en alguno de los sockets
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("Error en select");
        }

        // Si hay una nueva conexión
        if (FD_ISSET(server.getSocket(), &readfds))
        {
            new_socket = server.acceptConnection();
            std::cout << "Nueva conexión aceptada: FD " << new_socket << std::endl;

            // Añadir nuevo cliente a la lista
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    std::cout << "Agregando a la lista de clientes en la posición " << i << std::endl;
                    break;
                }
            }
        }

        // Leer datos de los clientes
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_socket[i];

            // Si el socket está activo
            if (FD_ISSET(sd, &readfds))
            {
                // Leer datos del cliente
                memset(buffer, 0, sizeof(buffer));
                int valread = read(sd, buffer, BUFFER_SIZE);
                if (valread == 0)
                {
                    // Si se recibe 0, el cliente se desconecta
                    std::cout << "Cliente desconectado: FD " << sd << std::endl;
                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    // Procesar el mensaje recibido
                    std::vector<std::string> commands = split(buffer);
                    for (int i = 0; i < commands.size(); i++)
                    {
                        process_command(commands[i], server, sd);
                    }
                }
            }
        }
    }
    return 0;
}

void command_tester(void)
{
	std::string buffer;
	while ((std::cout << BOLD "tester" GREEN ">  " RESET)
	&& (std::getline(std::cin, buffer)) && !(buffer == "EXIT"))
	{
		command cmd(buffer);
	}
}

int main(int argc, char **argv)
{
	try
	{
		//if (argc != 2 || !argv[1][0])
		//	throw "Error: No arguments given.";
	}
	catch (const char *e)
		{ std::cerr << RED << e << "\n" << RESET; return (1); }

	multiserv();

	return (0);
}

                        //std::vector<std::string> commands = split(buffer);
						//for (size_t i = 0; i < commands.size(); i++)
						//{
						//	process_command(commands[i], server, fds[i].fd);
						//}