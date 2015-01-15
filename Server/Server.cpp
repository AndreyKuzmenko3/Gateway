#include "stdafx.h"
#include "BTree.h"
#include "AVLTree.h"
#include "HashTable.h"
#include "Memory_manager.h"
#include <winsock2.h>
#include <iostream>

using namespace std;
#pragma comment (lib, "Ws2_32.lib")
#define ADD 1
#define CHANGE 2
#define BINARY_TREE 0
#define AVL_TREE 1
#define HASH_TABLE 2

binary_tree* mytree=NULL;
AVL_tree* myavltree=NULL;
Hash_node** hashtable=(Hash_node**)alloc(HASH_SIZE*sizeof(Hash_node*));

IP_PORT Get_IP_PORT(char* mess) {
	IP_PORT tmpipp;
	tmpipp.IP=((unsigned char)mess[1]<<24)|((unsigned char)mess[2]<<16)|((unsigned char)mess[3]<<8)|(unsigned char)mess[4];
	tmpipp.PORT=((unsigned char)mess[5]<<8)|(unsigned char)mess[6];
	return tmpipp;
}

void print_bt(binary_tree* t, int n=0)
{
	if (t->right) {
		print_bt(t->right, n+1);
	}
	for (int i=0;i<n;i++)
		cout<<" ";
	cout<<t->key.IP<<"/"<<t->key.PORT<<"\n";
	if (t->left) {
		print_bt(t->left, n+1);
	}
}
void print_ht(Hash_node** hn) {
	Hash_node *p;
	for(int i=0;i<HASH_SIZE;i++)
	{
		p=hashtable[i];
		while (p) {
			cout<<p->data.IP<<":"<<p->data.PORT<<" ";
			p=p->next;
		}
		cout<<'\n';
	}
}

void AddToStructure(char* buf, int type_of_struct)
{
	IP_PORT tmpipp=Get_IP_PORT(buf);
	switch (type_of_struct)
	{
	case BINARY_TREE:{
		if(NULL == find(mytree,tmpipp)) mytree=insert(mytree,tmpipp);
		break;
					 }
	case AVL_TREE:{
		if(NULL == find(myavltree,tmpipp)) myavltree=insert(myavltree,tmpipp);
		break;
				  }
	case HASH_TABLE:{
		if(NULL == find(hashtable,tmpipp)) insert(hashtable,tmpipp);
		break;
					}
	default:
		break;
	}

}
void ChangeInStructure(char* buf, int type_of_struct) {
	IP_PORT tmpipp=Get_IP_PORT(buf);
	switch (type_of_struct)
	{
	case BINARY_TREE:{
		mytree=remove(mytree,tmpipp);
		tmpipp=Get_IP_PORT(&buf[6]);
		mytree=insert(mytree, tmpipp);
		break;
					 }
	case AVL_TREE:{
		myavltree=remove(myavltree,tmpipp);
		tmpipp=Get_IP_PORT(&buf[6]);
		myavltree=insert(myavltree, tmpipp);
		break;
				  }
	case HASH_TABLE:{
		remove(hashtable,tmpipp);
		tmpipp=Get_IP_PORT(&buf[6]);
		insert(hashtable, tmpipp);
		break;
					}
	default:
		break;
	}
}

void main()
{
	WSADATA wdata;
	if(WSAStartup(MAKEWORD(2,2),&wdata))//инициализация библиотеки сокетов
		printf("%s","Init error");

	sockaddr_in local_addr;
	local_addr.sin_family=AF_INET;
	local_addr.sin_port=htons(1234);
	local_addr.sin_addr.s_addr=0;

	SOCKET serversock;
	serversock=socket(AF_INET,SOCK_STREAM,NULL);
	if(serversock==INVALID_SOCKET)
		printf("%s","Can't create socket");

	if(bind(serversock,(sockaddr*)&local_addr,sizeof(local_addr)))
		printf("%s","Bind Error");

	if(listen(serversock,1))
		printf("%s","listen Error");


	SOCKET clientsock;
	sockaddr_in client_addr;

	int client_addr_size=sizeof(client_addr);
	clientsock=accept(serversock,(sockaddr*)&client_addr,&client_addr_size);

	char buf[13];
	while (recv(clientsock,&buf[0],13,0))
	{
		switch (buf[0])
		{
		case ADD: { 
			for(int i=0;i<3;i++)
			AddToStructure(buf,i); 
			break; 
				  }
		case CHANGE: {
			for(int i=0;i<3;i++)
			ChangeInStructure(buf,i);
			break;
					 }
		default: {
			cout<<"Error";
			return;
				 }
		}
	}
	cout<<(int)myavltree->height;
	print_bt((binary_tree*)myavltree);
	cout<<'\n';
	print_bt((binary_tree*)mytree);
	WSACleanup();//освобождение ресурсов
}
