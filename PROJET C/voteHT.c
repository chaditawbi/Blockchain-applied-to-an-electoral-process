#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "rsaH.h"
#include "primalH.h"
#include "voteHTH.h"


/*supprime toute les declaration dont les signatures sont non valide*/
void fraude(CellProtected** LCP)
{
    CellProtected* cour = *LCP;
    CellProtected* last = NULL;
    CellProtected* next = NULL;
    CellProtected* trash = NULL;
    while (cour) 
    {
        next = cour->next;
        if (!verify(cour->data))
        {
            if (last == NULL) 
            {
                *LCP = next;
                trash = cour;
                cour = cour->next;
                delete_cell_protected(trash);
            }
            else if (next == NULL) 
            {
                last->next = NULL;
                trash = cour;
                cour = cour->next;
                delete_cell_protected(trash);
            }
            else 
            {
                last->next = next;
                trash = cour;
                cour = cour->next;
                delete_cell_protected(trash);
            }
        }
        else 
        {
            last = cour;
            cour = cour->next;
        }
    }
}


/*alloue et initialise une cellule hashCell*/
HashCell* create_hashcell(Key* key)
{
    HashCell* hc=malloc(sizeof(HashCell));
	if(!hc)
	{
		printf("Error function create_hashcell: Erreur de malloc\n");
	}
    hc->key=key;
    hc->val=0;
    return hc;
}

/*fonction de hash pour calculer l'index d'une cellule*/
int hash_function(Key* key,int size)
{
    int v=key->val;
    int n=key->n;
    return (v+1)*(n+1)%size;
}

/*supprimation d'une table de Hash*/
void delete_hashtable(HashTable* t)
{
    for(int i=0;i<t->size; i++)
    {
        if(t->tab[i])
        {
            free(t->tab[i]->key);
            free(t->tab[i]);
        }
    }
    free(t->tab);
    free(t);
}

  
/*cherche la position d'une cle*/
int find_position(HashTable* t, Key* key)
{
	if(t == NULL || key == NULL)
	{
		printf("Error function find_position: table or key is empty\n");
		return -1;
	}

	int index = hash_function(key, t->size);

	for (int i = 0;i<t->size; i++)
	{
		if (t && t->tab[i] && (t->tab[i]->key->val == key->val) && (t->tab[i]->key->n == key->n))
		{
			return i;
		}
	}

	return index;
}


/*creation de la table de Hash*/
HashTable* create_hashtable(CellKey* keys, int size)
{
	if (!keys)
	{
		printf("Error function create_hashtable: keys is empty\n");
		return NULL;
	}

	CellKey* ptr = keys;
	HashCell** tab =malloc(sizeof(HashCell*) * size);
	if (!tab)
	{
		printf("Error function create_hashtable: Erreur de malloc\n");
		return NULL;
	}

	//On initialise le tableau a NULL
	for (int i = 0; i < size; i++)
		tab[i] = NULL;


	HashTable* HT =malloc(sizeof(HashTable));
	if(!HT)
	{
		printf("Error function create_hashtable: Erreur de malloc\n");
		return NULL;
	}
	HT->size = size;
	HT->tab = tab;


	while (ptr)
	{
		Key* pKey= malloc(sizeof(Key));
		if(!pKey)
		{
			printf("Error function create_hashtable: pKey is empty\n");
			return NULL;
		}
		pKey->val = ptr->data->val;
		pKey->n = ptr->data->n;

		HashCell* cellvotant = create_hashcell(pKey);

		//Inserer dans ca position dans la table
		int hashindex = find_position(HT, pKey);
		if (tab[hashindex] == NULL)
		{
			tab[hashindex] = cellvotant;
		} 
		else
		{
			while(tab[hashindex] != NULL)
			{
				if (hashindex >= (size-1))
				{
					hashindex = 0;
				}
				hashindex++;
			}

			tab[hashindex] = cellvotant;
		}

		ptr = ptr->next;
	}

	return HT;
}

/*comptabilisation des votes et retourne la cle publique du gagnant*/
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV)
{
	

	//Table de Hachage des Voteurs
	HashTable* HV = create_hashtable(voters, sizeV*2);

	//Table de Hachage des Candidats
	HashTable* HC = create_hashtable(candidates, sizeC*2);

	//pointeur sur la tete de la liste
	CellProtected* ptr=decl;

	//suprimmer toute les declarations dont les signatures sont non valide
	fraude(&decl);

	//Parcourir la liste de declaration
	while(ptr)
	{

    	//Verifie si le votant a deja voter
    	int indexV = find_position(HV, ptr->data->pKey);

    	//Candidat selectioner par le votant
    	Key* KeyC = str_to_key(ptr->data->mess);

 		int indexC = find_position(HC, KeyC);

		//Vérification si la déclaration actuelle porte sur un candidat valide
		if (HC->tab[indexC] == NULL)
		{
    		printf("Vote sur un candidat non déclaré\n");
    		continue;
    	}

    	if(HV->tab[indexV] == NULL && HV->tab[indexV]->val > 0)
    	{
    		printf("Citoyen idx a déjà voté: %d\n", indexV);
    			continue;
    	}

		//Update de val a +1 car il vient de voté
    	(HV->tab[indexV]->val)++; 

		//Ajouter un vote au candidat
    	(HC->tab[indexC]->val)++; 

    	free(KeyC);
			
		ptr=ptr->next;
  	}

  	//Chercher le gagnant dans la table candidat
  	int winnerindex = 0;
  	int max = 0;
  	for (int i = 0; i<HC->size;i++)
  	{
  		if (HC->tab[i] && HC->tab[i]->val > max)
  		{
  			max = HC->tab[i]->val;
  			winnerindex = i;
  		}
  	}

  	Key* winner =malloc(sizeof(Key));

  	init_key(winner, HC->tab[winnerindex]->key->val, HC->tab[winnerindex]->key->n);
  	char* winnerstr = key_to_str(winner);
  	printf("Le winner est %s avec %d voies!\n", winnerstr, max);

  	delete_hashtable(HV);
  	delete_hashtable(HC);
	free(winnerstr);

  	return winner;
}

