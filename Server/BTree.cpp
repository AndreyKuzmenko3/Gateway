#include "stdafx.h"
#include "BTree.h"
#include "Memory_manager.h"

binary_tree* createbt(IP_PORT k) {
	binary_tree* tmp=(binary_tree*)alloc(16);
	tmp->key=k;
	tmp->left=tmp->right=NULL;
	return tmp;
}

binary_tree* insert(binary_tree* p, IP_PORT k) // вставка ключа k в дерево с корнем p
{
	if( !p ){return p=createbt(k);}
    if ( k < p->key )
        p->left = insert(p->left,k);
    else
        p->right = insert(p->right,k);
    return p;
}

binary_tree* find(binary_tree* p, IP_PORT k) // вставка ключа k в дерево с корнем p
{
	if( !p ) return NULL;
    if( k == p->key ) return p;
	else
		if (k < p->key)
			return find(p->left, k);
		else
			return find(p->right, k);
}

IP_PORT rightmost(binary_tree *p) {
	while (p->right != NULL)
		p=p->right;
	return p->key;
}

binary_tree* remove(binary_tree* root,IP_PORT key) {
	if (NULL == root) return NULL;
	if (root->key == key) {
		if (NULL == root->left && NULL == root->right) {
			freem(root);
			return NULL;
		}
		if (NULL ==root->left && NULL != root->right) {
			binary_tree *tmp=root->right;
			freem(root);
			return tmp;
		}
		if (NULL !=root->left && NULL == root->right) {
			binary_tree *tmp=root->left;
			freem(root);
			return tmp;
		}
		root->key =rightmost(root->left);
		root->left =remove(root->left, root->key);
		return root;
	}
	if (key < root->key) 
		root->left = remove(root->left, key);
	if (root->key < key)
		root->right =remove(root->right, key);
	return root;
}

void deletebt(binary_tree* p) {
	if(p->left != NULL) deletebt(p->left);
	if(p->right != NULL) deletebt(p->right);
	freem(p);
}