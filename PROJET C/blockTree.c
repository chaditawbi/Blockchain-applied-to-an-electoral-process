#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "rsaH.h"
#include "primalH.h"
#include "blockTreeH.h"
#include <assert.h>
#include <dirent.h>


/*creer et initialiser un node de l'arbre*/
CellTree* create_node(Block* b)
{

    if(b==NULL)
    {
        printf("Error function create_node: block is empty\n");
    }

    CellTree *bt=malloc(sizeof(CellTree));
    if(bt==NULL)
    {
        printf("Error function create_node: Error of malloc\n");
    }
    bt->block=b;
    bt->father=NULL;
    bt->firstChild=NULL;
    bt->nextBro=NULL;
    bt->height=0;

    return bt;
}

/*update l'hauteur des noeuds a chaque modification*/
int update_height(CellTree* father, CellTree* child)
{

    if ((father == NULL)||(child == NULL))
    {
        printf("Error function update_height:father or child is empty\n");
    }
    if (child->father != father)
    {
        printf("Error function update_height:father is not the father of the child\n");
    }
    if (father->height<child->height+1)
    {
        father->height = child->height+1;
        return 1;
    }
    else
    {
        //on ne modifie pas le père
        return 0;
    }
}

/*ajouter un fils et update l'hauteur*/
void add_child(CellTree *father, CellTree *child)
{
    if((father == NULL) || (child == NULL))
    {
        printf("Error function add_child: father or child is empty\n");
        return;
    }
    //On actualise le pere de child
    child->father = father;
    //on ajoute le fils
    CellTree *ptr = father->firstChild;
    if(ptr == NULL)
    {
        father->firstChild = child;
    }
    else
    {
        //on cherche le dernier des frères du fils du père
        while (ptr->nextBro)
        {
            ptr=ptr->nextBro;
        }

        ptr->nextBro =child;
    }

    //on met à jour la hauteur des pères tant qu'il y a des modifications
    CellTree *fathers = father;
    CellTree *children = child;
    int modification = 1;
    while((fathers)&&(modification==1))
    {
        modification = update_height(fathers,children);
        children = fathers;
        fathers = fathers->father;
    }

}

/*affiche un arbre*/
void print_tree(CellTree *tree)
{
    if(!tree)
    {
        printf("tree is empty\n");
        return;
    }
    //on affiche le noeud courrant
    printf("Block de hauteur : %d et d'identifiant: %s\n", tree->height, tree->block->hash);

    //on appelle la fonction pour ses frères puis ses fils
    if(tree->nextBro)
    {
        print_tree(tree->nextBro);
    }
    if (tree->firstChild)
    {
        print_tree(tree->firstChild);
    }
}

/*supprime un node de l'arbre*/
void delete_node(CellTree *node)
{
    if(!node)
    {
        printf("error function delete_node:node is empty\n");
    }
    delete_block(node->block);
    free(node);
}

/*supprime tout l'arbre*/
void delete_tree(CellTree* tree)
{
	if(tree)
    {
        CellTree *brothers = tree->nextBro;
        CellTree *children =  tree->firstChild;

        //on supprime le noeud courant
        delete_node(tree);

        //on supprime ses frères puis ses fils
        delete_tree(brothers);
        delete_tree(children);
    }

	return;
}


/*retourne le fils avec la plus grande hauteur*/
CellTree* highest_child(CellTree* cell)
{
    //retourne l'adresse du fils dont la hauteur est la plus grande
    if (cell==NULL)
    {
        printf("Error function highest_child : tree is empty\n");
        return NULL;
    }

    CellTree* child = cell->firstChild;
    CellTree* highest_child = cell->firstChild;
    while(child)
    {
        if (child->height > highest_child->height)
        {
            highest_child = child;
        }
        child = child->nextBro;
    }
    return highest_child;
}


/*retourne la feuille de la plus grande branche*/
CellTree *last_node(CellTree *tree)
{
    
    if (tree == NULL)
    {
        return NULL;
    }
    //renvoie une feuille
    if (tree->firstChild == NULL)
    {
        return tree;

    }
    //parcourt le plus grand fils
    else
    {
        last_node(highest_child(tree));
    }
}

/*fusionne les listes de votes de la plus longue branche et retourne tout la liste*/
CellProtected *get_trusted_list_protected(CellTree *tree)
{
    
    if (!tree)
    {
        printf("Error function get_trusted_list_protected: tree is empty\n");
        return NULL;
    }

    CellTree *node = last_node(tree);
    CellProtected *res = NULL;
    while (node != NULL)
    {
        res = fusionner_list_protected(res,copie_list_protected(node->block->votes));
        node = node->father;
    }
    return res;
}

/*ajoute un vote a la fin du fichier Pending_votes.txt*/
void submit_vote(Protected *p)
{
    FILE *f = fopen("Pending_votes.txt","a");
    if (!f)
    {
        printf("Error function submit_vote: Erreur d'ouverture du fichier Pending_votes.txt\n");
        return;
    }
    char *vote = protected_to_str(p);
    fprintf(f,"%s\n",vote);
    free(vote);
    fclose(f);
}


