#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rsaH.h"
#include "primalH.h"
#include <string.h>
#include <time.h>

/*Utilisation et test du protcole RSA*/
/*/*Pour executer: cf Makefile: changez PROGRAMS a run_rsa_main et executez la commande suivante: make all && ./run_rsa_main && make clean  */

int main()
{
	srand(time(NULL));
	
  	//generation de cle:
  	long p=random_prime_number(3,7, 5000);
  	long q=random_prime_number(3,7, 5000);
  	while(p==q)
	{
    		q=random_prime_number(3,7, 5000);
  	}
  	long n,s,u;
  	generate_key_values(p,q,&n,&s,&u);
  	//pour avoir des clés positives:
  	if (u<0)
	{
    		long t=(p-1)*(q-1);
    		u+=t; //on aura toujours s*u mod t=1
  	}

  	//affichage des cles en hexadecimal
  	printf (" cle publique= (%lx , %lx ) \n",s,n);
  	printf (" cle privee= (%lx , %lx ) \n",u,n);

  	//chiffrement
  	char mess[10] ="Hello"; //TESTEZ ICI DIFFERENTS MESSAGES
  	int len=strlen(mess);
  	long *crypted = encrypt(mess, s, n);

  	printf("Initial message: %s \n",mess);
  	printf("Encoded representation : \n");
  	print_long_vector(crypted,len);

  	//dechiffrement
  	char* decoded=decrypt(crypted,len,u,n);
  	printf("Decoded:%s\n",decoded);
  
  	return 0;
}
