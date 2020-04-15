#pragma once

#include <stdlib.h>

#ifndef _DLIST_H_
#define _DLIST_H_

typedef struct _node_t
{
	char * data;
	unsigned long long count;
	struct _node_t  * next;
} sNode;

typedef struct _root_node_t
{
	unsigned long long list_len;
	int data_len;
	struct _node_t  * first;
} sRootNode;

void Add(sRootNode * root, char * data);
void CleanList(sRootNode * root);
sRootNode * InitList(char * data, int len);
void Remove(sRootNode * root, sNode * target);
sNode * Search(sRootNode * root, char * data, int * found);
void PrintList(sRootNode * root);

#endif