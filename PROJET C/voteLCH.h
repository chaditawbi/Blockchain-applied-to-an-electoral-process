#ifndef voteLCH_H
#define voteLCH_H
#include "DeclarationSecuriseH.h"

typedef struct cellKey 
{
    Key* data;
    struct cellKey* next;
}CellKey;

typedef struct cellProtected
{
    Protected* data;
    struct cellProtected * next;
}CellProtected;

CellKey* create_cell_key(Key* key);
void inserer_cellKey(CellKey**LCK, Key* key);
CellKey* read_public_keys(char* nomFichier);
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* c);
int list_keys_length(CellKey *c);

CellProtected* create_cell_protected(Protected* pr);
void inserer_CellProtected(CellProtected** cpr, Protected* data);
//CellProtected* read_protected();
CellProtected* read_protected(char* filename);
void print_list_protected(CellProtected* lcp);
void delete_cell_protected(CellProtected* c);
void delete_list_protected(CellProtected* c);
CellProtected *fusionner_list_protected(CellProtected *votes1, CellProtected *votes2);
CellProtected *copie_list_protected(CellProtected *votes);

#endif