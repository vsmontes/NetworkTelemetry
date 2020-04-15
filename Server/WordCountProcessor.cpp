#include "WordCountProcessor.h"

#include "proc\dlist.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

CWordCountProcessor::CWordCountProcessor()
{
}

CWordCountProcessor::~CWordCountProcessor()
{
	_DataQueue.clear();
}

void CWordCountProcessor::Append(NetworkPktRecv * pkt)
{
	_DataQueue.push_back(pkt);
}

void CWordCountProcessor::Process()
{
	const char * tempfolder = "d:/temp/wc";
	/* Declarations */
	sRootNode * wc_list = NULL;

	for (auto & it : _DataQueue)
	{
		ProcessInputData(wc_list, it->data, it->len);
	}

	sort(wc_list);

	NetworkPktRecv * pkt = _DataQueue.back();

	char tmp[1024];
	sprintf(tmp, "%s/%s.%s", tempfolder, pkt->filename, pkt->ext);

	WriteOutputData(tmp, wc_list);

	CleanList(wc_list);

	_DataQueue.clear();

	free(wc_list);
}

void CWordCountProcessor::ProcessInputData(sRootNode *& wc_list, char * data, int len)
{
	/* Declarations */
	char word[PROC_WORDLEN];
	size_t i;

	if (data)
	{
		for (i = 0; i < len; i += PROC_WORDLEN)
		{
			memcpy(word, &(data[i]), PROC_WORDLEN);

			if (wc_list == NULL)
				wc_list = InitList(word, PROC_WORDLEN);
			else
			{
				Add(wc_list, word);
			}
		}
	}
}

int CWordCountProcessor::WriteOutputData(char * file, sRootNode * wc_list)
{
	/* Declarations */
	FILE *f;
	sNode * tmp;
	char * buf;
	char * p;

	if (wc_list)
	{
		/* Declarations */
		__int64 size;

		tmp = wc_list->first;
		size = wc_list->list_len*wc_list->data_len;
		buf = (char*)malloc(size);
		p = buf;

		f = fopen(file, "wb");

		if (f)
		{
			for (int i=0; i< wc_list->list_len; i++)
			{
				memcpy(p, tmp->data, wc_list->data_len);
				p += wc_list->data_len;
				tmp = tmp->next;
			}

			fwrite(buf, size, 1, f);

			fclose(f);
		}

		crcInit();

		int n = crcFast((const unsigned char*)buf, size);

		printf("0x%X\n", n);

		free(buf);
	}

	return 0;
}