/*Creation d'un bloc et son ajout dans Pending_block*/
void create_block(CellTree **tree, Key *author, int d)
{
    
    //Creation d'un bloc valide a partir de Pending_votes.txt
    CellProtected *votes = read_protected("Pending_votes.txt");

    CellTree *leaf = last_node(*tree);
    unsigned char previous_hash[2*SHA256_DIGEST_LENGTH+1];
    int i;
    //Genesis Block (1er bloc de la blockchain)
    if (leaf == NULL)   
    {  
        previous_hash[0] = '0';
        previous_hash[1] = '\0';
    }
    else
    {
        for (i=0; i<(2*SHA256_DIGEST_LENGTH+1); i++)
        {
            previous_hash[i] = leaf->block->hash[i];
        }
        previous_hash[i] ='\0';

    }
    Block *b = creerBlock(author,votes,(unsigned char *)"0",previous_hash,0);
    compute_proof_of_work(b,d);
    CellTree *newnode = create_node(b);
    //On gere le Genesis Block de la chaine
    if (leaf == NULL)   
    {
        *tree = newnode;
    } 
    else 
    {
        add_child(leaf,newnode); 
    }

    assert(remove("Pending_votes.txt") == 0);
    enregistrerBlock("Pending_block.txt", b);
    
}


/*Ajout d'un block au repertoir Blockchain*/
void add_block(int d, char *name)
{
    Block *b = lireBlock("Pending_block.txt");
    if (!b)
    {
        printf("Error function add_block: b is empty\n");
        return;
    }

    int verified = verify_block(b, d);
    char path[256];
    if (verified)
    {
        strcpy(path,"./Blockchain/");
        strcat(path,name);
        enregistrerBlock(path, b);
    }
    assert(remove("Pending_block.txt") == 0);
    free(b->author);
    free(b->hash);
    free(b->previous_hash);
    delete_list_protected(b->votes);
    free(b);
}






/*Creation de l'arbre de bloc a partir du repertoire Blockchain*/
CellTree * read_tree()
{
    //On initialise size à -2 pour ne pas compter le début et la fin du répertoire ("." et "..")
    int size = - 2; 

    //Détermination de la taille du repertoire
    DIR * dir = opendir("./Blockchain/");
    if (dir != NULL) 
    {
        struct dirent * entry;
        while ((entry = readdir(dir)) != NULL)
        {
            size++;
        }
    closedir(dir);
    } 

    //Creation du tableau T contenant les noeuds
    CellTree ** T = (CellTree **) malloc(sizeof(CellTree *) * size);
    if (T == NULL)
    {
        printf("Erreur malloc CellTree\n");
        exit(EXIT_FAILURE);
    }

    //Initialisation des cases de T à NULL
    int i;
    for (i = 0; i < size; i++) 
    {
        T[i] = NULL;
    }

    Block * block;

    //Stockage des noeuds dans T
    const char nom_rep[14] = "./Blockchain/";
    char nom_fic[256];

    DIR * dir2 = opendir("./Blockchain/");
    if (dir2 != NULL) 
    {
        struct dirent * entry2;
        i = 0;
        while ((entry2 = readdir(dir2)) != NULL) 
        {

            if (strcmp(entry2->d_name, "." ) != 0 && strcmp(entry2->d_name, ".." ) != 0)
            {

                //Creation d'une chaîne contenant le nom du fichier dans le repertoire
                strcpy(nom_fic, nom_rep);
                strcat(nom_fic, entry2->d_name);


                //Lecture du bloc et creation du noeud dans T
                block = lireBlock(nom_fic);
                T[i] = create_node(block);
                i++;
            }

        }   
    closedir(dir2);
    }

    //Recherche des fils
    i = 0;
    int j = 0;

    while (i < size)   
    {
        j = 0;

        while (j < size) 
        {
            if (strcmp( (char *) T[i]->block->hash, (char *) T[j]->block->previous_hash) == 0) 
            {
                add_child(T[i], T[j]);
            }
        j++;
        }
        i++;
    }


    //Recherche du pere
    i = 0;
    while (i < size && T[i]->father != NULL) 
    {
        i++;
    }

    CellTree * root;

    //Si on a trouvé le père
    if (i < size) 
    {
        root = T[i];
    }
    //Sinon, on ne renvoie rien
    else
    {
        root = NULL;
    }

    //Libération mémoire
    free(T);

    return root;
}

/*Compute le gagnant de l'election et renvoie sa cle publique*/
Key *compute_winner_BT(CellTree *tree, CellKey *candidates, CellKey *voters, int sizeC, int sizeV)
{
    //verification de sizeC >= len(candidates) && sizeV >= len(voters)
    assert((sizeC >= list_keys_length(candidates)) && (sizeV >= list_keys_length(voters)));

    //extraction de la la plus longue chaine de declaration de l'arbre
    CellProtected * votes = get_trusted_list_protected(tree);

    //supprimer toute les declarations dont les signatures sont non valides
    fraude(&votes);

    //Calcul du candidat gagnant
    Key *winner = compute_winner(votes, candidates, voters, sizeC, sizeV);

    //on libere les cellules mais pas les protected
    CellProtected *ptr;
    while (votes)
    {
        ptr = votes;
        votes = votes->next;
        free(ptr);
    }
    return winner;
}
