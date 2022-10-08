#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rsaH.h"
#include "primalH.h"
#include "DeclarationSecuriseH.h"
#include <string.h>
#define N 5000


/*initialiser une cle deja allouer*/
void init_key(Key* key, long val, long n)
{
	key->val=val;
	key->n=n;

}

/*initialise les valeurs des cles secrete et publique*/
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size)
{
	long p=random_prime_number(low_size,up_size, N);
  	long q=random_prime_number(low_size,up_size, N);
  	while(p==q)
	{
    		q=random_prime_number(low_size,up_size,N);
  	}
  	long n,s,u;
  	generate_key_values(p,q,&n,&s,&u);
  	//pour avoir des clés positives:
  	if (u<0)
	{
    		long t=(p-1)*(q-1);
    		u+=t; //on aura toujours s*u mod t=1
  	}

	init_key(pKey,s,n);
	init_key(sKey,u,n);
}


/*Transforme une cle en une chaine de caractere*/
char* key_to_str(Key* key)
{
    if(!key)
    {
        printf("Erreur key est NULL\n");
    }

    long value=key->val;
    long n=key->n;
	char* str=malloc(sizeof(char)*256);
    if(!str)
    {
        printf("Erreur de malloc,str is NULL\n");
    }
	sprintf(str,"(%lx,%lx)",value,n);

	return str;
}

/*Transforme une chaine de caractere en une cle*/
Key* str_to_key(char* str)
{
	long val;
    long n;
    if(!str)
    {
        printf("Erreur str is NULL\n");
    }
	sscanf(str,"(%lx,%lx)",&val,&n);

	Key *key=malloc(sizeof(Key));
    if(!key)
    {
        printf("Erreur de malloc key is NULL\n");
    }
	init_key(key,val,n);
	return key;
}


/*Fait une copie d'une cle*/
Key *copie_key(Key *key)
{
    Key *new =malloc(sizeof(Key));
	if(!new)
	{
		printf("Error function copie_key:Erreur de malloc\n");
	}
    new->val = key->val;
    new->n = key->n;
    return new;
}

/*alloue et initialise une signature avec un tableau de long*/
Signature* init_signature(long* content, int size)
{
    Signature *sgn=malloc(sizeof(Signature));
	if(!sgn)
	{
		printf("Error function init_signature: Erreur de malloc\n");
	}
    sgn->content=malloc(size*sizeof(long));
	if(!(sgn->content))
	{
		printf("Error function init_signature: Erreur de malloc\n");
	}

    sgn->size=size;

    for(int i=0;i<size;i++)
    {
        sgn->content[i]=content[i];
    }

    return sgn;
}

/*retourne une signature initialiser avec un message chiffrer avec une cle secrete*/
Signature* sign(char* mess, Key* sKey)
{
    long *crypted=encrypt(mess,sKey->val,sKey->n);
    int len=strlen(mess);
    Signature *s=init_signature(crypted,len);

    return s;
}

/*Transforme une signature en une chaine de caractere avec "#" entre chaque caractere*/
char *signature_to_str( Signature *sgn)
{
    char * result = malloc(10*sgn->size * sizeof(char));
    result [0]='#';
    int pos = 1;
    char buffer [156];
    for(int i =0; i < sgn->size;i++)
    {
        sprintf(buffer ,"%lx",sgn->content[i]) ;
        for(int j=0;j < strlen(buffer);j++)
        {
            result[pos]=buffer[j];
            pos = pos +1;
        }
        result[pos]='#';
        pos = pos+1;
    }
    result[pos] ='\0';
    result =realloc(result,(pos+1)*sizeof(char));
    return result;
}

/*Transforme une chaine de caractere en une signature*/
Signature *str_to_signature(char *str)
{
    int len = strlen ( str );
    long * content = ( long *)malloc(sizeof(long)*len);
    int num = 0;
    char buffer [256];
    int pos = 0;
    for (int i =0; i < len ; i ++)
    {
        if(str[i]!='#')
        {
            buffer[pos]=str[i];
            pos = pos +1;
        }
        else
        {
            if( pos != 0)
            {
                buffer[pos]='\0';
                sscanf(buffer,"%lx",&(content[num]));
                num = num + 1;
                pos = 0;
            }
        }
    }
    content = realloc(content,num*sizeof(long));

    return init_signature(content,num);
}

