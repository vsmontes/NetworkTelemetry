/*
	DATA PROCESSING LIB

	DATA PROCESSOR
	Rev 27/02/2019
*/
#include "faults.h"
#include "DataProcessor.h"
#include "CsvProcessor.h"
#include "BinProcessor.h"

#include <thread>
#include <chrono>
#include <assert.h>
#include <mutex>
#include <iostream>

#ifdef FAULT_DATAPROCESSOR_MONOLITHIC
#pragma message("--------------------------------------")
#pragma message("FAULT_DATAPROCESSOR_MONOLITHIC")
#pragma message("--------------------------------------")
#define PROCESSORS_NUM_BIN (1)
#define PROCESSORS_NUM_CSV (1)
#else
#define PROCESSORS_NUM_BIN (2)
#define PROCESSORS_NUM_CSV (2)
#endif

void StreamThread(void * p)
{
	CDataProcessor * s = (CDataProcessor*)p;
	s->Run();
}

CDataProcessor::CDataProcessor()
{
	m_ProcThread = thread(StreamThread, (void*)this);

	for (int i = 0; i < PROCESSORS_NUM_BIN; i++)
		m_vBinProcessor.push_back(new CBinPktProcessor());

	for (int i = 0; i < PROCESSORS_NUM_CSV; i++)
		m_vCSVProcessor.push_back(new CCsvPktProcessor());
}

CDataProcessor::~CDataProcessor()
{
	while (!m_queue.empty())
		this_thread::sleep_for(chrono::milliseconds(1));
	m_loop = false;
	m_proc_cv.notify_all();
	m_ProcThread.join();
	for (int i = 0; i < PROCESSORS_NUM_BIN; i++)
		delete m_vBinProcessor[i];
	for (int i = 0; i < PROCESSORS_NUM_CSV; i++)
		delete m_vCSVProcessor[i];
	m_vBinProcessor.clear();
	m_vCSVProcessor.clear();
}

void CDataProcessor::Add(NetworkPktRecvRAW * pkt)
{
	char * rawbuff = new char[pkt->pkt_len+1];
	memset(rawbuff, 0, pkt->pkt_len);
	memcpy(rawbuff, pkt, pkt->pkt_len);
	NetworkPktRecvRAW* queuepkt = (NetworkPktRecvRAW*)rawbuff;
	queuepkt->payload = (char*)(&(queuepkt->payload) + 1);
	queuepkt->payload[queuepkt->payload_len] = '\0';
	m_proc_mtx.lock();
	m_queue.push(queuepkt);
	m_proc_mtx.unlock();
	m_proc_cv.notify_one();
}

void CDataProcessor::Remove()
{
	delete m_queue.front();
	m_queue.pop();
}

void CDataProcessor::Run()
{
	m_loop = true;
	std::unique_lock<std::mutex> lck(m_proc_mtx);

	while (m_loop)
	{
		m_proc_cv.wait(lck);
		while (!m_queue.empty())
		{
			NetworkPktRecvRAW * pkt = m_queue.front();

			switch (pkt->type)
			{
				case CSV_REPORT:
				{
					int n = 0;
					bool locked = false;
					CCsvPktProcessor * proc = nullptr;
					do
					{
						for (n = 0; n < PROCESSORS_NUM_CSV; n++)
						{
							proc = m_vCSVProcessor[n];
							if (!proc->m_isBusy)
							{
								locked = true;
								break;
							}
						}
						if (!locked)
							this_thread::sleep_for(chrono::milliseconds(1));
						
					} while (!locked);
					
					NetworkPktRecvCSV * csvpkt = (NetworkPktRecvCSV *)pkt;					
					proc->Init(csvpkt);
					proc->Process();
					
#ifdef FAULT_DATAPROCESSOR_RUN_NO_BREAK
#pragma message("--------------------------------------")
#pragma message("FAULT_NOBREAK")
#pragma message("--------------------------------------")
#else
					break;
#endif
				}
				case BIN_CVT:
				{
					int n = 0;
					bool locked = false;
					CBinPktProcessor * proc = nullptr;
					do
					{
						for (n = 0; n < PROCESSORS_NUM_BIN; n++)
						{
							proc = m_vBinProcessor[n]; 
							if (!proc->m_isBusy)
							{
								locked = true;
								break;
							}
						}

#ifdef FAULT_DATAPROCESSOR_WRONG_PLACE_SLEEP
#pragma message("--------------------------------------")
#pragma message("FAULT_WRONGSLEEP")
#pragma message("--------------------------------------")
						if (locked)
							this_thread::sleep_for(chrono::milliseconds(100));
#else
						if (!locked)
							this_thread::sleep_for(chrono::milliseconds(100));
#endif						
						
					} while (!locked);
					
					NetworkPktRecvBIN * binpkt = (NetworkPktRecvBIN*)pkt;
					proc->Process(binpkt);
					break;
				}
				default:
					break;
			}

			Remove();
		}
	}
}
