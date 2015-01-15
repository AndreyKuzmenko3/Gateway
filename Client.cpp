#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define Server_PORT 1234
#define SERVERADDR "127.0.0.1"
#define TRANSMITTER_COUNT 30
#define DATAGRAMM_COUNT 3000
#define IS_UNIQUE 1

struct IP_PORT
{
	unsigned int IP; //4 байта под IP
	unsigned short PORT;//2 байта по Порт
	IP_PORT(){IP=0;PORT=0;}
	bool operator==(const IP_PORT&aa) {return IP == aa.IP && PORT == aa.PORT;}
};

void GenerateAddress(IP_PORT *ipp)
{
	ipp->IP=rand()%256<<24|rand()%256<<16|rand()%256<<8|rand()%256;
	ipp->PORT=rand()+rand();
	ipp->PORT=ipp->PORT==0?1:ipp->PORT;
}

void GenerateTransmitter(IP_PORT *ipps)
{
	for(int i=0;i<TRANSMITTER_COUNT;i++) {
		GenerateAddress(&ipps[i]);
		if (IS_UNIQUE) {
			bool f=true;
			while (f) {
				f=false;
				for (int j=0;j<i;j++)
					if(ipps[i]==ipps[j] && i!=j) f=true;
				if (f) GenerateAddress(&ipps[i]);
			}
		}
	}
}

char* CreateMessage(IP_PORT *ipps, IP_PORT *ipp, int type, char *buf)
{
	ZeroMemory(buf,13);
	unsigned int mask=255;
	buf[0]=type;
	for (int i=3;i>=0;i--)
		buf[4-i] = (char)((ipp->IP & (mask<<(i*8)))>>(i*8));
	for (int i=1;i>=0;i--)
		buf[6-i] = (char)((ipp->PORT & (mask<<(i*8)))>>(i*8));
	if (1 == type)
	{
		for (int i=7;i<13;i++)
			buf[i]=0;
	}
	else
	{
		if (IS_UNIQUE) {
			bool f=true;
			while (f) {
				f=false;
				for (int i=0;i<TRANSMITTER_COUNT;i++)
					if(&ipps[i]==ipp && !(ipps[i]==*ipp)) f=true;
				if (f) GenerateAddress(ipp);
			}
		}
		else
			GenerateAddress(ipp);
		for (int i=3;i>=0;i--)
			buf[10-i] = (char)((ipp->IP & (mask<<(i*8)))>>(i*8));
		for (int i=1;i>=0;i--)
			buf[12-i] = (char)((ipp->PORT & (mask<<(i*8)))>>(i*8));
	}
	return buf;
}

int main(int argc, char* argv[])
{
	IP_PORT* mass_of_trans=(IP_PORT*)malloc(10000*sizeof(IP_PORT)); //10000 передатчиков по 6 байт каждый
	for (int i=0;i<TRANSMITTER_COUNT;i++)
		mass_of_trans[i]=IP_PORT();

	WSADATA wsa;
	struct sockaddr_in dest_addr;
	SOCKET my_sock;
	srand(time(NULL)); 
	printf("TCP CLIENT\n");


	// Инициализируем библиотеку WinSock
	if (WSAStartup(0x202, &wsa))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}

	// Создаём сокет
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		return -1;
	}

	// заполнение структуры sockaddr_in
	// указание адреса и порта сервера
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(Server_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);


	// адрес сервера получен,сокет клиента создан – пытаемся установить
	// соединение с сервером 
	if (connect(my_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)))
	{
		printf("Connect error %d\n", WSAGetLastError());
		return -1;
	}

	GenerateTransmitter(mass_of_trans);
	unsigned char message[13];
	for(int i=0;i<TRANSMITTER_COUNT;i++) {
		CreateMessage(mass_of_trans, &mass_of_trans[i], 1, (char*)message);

		if (send(my_sock, (char*)message, 13, 0) < 0)
		{
			printf("Send failed");
			break;
		}
	}
	int count=0;
	while (count < DATAGRAMM_COUNT) {
		int trans=rand()%TRANSMITTER_COUNT;
		CreateMessage(mass_of_trans, &mass_of_trans[trans], count%50==0?2:1, (char*)message);

		if (send(my_sock, (char*)message, 13, 0) < 0)
		{
			printf("Send failed");
			break;
		}
		//printf("%s\n",message);
		printf("%u/%u\n",((message[1]<<24)|(message[2]<<16)|(message[3]<<8)|(message[4])),((message[5]<<8)|(message[6])));
		count++;
	}
	closesocket(my_sock);
	WSACleanup();
	return 0;
}