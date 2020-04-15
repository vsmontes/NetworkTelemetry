#include "dlist.h"

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#ifndef FALSE
#define FALSE (0)
#endif // !FALSE

#ifndef TRUE
#define TRUE (1)
#endif // !TRUE



/* FUNCTIONS */

char * AllocData(char * data, int len)
{
	/* Declarations */
#ifdef MUTA_1
	char * ret = (char*)calloc(1, (len + 1) * 1000);
#else
	char * ret = (char*)calloc(1, len + 1);
#endif

	memcpy(ret, data, len);
	return ret;
}

sRootNode * InitList(char * data, int len)
{
	/* Declarations */
	sNode * nnode;
	sRootNode * rnode;

	nnode = (sNode*)malloc(sizeof(sNode));
	nnode->data = AllocData(data, len);
	nnode->count = 1;
	nnode->next = NULL;

	rnode = (sRootNode*)malloc(sizeof(sRootNode));
	rnode->list_len = 1;
	rnode->data_len = len;
	rnode->first = nnode;

	return rnode;
}

void CleanList(sRootNode * root)
{
	/* Declarations */
	sNode * tmp;

	if (root != NULL)
	{
		tmp = root->first;
		for (int i = 0; i< root->list_len; i++)
		{
			/* Declarations */
			sNode * d = tmp;

			tmp = tmp->next;

			free(d->data);
			free(d);
		}

		root->list_len = 0;
		root->first = NULL;
	}
}

/* Returns the prev node if cannot found the target data at ordered list */
sNode * Search(sRootNode * root, char * data, int * found)
{
	/* Declarations */
	sNode * tmp = NULL;
	sNode * target = NULL;
	sNode * prev = NULL;

	*found = FALSE;

	if (root != NULL)
	{
		tmp = root->first;

		for (int i=0; i< root->list_len; i++)
		{
			/* Declarations */
			int cmp;

			cmp = memcmp(tmp->data, data, root->data_len);
			if (cmp == 0)
			{
				*found = TRUE;
				break;
			}
			else if (cmp < 0)
				break;

			prev = tmp;
			tmp = tmp->next;
		}
	}

	if (*found)
		target = tmp;
	else
		target = prev;

	return target;
}

void Add(sRootNode * root, char * data)
{
	/* Declarations */
	sNode * target = NULL;
	sNode * nnode;
	int found = FALSE;

	if (root != NULL)
	{
		target = Search(root, data, &found);

		if (found)
			target->count++;
		else
		{
			nnode = (sNode*)malloc(sizeof(sNode));
			nnode->data = AllocData(data, root->data_len);
			nnode->count = 1;

			/* target is the prev node */
			if (target == NULL)
			{
				/*  Add at front */
				sNode* tmp = root->first;
				root->first = nnode;
				nnode->next = tmp;
			}
			else
			{
				/* Add after target */
				nnode->next = target->next;
				target->next = nnode;
			}

			root->list_len++;
		}
	}
}

void Remove(sRootNode * root, sNode * target)
{
	/* Declarations */
	sNode * tmp = NULL;

	if (root != NULL)
	{
		tmp = root->first;

		for (int i = 0; i< root->list_len; i++)
		{
			if (tmp->next == target)
			{
				tmp->next = target->next;
				free(target);
			}
			tmp = tmp->next;
		}
	}
}

//void PrintList(sRootNode * root)
//{
//	/* Declarations */
//	sNode * tmp = NULL;
//
//	if (root != NULL)
//	{
//		tmp = root->first;
//
//		while (tmp != NULL)
//		{
//			/* Declarations */
//			int i = 0;
//
//			for (i = 0; i < root->data_len; i++)
//				printf("%c", tmp->data[i]);
//
//			printf(": %llu\n", tmp->count);
//			tmp = tmp->next;
//		}
//		printf("\n");
//	}
//	printf("\n");
//}
