#include <stdlib.h>
#include "dlist.h"
#include "sort.h"

sNode * swap_nodes(sRootNode  * head, sNode * prev)
{
	/* Declarations */
	sNode * ret = NULL;

	if (prev)
	{
		/* Declarations */
		sNode * cur = prev->next;

		if (cur)
		{
			/* Declarations */
			sNode * nxt = cur->next;

			if (nxt)
			{
				cur->next = nxt->next;
				nxt->next = cur;
				prev->next = nxt;
				ret = nxt;
			}
		}
	}
	else
	{
		/* Declarations */
		sNode * cur = head->first;
		if (cur)
		{
			/* Declarations */
			sNode * nxt = cur->next;

			if (nxt)
			{
				cur->next = nxt->next;
				nxt->next = cur;
				head->first = nxt;
				ret = nxt;
			}
		}
	}
	
	return ret;
}

void sort(sRootNode  * head)
{
	/* Declarations */
	int changed = 0;
	sNode * node = NULL;

	if ( (head) && (head->first) )
	{
		do
		{
			/* Declarations */
			sNode * node = head->first;
			sNode * prev = NULL;

			if (node)
			{
				changed = 0;
				while (node != NULL)
				{
					if ((node->next) && (node->count > node->next->count))
					{
						node = swap_nodes(head, prev);
						changed = 1;
					}
					prev = node;
					if (node)
						node = node->next;
				}
			}
			else
				changed = 0;

		} while (changed);
	}
}
//
//int main()
//{
//	sRootNode * n = InitList("aa00", 4);
//
//	for (int i=0; i < 7; i++)
//		Add(n, "000d");
//
//	for (int i = 0; i < 4; i++)
//		Add(n, "bbbb");
//
//	for (int i = 0; i < 10; i++)
//		Add(n, "0e00");
//
//	for (int i = 0; i < 7; i++)
//		Add(n, "000d");
//
//	for (int i = 0; i < 2; i++)
//		Add(n, "aa00");
//
//	for (int i = 0; i < 2; i++)
//		Add(n, "000d");
//
//	for (int i = 0; i < 5; i++)
//		Add(n, "ccc0");
//
//	for (int i = 0; i < 4; i++)
//		Add(n, "bbbb");
//
//	PrintList(n);
//
//	sort(n);
//
//	PrintList(n);
//
//	return 0;
//}