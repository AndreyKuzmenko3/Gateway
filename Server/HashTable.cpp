#include "stdafx.h"
#include "HashTable.h"
#include "Memory_manager.h"

int hash(IP_PORT key) {
	return key.IP%HASH_SIZE;
}
Hash_node* insert(Hash_node** hashtable, IP_PORT key) {
	Hash_node *p,*p0;
	int bucket = hash(key);
	p=(Hash_node*)alloc(16);
	p0=hashtable[bucket];
	hashtable[bucket]=p;
	p->next = p0;
	p->data = key;
	return p;
}
void remove(Hash_node** hashtable, IP_PORT key) {
	Hash_node *p,*p0;
	int bucket=hash(key);
	p0=0;
	p=hashtable[bucket];
	while (p && !(p->data == key)) {
		p0=p;
		p=p->next;
	}
	if(!p) return;
	if(p0) p0->next=p->next;
	else
		hashtable[bucket] = p->next;
	freem(p);
}
Hash_node* find(Hash_node** hashtable, IP_PORT key) {
	Hash_node *p;
	p=hashtable[hash(key)];
	while (p && !(p->data ==key)) p=p->next;
	return p;
}
void deleteht(Hash_node** ht) {
	Hash_node *p;
	Hash_node *p0;
	for(int i=0;i<HASH_SIZE;i++)
	{
		p=ht[i];
		while (p) {
			p0=p;
			p=p->next;
			freem(p0);
			p0=NULL;
		}
	}
	freem(ht);
}