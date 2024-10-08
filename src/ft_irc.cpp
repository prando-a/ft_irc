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

void process_command(std::string buffer, server& server, int socket)
{
	std::vector<std::string> params = split(buffer);
	for (size_t i = 0; i < params.size(); i++)
	{
		std::cout << "Recibido: " << params[i] << std::endl;
		try
		{
			command cmd(params[i]);
			server.useCommand(cmd);
		}
		catch (const char *e)
		{
			std::cout << e << std::endl;
			
			ssize_t sent = send(socket, e, strlen(e), MSG_NOSIGNAL);
            if (sent < 0)
            {
                perror("Error al enviar el mensaje de error al cliente");
            }
		}
	}
}

void handle_sigpipe(int sig) {
    std::cout << "SIGPIPE received: " << sig << std::endl;
}

int miniserv()
{
//	signal(SIGPIPE, handle_sigpipe);
	server server("password", 6667);
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Crear el socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0); //PERMITIDA
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //PERMITIDA

    // Configurar la dirección y el puerto
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(6667);  // El puerto que quieras usar //PERMITIDA

    // Bind
    bind(server_fd, (struct sockaddr *)&address, sizeof(address)); //PERMITIDA

    // Listen
    listen(server_fd, 10); //PERMITIDA

    std::cout << "Servidor escuchando en el puerto 6667..." << std::endl; //NO PERMITIDA

    // Configurar poll
    struct pollfd fds[100];
    int nfds = 1;
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    while (1)
	{
        int poll_count = poll(fds, nfds, -1);
        if (poll_count < 0)
		{
            perror("poll error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; i++)
		{
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd)
				{
                    // Aceptar una nueva conexión
                    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                    if (new_socket < 0) {
                        perror("Error al aceptar la conexión");
                        exit(EXIT_FAILURE);
                    }
                    // Añadir el nuevo socket al array de poll
                    fds[nfds].fd = new_socket;
                    fds[nfds].events = POLLIN;
                    nfds++;
                    std::cout << "Nueva conexión aceptada" << std::endl;
                }
				else
				{
                    // Leer datos del cliente
                    memset(buffer, 0, sizeof(buffer));
                    int valread = read(fds[i].fd, buffer, 1024);
                    if (valread > 0)
					{
                        std::vector<std::string> commands = split(buffer);
						for (size_t i = 0; i < commands.size(); i++)
						{
							process_command(commands[i], server, fds[i].fd);
						}
                    }
					else
					{
                        // Cerrar la conexión
                        std::cout << "Conexión cerrada por el cliente." << std::endl;
                        close(fds[i].fd);
                        fds[i] = fds[nfds - 1];
                        nfds--;
                    }
                }
            }
        }
    }

    close(server_fd);
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
		{ std::cerr << e << "\n"; return (1); }

	miniserv();

	return (0);
}