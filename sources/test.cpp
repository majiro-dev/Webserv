/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 21:03:07 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/03 22:23:43 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <poll.h>



#define PORT 8080

void sendPart(int sock, const char* part) {
    send(sock, part, strlen(part), 0);
    std::cout << "Sent part: " << part << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(0));  // Simular un retraso de 1 segundo
}

std::string buildHttpRequest(int contentOption) {
    if (contentOption == 1) {
        // Mensaje con Content-Length
        return "GET / HTTP/1.1\r\n"
               "Host: example.com\r\n"
               "Content-Length: 10\r\n"
               "\r\n"
               "Hola mundo";
    } else {
        // Mensaje con Transfer-Encoding: chunked
        return "GET /path HTTP/1.1\r\n"
               "Host: example.com\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "5\r\n"
               "Hola \r\n"
               "5\r\n"
               "mundo\r\n";
    }
}

int main(int argc, char const **argv) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // Crear el socket de comunicación al servidor con IPv4 localhost y conexión TCP
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error al crear el socket" << std::endl;
        return -1;
    }
    
    // Configurar serv_addr para establecer el socket
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convertir direcciones IPv4 e IPv6 de texto a forma binaria y poner en sin_addr
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Dirección no válida/No compatible" << std::endl;
        return -1;
    }
    // Intentar conectarse al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error de conexión" << std::endl;
        return -1;
    }

    // Enviar la solicitud en partes
    std::string part1 = buildHttpRequest(1).c_str();
    
    const char *part2 = "0\r\n\r\n";

    sendPart(sock, part1.c_str());
    //sendPart(sock, part2);

    // Leer y mostrar la respuesta del servidor
    char buffer[1024] = {0};
    ssize_t valread = read(sock, buffer, sizeof(buffer));
    if (valread > 0) {
        std::cout << std::endl << "Respuesta del servidor: \n" << buffer << std::endl;
    } else {
        std::cerr << "Error de lectura o el servidor cerró la conexión" << std::endl;
    }

    // Cerrar el socket
    close(sock);

    return 0;
}
