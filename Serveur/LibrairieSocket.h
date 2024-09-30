#ifndef TCP_H
#define TCP_H

#define TAILLE_MAX_DATA 10000

int SocketServeur(int Port);
int Accept(int Socket_Ecoute, char *ipClient);
int SocketClient(char *ipServeur, int PortServeur);
int Send(int Socket_Service, char *data, int taille);
int Receive(int Socket_Service, char *data);

#endif