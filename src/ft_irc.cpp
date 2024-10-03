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

int miniserv() {
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
    listen(server_fd, 3); //PERMITIDA

    printf("Servidor escuchando en el puerto 6667...\n"); //NO PERMITIDA

    // Aceptar una conexión
	// PERMITIDA
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    if (new_socket < 0) {
        perror("Error al aceptar la conexión");
        exit(EXIT_FAILURE);
    }

    // Mantener el servidor recibiendo mensajes en un bucle
    while (1) {
        memset(buffer, 0, sizeof(buffer)); // Limpiar el buffer
        int valread = read(new_socket, buffer, 1024);
        if (valread > 0) {
            printf("Mensaje recibido: %s\n", buffer);
            // Enviar una respuesta al cliente
            send(new_socket, "Hooola", strlen("Hooola"), 0);
            printf("Mensaje enviado: Hello from server\n");
        } else {
            printf("Conexión cerrada por el cliente.\n");
            break; // Salir del bucle si no hay más datos
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

void command_tester(void)
{
	std::string buffer;
	while ((std::cout << BOLD "tester" GREEN ">  " RESET)
	&& std::getline(std::cin, buffer) && !(buffer == "EXIT"))
	{
		command cmd(buffer);
	}
}

int main(int argc, char **argv)
{
	config		config;
	try
	{
		//if (argc != 2 || !argv[1][0])
		//	throw "Error: No arguments given.";
		//miniserv();
		command_tester();
	}
	catch (const char *e)
		{ std::cerr << e << "\n"; return (1); }

	return (0);
}