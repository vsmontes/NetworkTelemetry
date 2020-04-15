#include "AVLWordCountProcessor.h"

CAVLWordCountProcessor::CAVLWordCountProcessor()
{
}


CAVLWordCountProcessor::~CAVLWordCountProcessor()
{
}

void CAVLWordCountProcessor::Append(NetworkPktRecv * pkt)
{
	_DataQueue.push_back(pkt);
}

void CAVLWordCountProcessor::Process()
{
	const char * tempfolder = "d:/temp/wc";
	/* Declarations */
	struct Node *root = NULL;
	sRootNode * list = NULL;

	for (auto & it : _DataQueue)
	{
		ProcessInputData(root, it->data, it->len);
	}

	BuildPreOrderList(root, &list);

	sort(list);

	NetworkPktRecv * pkt = _DataQueue.back();

	char tmp[1024];
	sprintf(tmp, "%s/%s.%s", tempfolder, pkt->filename, pkt->ext);

	WriteOutputData(tmp, list);

	destroyTree(root);

	_DataQueue.clear();

	free(list);
}

void CAVLWordCountProcessor::ProcessInputData(struct Node *& root, char * data, int len)
{
	/* Declarations */
	char cword[sizeof(int)];
	int * word;
	size_t i;
	const int word_size = sizeof(int);

	if (data)
	{
		for (i = 0; i < len; i += word_size)
		{
			memcpy(cword, &(data[i]), word_size);
			word = (int*)cword;
			root = insert(root, *word);
		}
	}
}

int CAVLWordCountProcessor::WriteOutputData(char * file, sRootNode * wc_list)
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
			while (tmp != NULL)
			{
				memcpy(p, tmp->data, wc_list->data_len);
				p += wc_list->data_len;
				tmp = tmp->next;
			}

			fwrite(buf, size, 1, f);

			fclose(f);
		}

		free(buf);
		CleanList(wc_list);
	}

	return 0;
}