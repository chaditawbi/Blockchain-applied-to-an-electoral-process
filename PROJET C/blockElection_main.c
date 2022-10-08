#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "rsaH.h"
#include "primalH.h"
#include "blockH.h"
#include "blockTreeH.h"
#define NBVoteurs 1000
#define NBCandidats 2
#define NB_Bits 2

/*Simulation de la blockchain*/
/*Pour executer: cf Makefile: changez PROGRAMS a run_blockElection_main et executez la commande suivante: make all && ./run_blockElection_main && make clean  */
int main()
{
    srand(time(NULL));
 	fprintf(stdout,"\nLancement d'une generation de clés publiques/privées pour %d Citoyen(s) et %d Candidat(s)\n", NBVoteurs,NBCandidats);
 	sleep(2);
 	generate_random_data(NBVoteurs, NBCandidats);
 	fprintf(stdout,"\n\nLa génération est terminée pour %d Citoyen(s) et %d Candidat(s)\n", NBVoteurs,NBCandidats);
 	sleep(2);
    fprintf(stdout,"\nLecture des fichiers 'candidates.txt', 'keys.txt' et 'declarations.txt'\n");
    sleep(2);

    CellKey *candidates = read_public_keys("candidates.txt");
    CellProtected *votes = read_protected("declarations.txt");
    CellKey *publicKeys = read_public_keys("keys.txt");
    
    fprintf(stdout,"\nLecture des fichiers 'candidates.txt', 'keys.txt' et 'declarations.txt' terminée\n");
    sleep(2);

    //soumission de tous les votes et rajout dans l'arbre
    fprintf(stdout, "\nSoumission des votes avec generation d'un block tous les 10 votes soumis\n");
    sleep(2);

    CellTree *tree = NULL;
    int i, nbFichier = 1;
    int votesParBlock = 10;
    char nomFichier[256];
    CellProtected *ptr = votes;
    Protected *pr;
    Key *cleAssesseur = NULL;

    while(ptr)
    {
        //La cle assesseur est la 1ere a voter
        cleAssesseur = copie_key(ptr->data->pKey); 

        //On cree Pending_votes.txt en soumettant le bon nombre de votes
        i = 0;
        while(ptr && i<votesParBlock)
        {
            pr = ptr->data;
            submit_vote(pr);
            ptr = ptr->next;
            i++;
        }
        //On cree un block a partir de Pending_votes.txt et on ecrit le block dans Pending_block.txt
        create_block(&tree, cleAssesseur, NB_Bits); 
        sprintf(nomFichier, "ficher%d", nbFichier);
        //On ajoute le contenu du fichier Pending_block.txt au repertoire Blockchain
        add_block(NB_Bits, nomFichier); 
        nbFichier++;

		fprintf(stdout,"Ajout des blocks a la blockchain...\n(%d/%d)\n", nbFichier, (NBVoteurs/votesParBlock)+1);
    }

    fprintf(stdout,"\nAjout des blocks terminee\n");
    sleep(2);

    //Lecture du repertoire Blockchain, re-creation et affichage de l'arbre
    tree = read_tree();
    fprintf(stdout, "\nAffichage de l'arbre provenant du répertoire ./Blockchain/\n");
    sleep(2);
    print_tree(tree);

    printf("\n\n");
    Key *gagnant = compute_winner_BT(tree, candidates, publicKeys, NBCandidats*2, NBVoteurs*2);

    char *gagnantstr = key_to_str(gagnant);
    printf("\nLe gagnant de l'election est %s\n\n", gagnantstr);
    free(gagnantstr);



    //delete tout les fichiers dans le repertoire Blockchain apres la fin
    DIR *theFolder = opendir("./Blockchain/");
    struct dirent *next_file;
    char filepath[256];

    while ( (next_file = readdir(theFolder)) != NULL )
    {
        // build the path for each file in the folder
        sprintf(filepath, "%s/%s", "./Blockchain/", next_file->d_name);
        remove(filepath);
    }
    closedir(theFolder);

    
    free(gagnant);
    delete_tree(tree);
    
    delete_list_protected(votes);
    delete_list_keys(candidates);
    delete_list_keys(publicKeys);
    
 	return 0;
}

