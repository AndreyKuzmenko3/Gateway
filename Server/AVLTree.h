#include "IP_PORT.h"
#pragma pack(1)
struct AVL_tree
{
	IP_PORT key;
	AVL_tree *left, *right;
	char height;
	AVL_tree(IP_PORT k) {key=k; left=right=NULL; height=1;}
};
#pragma pack()
AVL_tree* createavlt(IP_PORT);
AVL_tree* insert(AVL_tree*, IP_PORT);
AVL_tree* find(AVL_tree*, IP_PORT);
IP_PORT rightmost(AVL_tree*);
AVL_tree* remove(AVL_tree*, IP_PORT);
char height(AVL_tree*);
char bfactor(AVL_tree*);
void fixHeight(AVL_tree*);
AVL_tree* rotateRight(AVL_tree*);
AVL_tree* rotateLeft(AVL_tree*);
AVL_tree* balance(AVL_tree*);
void deleteavlt(AVL_tree*);