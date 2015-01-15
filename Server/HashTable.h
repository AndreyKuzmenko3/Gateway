#include "IP_PORT.h"
#define HASH_SIZE 50

struct Hash_node {
	Hash_node* next;
	IP_PORT data;
};
Hash_node* insert(Hash_node**, IP_PORT);
void remove(Hash_node**, IP_PORT);
Hash_node* find(Hash_node**, IP_PORT);
int hash(IP_PORT);
void deleteht(Hash_node**);