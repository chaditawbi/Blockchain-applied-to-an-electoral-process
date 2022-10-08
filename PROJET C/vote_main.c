#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "rsaH.h"
#include "primalH.h"
#include "DeclarationSecuriseH.h"
#include "voteLCH.h"
#include "voteHTH.h"
#define sizeC 2
#define sizeV 10000

/*Simulation d'un scrutin de vote et computation du gagnant*/
/*Pour executer: cf Makefile: changez PROGRAMS a run_vote_main et executez la commande suivante: make all && ./run_vote_main && make clean  */

int main()
{
	srand(time(NULL));
	generate_random_data(sizeV,sizeC);
	
	CellKey *listElecteur=read_public_keys("keys.txt");
	CellKey *listCandidats=read_public_keys("candidates.txt");
	CellProtected *listDeclaration=read_protected("declarations.txt");

	//print_list_protected(listDeclaration);
	Key *winner;

	winner=compute_winner(listDeclaration,listCandidats,listElecteur,sizeC,sizeV);

	return 0;
}