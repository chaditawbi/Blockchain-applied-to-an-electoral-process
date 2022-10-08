#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "rsaH.h"
#include "primalH.h"
#include "blockH.h"
#include <assert.h>


/*allouer et initialiser un bloc*/
Block *creerBlock(Key *author, CellProtected *votes, unsigned char *hash, unsigned char *previous_hash, int nonce)
{
    Block *new =malloc(sizeof(Block));
    if (!new)
    {
        printf("Error function creerBlock: Erreur de malloc\n");
        return NULL;
    }
    unsigned char *h = (unsigned char*) malloc((strlen((char *) hash) + 1) * sizeof(unsigned char));
    unsigned char *ph = (unsigned char*) malloc((strlen((char *) previous_hash) + 1) * sizeof(unsigned char));
    int i;
    for (i=0; i < strlen((char *)hash); i++)
    {
        h[i] = hash[i];
    }
    h[i] = (unsigned char)'\0';
    for (i=0; i < strlen((char *)previous_hash); i++)
    {
        ph[i] = previous_hash[i];
    }
    ph[i] = (unsigned char)'\0';

    new->author = author;
    new->votes = votes;
    new->hash = h;
    new->previous_hash = ph;
    new->nonce = nonce;
    return new;
}

/*ecrire un bloc dans un fichier*/
void enregistrerBlock(char *filename, Block *block)
{
    FILE *f = fopen(filename,"w");
    if(f == NULL)
    {

        printf("Error function enregistrerBlock: file failed to open");
        return;
    }
    char *aut = key_to_str(block->author);

    //on écrit dans le fichier les données du block
    fprintf(f,"%s %s %s %d\n",aut, block->hash, block->previous_hash, block->nonce);
    CellProtected *voteList = block->votes;
    while (voteList)
    {
        char *str = protected_to_str(voteList->data);
        fprintf(f, "%s\n",str);
        voteList = voteList->next;
        free(str);
    }
    free(aut);
    fclose(f);
    return;
}


/*lire un bloc depuis un fichier*/
Block* lireBlock(char* filename)
{
	FILE *f = fopen(filename,"r");
    if (f == NULL)
    {
        printf("Error function lireBlock: Erreur a l'ouverture du fichier %s\n", filename);
        return NULL;
    }
    char buffer[2048];
    char authorStr[32];
    unsigned char hash[256];
    unsigned char previous_hash[256];
    int nonce;

    //Lecture de la premiere ligne
    if (fgets(buffer,4096,f) == NULL)
    {
        printf("Error function lireBlock: Erreur a la lecture de la 1ere ligne du fichier %s\n", filename);
        fclose(f);
        return NULL;
    } else {
        if (sscanf(buffer,"%s %s %s %d\n",authorStr,hash,previous_hash,&nonce) != 4)
        {
            printf("Error function lireBlock:Erreur de format\n");
            fclose(f);
            return NULL;
        }
    }

    //Lecture des votes
    CellProtected *votesTmp = NULL;
    while (fgets(buffer,4096,f) != NULL)   
    {
        Protected *pr = str_to_protected(buffer); 
        inserer_CellProtected(&votesTmp,pr);
    }

    fclose(f);

    CellProtected *votesTmpBis = votesTmp;
    CellProtected *votes = NULL;
    while (votesTmp)    
    {
        inserer_CellProtected(&votes,votesTmp->data);
        votesTmp = votesTmp->next;
    }

    //Liberation de votetmp mais pas son contenue
    CellProtected *tmp;
    while (votesTmpBis) 
    {
        tmp = votesTmpBis;
        votesTmpBis = votesTmpBis->next;
        free(tmp);
    }

    return creerBlock(str_to_key(authorStr),votes,hash,previous_hash,nonce);
}


/*Transforme un bloc en chaine de caractere*/
char *block_to_str(Block *block)
{
    if(!block)
    {
        printf("Error function block_to_str: block is empty\n");
    }

    char buffer[10000];
    buffer[0] = '\0';
    char previous_hash[32];
    char nonce[32];

    char *author = key_to_str(block->author);
    strcat(buffer,author);

    sprintf(previous_hash, " %s ", block->previous_hash);
    strcat(buffer,previous_hash);

    CellProtected *votes = block->votes;
    while (votes)
    {
        char *prStr = protected_to_str(votes->data);
        strcat(buffer,prStr);
        strcat(buffer, " ");
        free(prStr);
        votes = votes->next;
    }

    sprintf(nonce, "%d", block->nonce);
    strcat(buffer,nonce);

    free(author);
    
    return strdup(buffer);
}


/*fonction de hachage SHA256*/
unsigned char* hash_function_block(const char* str)
{
    if (!str)
    {
        fprintf(stderr, "Error function hash_function_block: str is empty\n");
        return NULL;
    }

    char *res = (char*) malloc(2*SHA256_DIGEST_LENGTH+1);
    char buffer[2*SHA256_DIGEST_LENGTH+1];
    unsigned char* d = SHA256((const unsigned char*) str, strlen(str), 0);
    res[0]='\0';
    buffer[0]='\0';
    //on transforme la chaine en écriture héxadécimal
    for (int i=0; i<SHA256_DIGEST_LENGTH; i++)
    {
        strcpy(buffer, res);
        sprintf(res,"%s%02x",buffer,d[i]);
        buffer[0] = '\0';
    }
    return (unsigned char*) res;
}

/*verifie si la valeur hachee des donnees du blocs commencent pas d zero successifs*/
int verify_block(Block *B, int d)
{

	for(int i=0;i<d;i++) 
    {
		if(B->hash[i] != '0') 
        {
			return 0;
		}
	}
  return 1;
}

/*compte le nombre de zeros*/
int count_zeros(unsigned char* str)
{
    if (!str)
    {
        printf("Error function count_zeros:  str is empty\n");
        return -1;
    }

    int nbZeros = 0;
    int taille_str = strlen((const char*) str);

    
    while ((nbZeros<taille_str) && (str[nbZeros]=='0'))
    {
        nbZeros++;
    }
    return nbZeros;
}

/*rend un bloc valide par bruteforce*/
void compute_proof_of_work(Block *B, int d)
{
	if (!B)
	{
		fprintf(stderr, "[compute_proof_of_work Function] B => NULL\n");
		return;
	}

	B->nonce = 0;
    char* str =block_to_str(B);
    unsigned char* hash = hash_function_block((const char*) str);

    //on incremente nonce et on recalcule hash avec la fonction SHA256 tant qu'il n'y a pas d zeros en tête de hash
    while (count_zeros(hash) < d)
    {
        free(str);
        free(hash);
        B->nonce ++;
        str = block_to_str(B);
        hash = hash_function_block((const char*) str);
    }
    free(str);

    free(B->hash); 
    B->hash = hash;
}

/*supprimation d'un bloc*/
void delete_block(Block* b)
{
    if (!b)
    {
        printf("Error function delete_block:block is null");
        return;
    }

    free(b->author);
    free(b->hash);
    free(b->previous_hash);
    free(b);

    return;
}
