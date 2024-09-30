#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>




void Socket_Serveur()
{
	
	printf("\nCréation de la Socket Serveur\n");
	int Socket_Serveur;
	int Socket_Service;

	Socket_Serveur = socket(AF_INET, SOCK_STREAM, 0);

	if(Socket_Serveur == -1)
	{
		perror("\nErreur de socket()\n");
		exit(1);
	}

	else
	{
		printf("\nSocket Serveur créée %d\n", Socket_Serveur);

		printf("\nBind de la Socket Serveur\n");

		struct addrinfo hints;
		struct addrinfo *results;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

		if(getaddrinfo(NULL, "50000", &hints, &results) != 0)
		{
			close(Socket_Serveur);
			exit(1);
		}

		else
		{
			char Hote[NI_MAXHOST];
			char Port[NI_MAXSERV];
			struct addrinfo *info;
			getnameinfo(results->ai_addr, results->ai_addrlen, Hote, NI_MAXHOST, Port, NI_MAXSERV, NI_NUMERICSERV | AI_NUMERICHOST);

			printf("\nL'adresse IP du serveur est : %s\n", Hote);
			printf("\nLe port du serveur est : %s\n", Port);

			if(bind(Socket_Serveur, results->ai_addr, results->ai_addrlen) < 0)
			{
				perror("\nErreur de bind()\n");
				exit(1);
			}

			else
			{
				freeaddrinfo(results);

				printf("\nBind réussi\n");

				printf("\nListen du Serveur\n");

				if(listen(Socket_Serveur, SOMAXCONN) == -1)
				{
					perror("\nErreur de listen()\n");
					exit(1);
				}

				else
				{
					printf("\nListen réussi\n");

					printf("\nAccept du serveur\n");

					Socket_Service = accept(Socket_Serveur, NULL, NULL);

					if(Socket_Service == -1)
					{
						perror("\nErreur d'accept()\n");
						exit(1);
					}

					else
					{
						printf("\nAccept réussi\n");
						printf("\nSocket Service créée %d\n", Socket_Service);

						struct sockaddr_in AdrClient;
						socklen_t AdrClienLen = sizeof(struct sockaddr_in);

						getpeername(Socket_Service, (struct sockaddr*)&AdrClient, &AdrClienLen);
						getnameinfo((struct sockaddr*)&AdrClient, AdrClienLen, Hote, NI_MAXHOST, Port, NI_MAXSERV, NI_NUMERICSERV | AI_NUMERICHOST);

						printf("\nAdresse IP du client : %s\n", Hote);
						printf("\nPort du client : %s\n", Port);
						
						printf("\nRead de la requête client\n");

						int Nombre_Bytes;
						char buffer1[50];

						Nombre_Bytes = read(Socket_Service, buffer1, 10);

						if(Nombre_Bytes == -1)
						{
							perror("\nErreur de read\n");
							close(Socket_Serveur);
							close(Socket_Service);
						}

						else
						{
							buffer1[Nombre_Bytes] = 0;
							printf("\nNombre de bytes lus : %d\n", Nombre_Bytes);
							printf("\nVoici ce qui est lu : %s\n", buffer1);

							printf("\nSecond read de la requête client\n");

							char buffer2[50];

							Nombre_Bytes = read(Socket_Service, buffer2, 10);

							if(Nombre_Bytes == -1)
							{
								perror("\nErreur de read\n");
								close(Socket_Serveur);
								close(Socket_Service);
							}

							else
							{
								buffer2[Nombre_Bytes] = 0;
								printf("\nNombre de bytes lus : %d\n", Nombre_Bytes);
								printf("\nVoici ce qui est lu : %s\n", buffer2);

								printf("\nEcriture sur la socket\n");

								char Reponse[50];
								sprintf(Reponse,"[SERVEUR] %s%s", buffer1, buffer2);

								Nombre_Bytes = write(Socket_Service, Reponse, strlen(Reponse));

								if(Nombre_Bytes == -1)
								{
									perror("\nErreur de write\n");
									close(Socket_Serveur);
									close(Socket_Service);
								}

								else
								{
									printf("\nNombre de bytes écrits : %d\n", Nombre_Bytes);
									printf("\nVoici ce qui est écrit : %s\n", buffer2);
								}
							}
						}
					}
				}

			}
		}
		
	}

}

void Socket_Client()
{
	int argc; 
	char *argv[3];

	printf("\nCréation de la Socket Client\n");
	int Socket_Client;

	Socket_Client = socket(AF_INET, SOCK_STREAM, 0);

	if(Socket_Client == -1)
	{
		perror("\nErreur de socket()\n");
	}

	else
	{
		printf("\nSocket Client créée %d\n", Socket_Client);
		
		printf("\nConnect de la socket Client\n");

		struct addrinfo hints;
		struct addrinfo *results;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_NUMERICSERV;

		if(getaddrinfo(argv[1], argv[2], &hints, &results) != 0)
		{
			exit(1);
		}

		else
		{
			if(connect(Socket_Client, results->ai_addr, results->ai_addrlen) == -1)
			{
				perror("\nErreur de connect\n");
				exit(1);
			}

			else
			{
				printf("\nConnect réussi\n");
				pause();
			}
		}
	}
}

int main()
{
	Socket_Serveur();
	Socket_Client();
}