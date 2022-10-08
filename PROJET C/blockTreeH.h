#ifndef blockTreeH_H
#define blockTreeH_H
#include "blockH.h"

typedef struct block_tree_cell
{
    Block* block;
    struct block_tree_cell* father;
    struct block_tree_cell* firstChild;
    struct block_tree_cell* nextBro;
    int height;
}CellTree;


CellTree* create_node(Block* b);
int update_height(CellTree *father, CellTree *child);
void add_child(CellTree *father, CellTree *child);
void print_tree(CellTree *tree);
void delete_node(CellTree *node);
void delete_tree(CellTree *tree);
CellTree* highest_child(CellTree* cell);
CellTree *last_node(CellTree *tree);
CellProtected *get_trusted_list_protected(CellTree *tree);

void submit_vote(Protected* p);
void create_block(CellTree** tree, Key* author, int d);
void add_block(int d, char* name);
CellTree *read_tree();
//int count_files_in_dir(char* dirName);
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);


#endif
