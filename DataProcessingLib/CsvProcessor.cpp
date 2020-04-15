/*
	DATA PROCESSING LIB

	CSV DATA PROCESSOR
	Rev 27/02/2019
*/
#include "faults.h"
#include "CsvProcessor.h"
#include "DataProcessor.h"
#include "pkt.h"
#include <assert.h>
#include <thread>
#include <mutex>

#define IS_SEPARATOR(x) ((x==',')||(x==';'))
#define IS_EOL(x) ((x=='\r')||(x=='\n'))
#define VALID(x) ((x-m_payload) < m_payload_len) 

unordered_map<int,shared_ptr<tables_t>>	CCsvPktProcessor::CurrentReports;
mutex					CCsvPktProcessor::done_mtx;
condition_variable		CCsvPktProcessor::done_cv;

CCsvPktProcessor::CCsvPktProcessor() : m_pkt(nullptr), m_initialized(false), m_pktlen(0), m_isBusy(false)
{

}

CCsvPktProcessor::~CCsvPktProcessor()
{
	Cleanup();
}

void CCsvPktProcessor::Init(NetworkPktRecvCSV * pkt)
{	
	m_pktlen = pkt->pkt_len;
	m_pkt = new char[m_pktlen + 1];
	memcpy(m_pkt, pkt, pkt->pkt_len);
	m_Header = (NetworkPktRecvCSV*)m_pkt;
	m_payload = m_pkt + (sizeof(NetworkPktRecvCSV));
	m_payload_len = pkt->payload_len;
	m_payload[m_payload_len] = '\0';
	m_Id = pkt->source_id;
	m_initialized = true;
}

void CCsvPktProcessor::Process()
{
	thread(&CCsvPktProcessor::ProcessData, this).detach();
}

void CCsvPktProcessor::Cleanup()
{
	// Must NOT use mtx
	if (m_pkt)
		delete[] m_pkt;
	m_pkt = nullptr;
}

char * CCsvPktProcessor::ParseLine(shared_ptr<tables_t> NewData, char * data, GridLine& gline, size_t& header_len)
{
	char * start = data;
	char * end = data;
	int col = 0;
	
	while ((!IS_EOL(*end)) && (VALID(end)))
	{
		if (IS_SEPARATOR(*end) || IS_EOL(*end))
		{
			string gdata;
			gdata.assign(start, end);
			start = ++end;
			if (header_len == 0)
			{
				NewData->m_Headers.push_back(gdata);
				unordered_map<string, string> s;
				NewData->m_ColumnsDatamap.push_back(s);
			}
			else
			{
				gline.line.push_back(gdata);
				if (!gdata.empty())
					NewData->m_ColumnsDatamap[col][gdata] = gdata;
				col++;
			}
		}
		else
			end++;
	}

	if (start != (end))
	{
		string gdata;
		gdata.assign(start, end);
		start = ++end;
		if (header_len == 0)
		{
			NewData->m_Headers.push_back(gdata);
			unordered_map<string, string> s;
			NewData->m_ColumnsDatamap.push_back(s);
		}
		else
		{
			gline.line.push_back(gdata);
			col++;
		}
	}

	if (header_len == 0)
	{
		header_len = NewData->m_Headers.size();
	}
	else
	{
		while (col < header_len)
		{
			string gdata;
			gline.line.push_back(gdata);
		}
	}

	header_len = NewData->m_Headers.size();

	return end;
}

void CCsvPktProcessor::ProcessData()
{	
	m_busy.lock();
	m_isBusy = true;

	shared_ptr<tables_t> NewData = make_shared<tables_t>();
	
	size_t header_len = 0;

	char * end = m_payload;
	char * start = end;

	int line = 0;
	int col = 0;

	while (VALID(end))
	{
		GridLine line;
		if (header_len == 0)
		{
			end = ParseLine(NewData, end, line, header_len);
		}
		else
		{
			end = ParseLine(NewData, end, line, header_len);
			NewData->m_Table.push_back(line);
		}

		while (VALID(end) && (IS_EOL(*end)))
			end++;
	}

	CurrentReports[m_Header->report_id] = NewData;

#ifdef FAULT_CSVPKTPROCESSOR_PROCESS_CLEANUP
#pragma message("--------------------------------------")
#pragma message("FAULT_CSVCLEANUP")
#pragma message("--------------------------------------")
#else
	Cleanup();
#endif

#ifdef FAULT_PROCESSOR_BUSY_UNLOCK
#pragma message("--------------------------------------")
#pragma message("FAULT_UNLOCK")
#pragma message("--------------------------------------")
#else
	m_busy.unlock();
#endif

	m_isBusy = false;

	done_cv.notify_all();

#ifdef FAULT_CSVPKTPROCESSOR_PROCESS_INFINITE
#pragma message("--------------------------------------")
#pragma message("FAULT_INFINITE")
#pragma message("--------------------------------------")
	while (1);
#endif

}

const shared_ptr<tables_t> CCsvPktProcessor::GetCurrentReport(int id)
{
	bool done = false;

	while (!done)
	{
		condition_variable & cv = done_cv;
		mutex & mtx = done_mtx;

		std::unique_lock<std::mutex> lck(mtx);

		cv.wait(lck);

		if (CurrentReports.find(id) != CurrentReports.end())
		{
			const shared_ptr<tables_t> DumpData = CurrentReports[id];
			return DumpData;
		}		
	}
}