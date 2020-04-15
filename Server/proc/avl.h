#include <stdio.h>

#include "dlist.h"

#ifndef _AVL_H_
#define _AVL_H_
// An AVL tree node
struct Node
{
	int key;
	struct Node *left;
	struct Node *right;
	int height;
};

struct Node* insert(struct Node* node, int key);

void dumpPreOrder(struct Node *root, FILE * fout);

void destroyTree(struct Node *root);

void BuildPreOrderList(struct Node *root, sRootNode ** list);

#endif