/*supprimer une signature*/
void delete_signature(Signature* sgn)
{
	if (!sgn)
	{
		printf("Error function delete_signature: sgn is empty\n");
		return;
	}
	free(sgn->content);
	free(sgn);
}

/*allouer et initialiser une declaration*/
Protected* init_protected(Key* pKey, char* mess,Signature* sgn)
{
    Protected *p=malloc(sizeof(Protected));
    if(!p)
    {
        printf("Fonction init_protected: Erreur de malloc p est NULL\n");
    }
    p->pKey=pKey;
    p->mess=strdup(mess);
    p->sgn=sgn;

    return p;
}

/*verifie si la signature dans pr correspond bien au meme message de pr*/
int verify(Protected* pr)
{

    if(!pr)
    {
        printf("Fonction Verify:Erreur pr est NULL\n");
    }


    char* decryptedmsg=decrypt(pr->sgn->content,pr->sgn->size,pr->pKey->val,pr->pKey->n);

    
    if(strcmp(pr->mess,decryptedmsg)==0)
    {
        free(decryptedmsg);
        return 1;
    }
    free(decryptedmsg);
    return 0;
}

/*Transforme une declaration en chaine de caractere*/
char* protected_to_str(Protected *pr)
{
    if(!pr)
    {
        printf("Fonction protected_to_str:Erreur pr is NULL\n");
    }
    char* keystr=key_to_str(pr->pKey);
    char* messStr=pr->mess;
    char* signstr=signature_to_str(pr->sgn);
    int len=strlen(keystr) + strlen(messStr) + strlen(signstr) + 2 + 1;


    char* result=malloc(sizeof(char)*len);

    sprintf(result,"%s %s %s",keystr,messStr,signstr);
	free(keystr);
	free(signstr);

    return result;
}

/*Transforme une chaine de caractere en declaration*/
Protected* str_to_protected(char* str)
{
    if(!str)
    {
        printf("Fonction str_to_protected:Erreur str is empty\n");
    }

    long val,n;
    char mess[256];
    char sgn[256];

    sscanf(str,"(%lx,%lx) %s %s",&val,&n,mess,sgn);


    Key* nKey=malloc(sizeof(Key));
    if(!nKey)
    {
        printf("Fonction str_to_protected: Erreur de malloc nKey\n");
        return NULL;
    }
    init_key(nKey,val,n);

    Signature *sig=str_to_signature(sgn);
    Protected *p=malloc(sizeof(Protected));
    if(!p)
    {
        printf("Fonction str_to_protected: Erreur de malloc p\n");
    }

    p->pKey=nKey;
    p->mess=strdup(mess);
    p->sgn=sig;

    return p;
}

/*supprime une declaration*/
void delete_protected(Protected* pr)
{
	if (!pr)
	{
		printf("Error function delete_protected: pr is empty\n");
		return;
	}

	if (pr->pKey)
		free(pr->pKey);
	
	if (pr->sgn)
		delete_signature(pr->sgn);

	if (pr->mess)
		free(pr->mess);

	free(pr);
	return;
}


