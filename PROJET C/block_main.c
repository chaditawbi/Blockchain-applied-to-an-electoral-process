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

#define sizeC 20
#define sizeV 100


/*Ce main teste la fonctionalite des differentes fonctions de block et blockTree*/

/*Pour executer: cf Makefile: changez PROGRAMS a run_blockTree_main et executez la commande suivante: make all && ./run_blockTree_main && make clean  */

int main()
{
    srand(time(NULL));
    generate_random_data(sizeV,sizeC);

    
    Block *b,*b1,*b2,*b3,*b4,*b5;
    Key* pKey;
    unsigned char c1[4] ="001";
    unsigned char c2[4] ="100";
    unsigned char c3[4]="101";
    unsigned char c4[4]="111";
    unsigned char c5[4]="110";
    unsigned char c6[4]="010";
    



    CellKey *ckey=read_public_keys("keys.txt");
    CellProtected *c=read_protected("declarations.txt");
    pKey=ckey->data;

    b=creerBlock(pKey,c,c1,c2,1);
    b1=creerBlock(pKey,c,c3,c2,2);
    b2=creerBlock(pKey,c,c4,c5,3);
    b3=creerBlock(pKey,c,c6,c1,3);
    b4=creerBlock(pKey,c,c5,c6,2);
    b5=creerBlock(pKey,c,c2,c4,2);

    unsigned char c7[4]="000";

   Block *bf=creerBlock(pKey,c,c7,"\0",1);   
  

    compute_proof_of_work(b,1);
    
    compute_proof_of_work(b1,1);
    compute_proof_of_work(b2,1);
    compute_proof_of_work(b3,1);
    compute_proof_of_work(b4,1);
    compute_proof_of_work(b5,1);

    compute_proof_of_work(bf,1);

////////////////////////////////////////////////////////////////

    CellTree *bft,*bt,*bt1,*bt2,*bt3,*bt4,*bt5;
    bft=create_node(bf);
    bt1=create_node(b1);
    bt2=create_node(b2);
    bt3=create_node(b3);
    bt4=create_node(b4);
    bt5=create_node(b5);
    bt=create_node(b);

    add_child(bft,bt);
    add_child(bft,bt1);
    add_child(bt1,bt2);
    add_child(bt2,bt3);
    add_child(bt3,bt4);
    add_child(bt3,bt5);
    

    print_tree(bft);
   
 
    printf("\n\n");
    
    CellTree *bb=highest_child(bft);
    CellTree *last=last_node(bft);
    print_tree(last);
    printf("\n");
    print_tree(bb);
    
    CellProtected *cp=get_trusted_list_protected(bft);

    print_list_protected(cp);

    return 0;
}

