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


int sToPort(std::string port)
{
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
            throw "Invalid port number.";
    }
    return (atoi(port.c_str()));
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
	try
	{
		command cmd(buffer, socket);
		server.useCommand(cmd);
	}
	catch (const char *e)
	{	
        std::cout << "Enviando a: " << socket << std::endl;
		ssize_t sent = send(socket, e, strlen(e), MSG_NOSIGNAL);
        if (sent < 0)
        {
            perror("ESTO NO DEBERÍA SALIR !!!!! ->");
        }
	}
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
        int activity = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
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
                    for (const auto& command_str : commands) {
                        process_command(command_str, server, sd);
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
		command cmd(buffer, 0);
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