/*simulation des donnees d'un scrutin de vote*/
void generate_random_data(int nv, int nc)
{

	if (nc > nv)
	{
		printf("Fonction generate_random_data: Erreur nc doit etre inferieur a nv\n");
		return;
	}

	FILE *f1 = fopen("keys.txt","w");
	FILE *f2 = fopen("candidates.txt","w");
	FILE *f3 = fopen("declarations.txt","w");

	if(f1 == NULL || f2 == NULL || f3==NULL)
	{
		printf("Fonction generate_random_data:erreur d'ouverture\n");
		return;
	}

	Key* pKey =malloc(sizeof(Key));
	Key* sKey =malloc(sizeof(Key));

	char** tabClesElecteurs =malloc(nv* sizeof(char*));
	for (int i = 0; i < nv; i++)
		tabClesElecteurs[i] = NULL;

	char** tabSKeyElecteurs = malloc(nv* sizeof(char*));
	for (int i = 0; i < nv; i++)
		tabSKeyElecteurs[i] = NULL;

	//Generation des couples de cles publiques/secretes des electeurs
	for (int i = 0; i < nv; i++)
	{
		init_pair_keys(pKey, sKey, 3, 7);

		char* StrPKeyElecteur = key_to_str(pKey);

		char* StrSKeyElecteur = key_to_str(sKey);

		tabClesElecteurs[i] = strdup(StrPKeyElecteur);

		tabSKeyElecteurs[i] = strdup(StrSKeyElecteur);

		fprintf(f1,"%s %s\n", StrPKeyElecteur, StrSKeyElecteur);
		free(StrPKeyElecteur);
		free(StrSKeyElecteur);

		printf("Compteur = %d\n", i);
	}

	char** tabClesCand =malloc(nv * sizeof(char*));
	for (int i = 0; i < nv; ++i)
		tabClesCand[i] = NULL;


	//Selection de nc cle publique pour definir les nc candidats parmis les nv citoyens
	for(int j = 0; j < nc; j++)
	{
		int random = (rand() % nv);

		while (tabClesCand[random] != NULL) //Tant que cette case n'est pas nulle on en cherche une autre...
		{
			random = rand() % nv;
		}

		//Candidat choisi d'index random <=> Citoyen d'index random
		tabClesCand[random] = strdup(tabClesElecteurs[random]);

		//Maj du fichier
		fprintf(f2, "%s\n", tabClesCand[random]);
	}


	char** tabCleCand2 =malloc(nv * sizeof(char*));
	for (int i = 0; i < nv; ++i)
		tabCleCand2[i] = NULL;

	int tabCleCandindex2 = 0;
	for (int k = 0; k < nv; k++)
	{
		if (tabClesCand[k] == NULL)
		{
			printf("%d is NULL\n", k);
		}
		else
		{
			printf("%s\n",tabClesCand[k]);

			tabCleCand2[tabCleCandindex2++] = strdup(tabClesCand[k]);
		}
	}

	//Generation des declarations pour chaque citoyen
	for (int i = 0; i < nv; i++)
	{
		Key* pKeyElec = str_to_key(tabClesElecteurs[i]);
		Key* sKeyElec = str_to_key(tabSKeyElecteurs[i]);

		int randCandidate = rand() % nc;


		Signature* sgn = sign(tabCleCand2[randCandidate], sKeyElec);


		Protected* pr = init_protected(pKeyElec, tabCleCand2[randCandidate], sgn);

		char* prCitoyen = protected_to_str(pr);

		fprintf(f3, "%s\n", prCitoyen);


		char* pKeyPr = key_to_str(pr->pKey);

		if(verify(pr)==1)
		{
			printf("Verification du message: %s, de pKey: %s, est valide? Oui\n", pr->mess, pKeyPr);
		}
		else
		{
			printf("Verification du message: %s, de pKey: %s, est valide? Non\n", pr->mess, pKeyPr);
		}

		free(pKeyPr);

		free(pKeyElec);
		free(sKeyElec);
		free(prCitoyen);
	}

	for(int i = 0; i < nc; i++)
	{
		free(tabCleCand2[i]);
	}
	free(tabCleCand2);

	for(int i = 0; i < nv; i++)
	{
		free(tabClesCand[i]);
	}
	free(tabClesCand);

	for(int j = 0; j < nv; j++)
	{
		free(tabClesElecteurs[j]);
	}
	free(tabClesElecteurs);

	for(int k = 0; k < nv; k++)
	{
		free(tabSKeyElecteurs[k]);
	}
	free(tabSKeyElecteurs);

	free(pKey);
	free(sKey);

	fclose(f1);
	fclose(f2);
	fclose(f3);
}

