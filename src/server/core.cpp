/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prando-a <prando-a@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 13:32:09 by prando-a          #+#    #+#             */
/*   Updated: 2024/10/13 13:32:09 by prando-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.hpp"

void server::processData(std::string buffer, int *socket)
{
    if (buffer.length() == 0 || *socket == 0) return;
	std::cout << "Recibido: " << buffer << std::endl;
	command cmd(buffer);
	try
	{
		cmd.parse();
		this->useCommand(cmd, *socket);
	}
	catch (IrcErrCode replCode)
	    { sendErrResponse(this->getHostname(), replCode, *socket); }

	if (isRegistered(*socket) == false && cmd.getType() != CAP)
	{
		close(*socket);
		*socket = 0;
	}
}

void server::readData(void)
{
	char buffer[1024];
	// Leer datos de los clientes
    for (int i = 0; i < 512; i++)
    {
        // Si el socket está activo
        if (FD_ISSET(sockets[i], &readfds))
        {
            // Leer datos del cliente
            memset(buffer, 0, sizeof(buffer));
            int valread = recv(sockets[i], buffer, sizeof(buffer) - 1, 0);
            if (valread == 0)
            {
                // Si se recibe 0, el cliente se desconecta
                std::cout << "Cliente desconectado: FD " << sockets[i] << std::endl;
                if (isRegistered(sockets[i]))
					unregisterUser(sockets[i]);
				close(sockets[i]);
				sockets[i] = 0;
            }
            else
            {
				this->sockbuff[i] += buffer;
                while (this->sockbuff[i].find("\n") != std::string::npos)
				{
					std::string line = this->sockbuff[i].substr(0, this->sockbuff[i].find("\n"));
					this->processData(line, &sockets[i]);
					this->sockbuff[i] = this->sockbuff[i].substr(this->sockbuff[i].find("\n") + 1);
				}
            }
        }
    }
}

void server::acceptConnection(void)
{
	if (FD_ISSET(this->getSocket(), &readfds))
    {
		int new_socket;
        new_socket = accept(this->_socket, (struct sockaddr *)&this->address, (socklen_t *)&this->addrlen);
		if (new_socket < 0)
			throw "Error: No se pudo aceptar la conexión.";
        std::cout << "Nueva conexión aceptada: FD " << new_socket << std::endl;

        // Añadir nuevo cliente a la lista
        for (int i = 0; i < 512; i++)
		{
            if (this->sockets[i] == 0)
            {
                this->sockets[i] = new_socket;
                std::cout << "Agregando a la lista de clientes en la posición " << i << std::endl;
                break;
            }
        }
    }
}

void server::manageSockets(void)
{
	    // Limpiar el conjunto de descriptores
        FD_ZERO(&this->readfds);

        // Agregar el socket del servidor al conjunto
        FD_SET(this->getSocket(), &this->readfds);

        // Agregar los sockets de clientes al conjunto
        int max_sd = this->getSocket();

        for (int i = 0; i < 512; i++)
		{
            int sd = this->sockets[i];
            if (sd > 0)
                FD_SET(sd, &this->readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

		// Esperar actividad en alguno de los sockets
        if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0)
            perror("Error en select");
}
