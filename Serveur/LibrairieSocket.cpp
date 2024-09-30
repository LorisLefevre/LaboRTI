#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include "LibrairieSocket.h"

int SocketServeur(int Port)
{
    char PORT[6];
    sprintf(PORT, "%d", Port);

    int Socket_Serveur = socket(AF_INET, SOCK_STREAM, 0);
    if (Socket_Serveur == -1)
    {
        perror("Erreur de création du socket");
        exit(1);
    }

    struct addrinfo hints;
    struct addrinfo *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    if (getaddrinfo(NULL, PORT, &hints, &results) != 0)
    {
        perror("Erreur dans getaddrinfo");
        close(Socket_Serveur);
        exit(1);
    }

    if (bind(Socket_Serveur, results->ai_addr, results->ai_addrlen) < 0)
    {
        perror("Erreur de bind()");
        freeaddrinfo(results);
        close(Socket_Serveur);
        exit(1);
    }

    freeaddrinfo(results);  // Libération des ressources

    if (listen(Socket_Serveur, SOMAXCONN) == -1)
    {
        perror("Erreur dans listen()");
        close(Socket_Serveur);
        exit(1);
    }

    printf("Serveur démarré sur le port %s\n", PORT);

    return Socket_Serveur;
}


int Accept(int Socket_Ecoute, char *ipClient)
{
	int Socket_Service;
	char *Port;

	if(listen(Socket_Ecoute, SOMAXCONN) == -1)
	{
		exit(1);
	}

	else
	{
		Socket_Service = accept(Socket_Ecoute, NULL, NULL);

		if(Socket_Service == -1)
		{
			exit(1);
		}

		else
		{
			struct sockaddr_in AdrClient;
			socklen_t AdrClienLen = sizeof(struct sockaddr_in);

			getpeername(Socket_Service, (struct sockaddr*)&AdrClient, &AdrClienLen);
			getnameinfo((struct sockaddr*)&AdrClient, AdrClienLen, ipClient, NI_MAXHOST, Port, NI_MAXSERV, NI_NUMERICSERV | AI_NUMERICHOST);
		}
	}

	return Socket_Service;
}

int SocketClient(char *ipServeur, int PortServeur)
{
    char PORT[6];
    sprintf(PORT, "%d", PortServeur);

    int Socket_Client = socket(AF_INET, SOCK_STREAM, 0);
    if (Socket_Client == -1)
    {
        perror("Erreur de création du socket client");
        exit(1);
    }

    struct addrinfo hints;
    struct addrinfo *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo(ipServeur, PORT, &hints, &results) != 0)
    {
        perror("Erreur dans getaddrinfo pour le client");
        exit(1);
    }

    if (connect(Socket_Client, results->ai_addr, results->ai_addrlen) == -1)
    {
        perror("Erreur dans connect()");
        freeaddrinfo(results);
        close(Socket_Client);
        exit(1);
    }

    freeaddrinfo(results);  // Libération des ressources après utilisation

    printf("Connexion établie avec le serveur %s sur le port %s\n", ipServeur, PORT);

    return Socket_Client;
}


int Send(int Socket_Service, char *data, int taille)
{
    int Ecriture = write(Socket_Service, data, taille);
    if (Ecriture == -1)
    {
        perror("Erreur d'envoi de données");
        close(Socket_Service);
    }

    return Ecriture;
}


int Receive(int Socket_Service, char *data)
{
    int Lecture = read(Socket_Service, data, 1024);
    if (Lecture == -1)
    {
        perror("Erreur de réception des données");
        close(Socket_Service);
    }

    return Lecture;
}


