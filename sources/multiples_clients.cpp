/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiples_clients.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmorales <moralesrojascr@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:34:52 by cmorales          #+#    #+#             */
/*   Updated: 2024/02/07 17:16:54 by cmorales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define NUM_REQUESTS 5
#define NUM_CLIENTS 2



void p_error(const char * msg)
{
    std::cout << msg << std::endl;
    exit(1);
}

void *client_fuction(void *arg)
{
    int fdsocket;
    struct sockaddr_in servaddr;
    int addrlen = sizeof(servaddr);

    if((fdsocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        p_error("Error while creating socket");
        
    bzero(&servaddr, addrlen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0)
        p_error("Inet_pton error address");
        
    clock_t start = clock();
    if(connect(fdsocket, (struct sockaddr *)&servaddr, addrlen) < 0 )
        p_error("Error connecting to the server");
    
    const char *hello = "Hello from client";
    for(int i = 0; i < NUM_REQUESTS; i++)
    {
        char message[256];
        if(send(fdsocket, hello, strlen(hello), 0) < 0)
            p_error("Error sending the request to the server");
        printf("Cliente %ld - Solicitud enviada: %s\n", (long)arg, hello);

        read(fdsocket, message, 256);
        clock_t end = clock();
        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Tiempo de respuesta en el hilo %d del cliente: %f segundos\n", i,cpu_time_used);
    }
    close(fdsocket);
    return NULL;
    //exit of the thread
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_CLIENTS];
    int i = 0;

    for (i = 0; i < NUM_CLIENTS; i++)
    {
        if(pthread_create(&threads[i], NULL, &client_fuction, (void *)(long)i) != 0)
            p_error("Error creating the threads");
    }
    
    for(int i = 0; i < NUM_CLIENTS; i++)
    {
        if(pthread_join(threads[i], NULL))
            p_error("Error joining the threads");
    } 
    client_fuction(NULL);
    return 0;
}