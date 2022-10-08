#ifndef voteHTH_H
#define voteHTH_H
#include "voteLCH.h"

typedef struct hashcell 
{ 
    Key* key;
    int val;
}HashCell;

typedef struct hashtable 
{
    HashCell** tab;
    int size;
}HashTable;

void fraude(CellProtected** LCP);
HashTable* create_hashtable(CellKey* keys,int size);
HashCell* create_hashcell(Key* key);
int hash_function(Key* key,int size);
int find_position(HashTable *t ,Key* key);
void delete_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);




#endif