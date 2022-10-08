#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rsaH.h"
#include "primalH.h"
#include <string.h>
#define N 5000


/*renvoie le plus grand PGCD entre s et t*/
long extended_gcd(long s,long t,long *u,long *v)
{
	if (s==0)
	{
		*u=0;
    	*v=1;
    	return t;
  	}
  	long uPrim, vPrim ;
  	long gcd=extended_gcd(t%s,s,&uPrim,&vPrim);
  	*u=vPrim-(t/s)*uPrim;
  	*v=uPrim;
	return gcd;

}


/*genere les valeurs des cles publiques (s,n) et secrete(u,n)*/
void generate_key_values(long p,long q, long *n, long *s,long *u)
{
	*n=p*q;
	long t=(p-1)*(q-1);
  	*s=rand_long(0,t);
  	long v=0;
  	while(extended_gcd(*s,t,u,&v)!=1)
	{
    		*s=rand_long(0,t);
  	}
	
}

/*Chiffre avec la cle publique et modpow une chaine de caractere et renvoie un tableau de long*/
long* encrypt(char* chaine, long s, long n)
{
	long *tab=malloc(sizeof(long)*strlen(chaine));
    if(!tab)
	{
      	fprintf(stderr,"erreur allocation\n");
      	return NULL;
    }
    
    for (int i=0;i<strlen(chaine); i++)
	{
	
      	tab[i]=modpow((long)chaine[i],s,n);
      		
    }
	return tab;
}



/*Dechiffre avec la cle secrete et modpow et retourne la chaine de caractere*/
char* decrypt(long* crypted, int size, long u, long n) 
{
    char* msg = (char*)malloc(sizeof(char)*size);
	if(!msg)
	{
		printf("Error function decrypt:Erreur de malloc\n");
	}
    for (int i = 0; i < size; i++) 
	{
        msg[i] = (char)modpow(crypted[i], u, n);
    }
    msg[size] = '\0';
    return msg;
}

/*affiche un tableau de long*/
void print_long_vector(long *result , int size)
{ 
	printf ("Vector: [ "); 
  	for(int i =0; i < size ; i ++)
	{
    		printf("%lx \t", result[i]);
  	} 
  	printf ("] \n");
}



























