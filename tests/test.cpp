/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatas-p <jmatas-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 21:03:07 by cmorales          #+#    #+#             */
/*   Updated: 2024/04/15 12:41:34 by jmatas-p         ###   ########.fr       */
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

char *getFileBuffer(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        std::cerr << "Error al abrir el archivo" << std::endl;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        std::cerr << "Error al reservar memoria para el archivo" << std::endl;
        return NULL;
    }
    fread(buffer, 1, fileSize, file);
    fclose(file);
    buffer[fileSize] = '\0';
    return buffer;
}

std::string getFileContentLength(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        std::cerr << "Error al abrir el archivo" << std::endl;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);
    std::cout << "File size: " << fileSize << std::endl;
    return std::to_string(fileSize + 2).c_str();
}

void sendPart(int sock, const char* part) {
    send(sock, part, strlen(part), 0);
    std::cout << "Sent part: " << part << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(0));  // Simular un retraso de 1 segundo
}

std::string buildHttpRequest(int contentOption) {

    if (contentOption == 0) {
        // GET Request con contenido
        return "GET /testpost.txt HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Content-Length: " + getFileContentLength("data/testWroteFile.txt") + "\r\n"
            "\r\n" + std::string(getFileBuffer("data/testWroteFile.txt")) + "\r\n";;
    }
    else if (contentOption == 1) {
        // POST Request con archivo HTML
        return "POST /hola.sh HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Content-Type: text/sh\r\n"
            "Content-Length: " + getFileContentLength("data/tours/hola.sh") + "\r\n"
            "\r\n" + std::string(getFileBuffer("data/tours/hola.sh")) + "\r\n";
    }
    else if (contentOption == 2) {
        // Mensaje con Content-Length
        return "DELETE http://localhost:8080/ HTTP/1.1\r\n"
               "Host: example.com\r\n"
               "\r\n";
    }
    else if (contentOption == 3) {
        // Mensaje con Content-Length
        return "DELETE /hola HTTP/1.1\r\n"
               "Host: example.com\r\n";
    }
    else if (contentOption == 4) {
        return "GET /tours/hola.sh HTTP/1.1\r\n"
               "Host: example.com\r\n"
               "\r\n";
    }
    else if (contentOption == 5) {
        return "GET /tours/hola.php HTTP/1.1\r\n"
               "Host: example.com\r\n"
               "\r\n";
    }
    else {
        // Mensaje con Transfer-Encoding: chunked
        return "GET / HTTP/1.1\r\n"
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
    //std::string part1 = buildHttpRequest(1).c_str();
    int contentOption = argc > 1 ? atoi(argv[1]) : 0;
    std::string part1 = buildHttpRequest(contentOption);
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