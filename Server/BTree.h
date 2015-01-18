#include "IP_PORT.h"
#pragma pack(1)
struct binary_tree
{
	IP_PORT key;
	binary_tree *left, *right;
	binary_tree(IP_PORT k) {key=k;left=right=NULL;}
};
#pragma pack()

binary_tree* createbt(IP_PORT);
binary_tree* insert(binary_tree*, IP_PORT);
binary_tree* find(binary_tree*, IP_PORT);
IP_PORT rightmost(binary_tree*);
binary_tree* remove(binary_tree*, IP_PORT);
void deletebt(binary_tree*);