// C program to insert a node in AVL tree
#include "avl.h"
#include "dlist.h"

#include<stdio.h>
#include<stdlib.h>

// A utility function to get height of the tree
int height(struct Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// A utility function to get maximum of two integers
int avlMax(int a, int b)
{
	return (a > b) ? a : b;
}

/* Helper function that allocates a new node with the given key and
NULL left and right pointers. */
struct Node* newNode(int key)
{
	struct Node* node = (struct Node*)
		malloc(sizeof(struct Node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1; // new node is initially added at leaf
	return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Node *rightRotate(struct Node *y)
{
	struct Node *x = y->left;
	struct Node *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = avlMax(height(y->left), height(y->right)) + 1;
	x->height = avlMax(height(x->left), height(x->right)) + 1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Node *leftRotate(struct Node *x)
{
	struct Node *y = x->right;
	struct Node *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = avlMax(height(x->left), height(x->right)) + 1;
	y->height = avlMax(height(y->left), height(y->right)) + 1;

	// Return new root
	return y;
}

// Get Balance factor of node N
int getBalance(struct Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

// Recursive function to insert key in subtree rooted
// with node and returns new root of subtree.
struct Node* insert(struct Node* node, int key)
{
	int balance = 0;

	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(key));

	if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + avlMax(height(node->left),
		height(node->right));

	/* 3. Get the balance factor of this ancestor
	node to check whether this node became
	unbalanced */
	balance = getBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
void preOrder(struct Node *root)
{
	if (root != NULL)
	{
		printf("%d ", root->key);
		preOrder(root->left);
		preOrder(root->right);
	}
}

 void BuildPreOrderList(struct Node *root, sRootNode ** list)
{
	if (root != NULL)
	{
		if (*list == NULL)
			*list = InitList((char*)&(root->key), sizeof(root->key));
		else
			Add(*list, (char*)&(root->key));

		BuildPreOrderList(root->left, list);
		BuildPreOrderList(root->right, list);
	}
}

// Extra
void dumpPreOrder(struct Node *root, FILE * fout)
{
	if ( (root != NULL) && (fout) )
	{
		fprintf(fout,"%d\n",root->key);
		dumpPreOrder(root->left, fout);
		dumpPreOrder(root->right, fout);
	}
}


// Extra
void destroyTree(struct Node *root)
{
	if (root != NULL)
	{
		struct Node *tmp = root;
		destroyTree(root->left);
		destroyTree(root->right);
		free (tmp);
	}
}
