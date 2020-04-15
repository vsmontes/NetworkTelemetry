/* Rev 27/02/2019 */
#pragma once

#include "pkt.h"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

using namespace std;

class CBinPktProcessor;
class CCsvPktProcessor;

class CDataProcessor
{

public:

	CDataProcessor();

	~CDataProcessor();

	void Add(NetworkPktRecvRAW * pkt);

	void Remove();

	void Run(); 

private:

	mutex m_proc_mtx;

	condition_variable m_proc_cv;

	bool m_loop;

	queue<NetworkPktRecvRAW*> m_queue;

	vector<CCsvPktProcessor*> m_vCSVProcessor;

	vector<CBinPktProcessor*> m_vBinProcessor;

	thread m_ProcThread;
};

