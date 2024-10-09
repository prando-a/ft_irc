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

#define PORT 6667
#define MAX_CLIENTS 100
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
			command cmd(buffer);
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

int multiserv() {
    server server("6262", 32332);
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Crear socket de servidor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Reutilizar dirección y puerto
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configurar la dirección y puerto del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    // Enlazar socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error en bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Poner el servidor en modo escucha
    if (listen(server_fd, 10) < 0) {
        perror("Error en listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor escuchando en el puerto " << PORT << "..." << std::endl;

    // Array de clientes
    int client_socket[MAX_CLIENTS] = {0};
    fd_set readfds;

    // Bucle principal
    while (true) {
        // Limpiar el conjunto de descriptores
        FD_ZERO(&readfds);

        // Agregar el socket del servidor al conjunto
        FD_SET(server_fd, &readfds);
        int max_sd = server_fd;

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
        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket < 0) {
                perror("Error al aceptar la conexión");
                continue;
            }
            std::cout << "Nueva conexión aceptada: FD " << new_socket << std::endl;

            // Añadir nuevo cliente a la lista
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    std::cout << "Agregando a la lista de clientes en la posición " << i << std::endl;
                    break;
                }
            }
        }

        // Leer datos de los clientes
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];

            // Si el socket está activo
            if (FD_ISSET(sd, &readfds)) {
                // Leer datos del cliente
                memset(buffer, 0, sizeof(buffer));
                int valread = read(sd, buffer, BUFFER_SIZE);
                if (valread == 0) {
                    // Si se recibe 0, el cliente se desconecta
                    std::cout << "Cliente desconectado: FD " << sd << std::endl;
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    // Procesar el mensaje recibido
                    buffer[valread] = '\0';
                    std::vector<std::string> commands = split(buffer);
                    for (const auto& command_str : commands) {
                        process_command(command_str, server, sd);
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

	multiserv();

	return (0);
}

                        //std::vector<std::string> commands = split(buffer);
						//for (size_t i = 0; i < commands.size(); i++)
						//{
						//	process_command(commands[i], server, fds[i].fd);
						//}