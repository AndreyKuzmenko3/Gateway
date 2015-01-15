#include "stdafx.h"
#include "AVLTree.h"
#include "Memory_manager.h"

char height(AVL_tree* p)
{
	return p?p->height:0;
}

char bfactor(AVL_tree* p) {
	return height(p->right)-height(p->left);
}

void fixHeight(AVL_tree* p) {
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl>hr?hl:hr)+1;
}

AVL_tree* rotateRight(AVL_tree* p) {
	AVL_tree* q = p->left;
	p->left = q->right;
	q->right = p;
	fixHeight(p);
	fixHeight(q);
	return q;
}

AVL_tree* rotateLeft(AVL_tree* p) {
	AVL_tree* q = p->right;
	p->right = q->left;
	q->left = p;
	fixHeight(p);
	fixHeight(q);
	return q;
}

AVL_tree* balance(AVL_tree* p) {
	fixHeight(p);
	if( bfactor(p)==2 )
	{
		if( bfactor(p->right) < 0 )
			p->right = rotateRight(p->right);
		return rotateLeft(p);
	}
	if( bfactor(p)==-2 )
	{
		if( bfactor(p->left) > 0  )
			p->left = rotateLeft(p->left);
		return rotateRight(p);
	}
	return p; // балансировка не нужна
}

AVL_tree* createavlt(IP_PORT k) {
	AVL_tree* tmp=(AVL_tree*)alloc(16);
	tmp->key=k;
	tmp->height=1;
	tmp->left=tmp->right=NULL;
	return tmp;
}

AVL_tree* insert(AVL_tree* p, IP_PORT k) // вставка ключа k в дерево с корнем p
{
	if( !p ){return p=createavlt(k);}
	if ( k < p->key )
		p->left = insert(p->left,k);
	else
		p->right = insert(p->right,k);
	return balance(p);
}

AVL_tree* find(AVL_tree* p, IP_PORT k) // вставка ключа k в дерево с корнем p
{
	if( !p ) return NULL;
	if( k == p->key ) return p;
	else
		if (k < p->key)
			return find(p->left, k);
		else
			return find(p->right, k);
}

IP_PORT rightmost(AVL_tree *p) {
	while (p->right != NULL)
		p=p->right;
	return p->key;
}

AVL_tree* remove(AVL_tree* root,IP_PORT key) {
	if (NULL == root) return NULL;
	if (root->key == key) {
		if (NULL == root->left && NULL == root->right) {
			freem(root);
			return NULL;
		}
		if (NULL ==root->left && NULL != root->right) {
			AVL_tree *tmp=root->right;
			freem(root);
			return balance(tmp);
		}
		if (NULL !=root->left && NULL == root->right) {
			AVL_tree *tmp=root->left;
			freem(root);
			return balance(tmp);
		}
		root->key = rightmost(root->left);
		root->left = remove(root->left, root->key);
		return balance(root);
	}
	if (key < root->key) 
		root->left = remove(root->left, key);
	if (root->key < key)
		root->right = remove(root->right, key);
	return balance(root);
}

void deleteavlt(AVL_tree* p) {
	if(p->left != NULL) deleteavlt(p->left);
	if(p->right != NULL) deleteavlt(p->right);
	freem(p);
}