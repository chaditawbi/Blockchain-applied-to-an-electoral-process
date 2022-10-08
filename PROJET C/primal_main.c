#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "primalH.h"


/*Compare les temps d'execution entre modpow et modpow_naive*/
/*Pour executer: cf Makefile: changez PROGRAMS a run_primal_main et executez la commande suivante: make all && ./run_primal_main && make clean  */
int main()
{
	
	clock_t  temps_initial;
   	clock_t  temps_final;
	double temps_cpu2,temps_cpu3;
      	
	FILE *f=fopen("modpow.txt","w");
	
	for(int i=0;i<1000000;i=i+10000)
	{
      	//modpow naive
    	temps_initial = clock();
    	
		long s;
		
        s=modpow_naive(2,i,13);

    	temps_final = clock();
    	temps_cpu2 =((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		printf("numero d'iteration: %d |  Tps CPU: %10f \n", i, temps_cpu2);

		//modpow
    	temps_initial = clock();

		long s2;

    	s2=modpow(2,i,13);

    	temps_final = clock();
    	temps_cpu3 =((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		printf("numero d'iteration: %d |  Tps CPU: %10f \n", i, temps_cpu3);
		

		fprintf(f,"temps modpow naive: %f ,s: %lx \n",temps_cpu2,s);
		fprintf(f, " %d %f %f\n",i,temps_cpu2,temps_cpu3);
	}

	fclose(f);

	
	return 0;
}

