#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "rsaH.h"
#include "primalH.h"
#include "voteLCH.h"


/*creer une cellule cellKey vide*/
CellKey* create_cell_key(Key* key)
{
    CellKey* ck=malloc(sizeof(CellKey));
    if(ck==NULL)
    {
        printf("Erreur d'allocation");
        return NULL;
    }
    if(key==NULL)
    {
        printf("Erreur");
        return NULL;
    }
    ck->data=key;
    ck->next=NULL;
    return ck;
}

/*insere une cellule a la tete de la liste*/
void inserer_cellKey(CellKey** LCK, Key* key)
{
    CellKey* ck=create_cell_key(key);
    if(*LCK==NULL)
    {
        *LCK=ck;
        return;
    }
    ck->next=*LCK;
    *LCK=ck;
}


/*cree une liste de toutes les cles d'un fichier*/
CellKey* read_public_keys(char* filename)
{
    if (strcmp(filename, "keys.txt") != 0 && strcmp(filename, "candidates.txt") != 0)
	{
		printf("Fonction read_public_keys: Erreur de fichier: %s\n", filename);
		return NULL;
	}

	FILE* file = fopen(filename, "r");
	if (!file)
	{
		printf("Fonction read_public_keys: Erreur ouverture fichier: %s\n", filename);
		return NULL;
	}

	char buffer[256];
	long p,n;

	CellKey* cellKey = NULL;

	int count = 0;

    while (fgets(buffer, 256, file))
    {
        sscanf(buffer, "(%lx,%lx)", &p, &n);
        Key* publicKey =malloc(sizeof(Key));
        init_key(publicKey, p, n);

   	    if (!cellKey)
   	    {
   		    cellKey = create_cell_key(publicKey);
   	    } 
        else 
        {
   		    inserer_cellKey(&cellKey, publicKey);
   	    }
   	    count++;
   	    //printf("%d\t",count);
    }
    
    printf("total: %d\n",count);
    fclose(file);
    return cellKey;
}


/*affiche la liste des cles*/
void print_list_keys(CellKey* LCK)
{
    if(LCK==NULL)
    {
        printf("Error function print_list_keys: LCK is empty\n");
        return;
    }

    CellKey *ptr=LCK;
    while(ptr!=NULL)
    {
        Key* k=ptr->data;
        //printf("%lx\n",ptr->data->val);
        printf("(%lx, %lx)\n", k->val, k->n);
        ptr=ptr->next;
    }
  printf("\n");
}

/*supprimation d'une seul cellule*/
void delete_cell_key(CellKey* c)
{
    if(!c)
    {
        printf("Error function delete_cell_key: c is empty\n");
    }
    free(c->data);
    free(c);
}

/*Supprimation de toute la liste de cle*/
void delete_list_keys(CellKey* c)
{
    if(!c)
    {
        printf("Function delete_list_keys:c already empty\n");
    }
    while(c)
    {
        CellKey* tmp=c->next;
        delete_cell_key(c);
        c=tmp;
    }
}

/*retourne le nombres d'elements de la liste de cles*/
int list_keys_length(CellKey *c)
{

    if (!c)
    {
        return 0;
    }
    return 1 + list_keys_length(c->next);
}


/*alloue et creer une cellule de declaration*/
CellProtected* create_cell_protected(Protected* pr)
{
    CellProtected *p=malloc(sizeof(CellProtected));
    if (!p)
    {
        printf("Error function create_cell_protected: Erreur de malloc\n");
        return NULL;
    }

    p->data=pr;
    p->next=NULL;
    return p;
}

/*insere a la tete de la liste une cellule d'une declaration*/
void inserer_CellProtected(CellProtected** cpr, Protected* data)
{
    //Si cpr est vide alors ajouter quand meme une declaration a la tete
    if (cpr==NULL)
    {
        *cpr=create_cell_protected(data);
        return;
    }
    CellProtected *p=create_cell_protected(data);
    p->next=*cpr;
    *cpr=p;
}


/*cree une liste de toutes les declarations d'un fichier*/
CellProtected* read_protected(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		printf("Error function read_protected:Erreur d'ouverture du fichier %s\n", filename);
		return NULL;
	}

	char buffer[256];

	CellProtected* cellProtected = NULL;

    while (fgets(buffer, 256, file))
    {
        Protected* newProtected = str_to_protected(buffer);

   	    if (!cellProtected)
   	    {
   		    cellProtected = create_cell_protected(newProtected);
   	    } 
        else 
        {
   		    inserer_CellProtected(&cellProtected, newProtected);
   	    }
    }

    fclose(file);
   
	return cellProtected;
}

/*affiche la liste de declarations*/
void print_list_protected(CellProtected *lcp)  
{
    if(!lcp)
    {
        printf("Error function print_list_protected: lcp is empty");
        return;
    }
    char *prStr;
    while (lcp) 
    {
        prStr = protected_to_str(lcp->data);
        printf("%s\n",prStr);
        free(prStr);
        lcp = lcp->next;
    }
}


/*supprimation d'une seul cellule d'une declaration*/
void delete_cell_protected(CellProtected* c)
{
    if(c!=NULL)
    {
        free(c->data->pKey);
        free(c->data->mess);
        free(c->data->sgn);
        free(c->data);
        free(c);
    }
    else
    {
        printf("cell Vide\n");
    }
}

/*Supprimation de toutes la liste de declarations*/
void delete_list_protected(CellProtected* c)
{
    while(c!=NULL)
    {
        CellProtected* tmp=c->next;
        delete_cell_protected(c);
        c=tmp;
    }
}


/*fusione deux listes de declarations*/
CellProtected *fusionner_list_protected(CellProtected *votes1, CellProtected *votes2)
{

    if (!votes1)
    {
        return votes2;
    }
    if(!votes2)
    {
        return votes1;
    }

    CellProtected *ptr = votes1;

    //On parcour la liste jusqu'a la fin
    while (ptr->next)
    {
        ptr = ptr->next;
    }
    //dernier element de la 1er liste pointe vers le premier element de la 2eme liste donc fusion
    ptr->next = votes2;
    return votes1;
}

/*Fait une copie d'une liste de declarations*/
CellProtected *copie_list_protected(CellProtected *votes)   
{
    
    if (!votes) 
    {
        return NULL;
    }
    CellProtected *copie = NULL;

    while (votes)  
    {
        inserer_CellProtected(&copie,votes->data);
        votes = votes->next;
    }
    return copie;
}