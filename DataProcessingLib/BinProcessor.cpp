/* 
	DATA PROCESSING LIB

	BIN DATA PROCESSOR
	Rev 27/02/2019 
*/
#include "faults.h"
#include "BinProcessor.h"
#include "DataProcessor.h"

#include <queue>
#include <list>
#include <string>
#include <iostream>

using namespace std;

shared_ptr<cvt_t>	CBinPktProcessor::CVT = nullptr;;
NetworkPktRecvBIN	CBinPktProcessor::Header = { 0 };
mutex				CBinPktProcessor::done_mtx;
mutex				CBinPktProcessor::results_mtx;
condition_variable	CBinPktProcessor::done_cv;

CBinPktProcessor::CBinPktProcessor() : m_pkt(nullptr), m_pktlen(0)
{

}

CBinPktProcessor::~CBinPktProcessor()
{
	if (CVT)
	{
		for (auto it : CVT->m_snapshots)
		{
			it->m_values.clear();
			delete it++;
		}
		CVT = nullptr;
	}
}

void CBinPktProcessor::Cleanup()
{
	// Must NOT use mtx
	if (m_pkt)
		delete[] m_pkt;
	m_pkt = nullptr;
}

void CBinPktProcessor::Process(NetworkPktRecvBIN * pkt)
{
	if (m_Worker.joinable())
		m_Worker.join();

#ifdef FAULT_PKTPROCESSOR_INIT_PKTLEN_NO_CLEAR
#pragma message("--------------------------------------")
#pragma message("FAULT_PKTPROCESSOR_INIT_PKTLEN_NO_CLEAR")
#pragma message("--------------------------------------")
	m_pktlen += pkt->pkt_len;
#else
	m_pktlen = pkt->pkt_len;
#endif

	m_pkt = new char[m_pktlen + 1];
	memcpy(m_pkt, pkt, pkt->pkt_len);
	memcpy(&Header, (NetworkPktRecvBIN*)m_pkt, sizeof(NetworkPktRecvBIN));
	m_payload = m_pkt + (sizeof(NetworkPktRecvBIN));
	m_payload_len = pkt->payload_len;
	m_payload[m_payload_len] = '\0';
	
	m_Worker = thread(&CBinPktProcessor::ProcessData, this);
}

void CBinPktProcessor::ProcessData()
{
	m_busy.lock();
	m_isBusy = true;

	NetworkPktRecvBIN header = CBinPktProcessor::Header;
	char * p = m_payload;
	
	snapshot_t * snapshot = new snapshot_t;
	snapshot->base_timestamp = header.timestamp_ms;

	while ((p - m_payload) < m_payload_len)
	{
#ifdef FAULT_BINPKTPROCESSOR_PROCESS_P_DECLARATION
		char * p = m_payload;
#endif
		int64_t * id = (int64_t*)p;
		p += sizeof(int64_t);
		if (snapshot->m_values.find(*id) == snapshot->m_values.end())
		{
			sample_t & sample = snapshot->m_values[*id];
			memset(&sample, 0, sizeof(sample_t));
			sample.min = INT32_MAX;
			sample.max = INT32_MIN;
			unordered_map<double, int> mode_count;
			for (int i = 0; i < header.samples_per_channel_id; i++)
			{
				sample.timestamp = snapshot->base_timestamp;
				double val = *(double*)p;
				sample.mean += val;
				if (i == (int)(header.samples_per_channel_id / 2.0))
					sample.median = val;
				if (sample.max < val)
					sample.max = val;
				if (sample.min > val)
					sample.min = val;
				if (mode_count.find(val) == mode_count.end())
					mode_count[val] = 0;
				else
					mode_count[val]++;
#ifdef FAULT_BINPKTPROCESSOR_PROCESS_P_INC
				p++;
#elif defined(FAULT_BINPKTPROCESSOR_PROCESS_P_NO_INC)
				// nothing
#else
				p += sizeof(int64_t);
#endif
#ifdef FAULT_BINPKTPROCESSOR_PROCESS_EARLY_BREAK
				if ((uint)p >= m_payload_len)
					break;
#else
				if ((uint64_t)(p - m_payload) >= m_payload_len)
					break;
#endif
			}
			sample.mean /= (double)header.samples_per_channel_id;
			double max_count = 0;
			double mode = 0;
			for (auto & it : mode_count)
			{
				if (max_count < it.second)
				{
					max_count = it.second;
					mode = it.first;
				}
			}
			sample.mode = mode;
		}
	}

	bool notify = false;
	// locks to update output data
	done_mtx.lock();
	{
		CBinLockResults lock;

		if (CVT == nullptr)
			CVT = make_shared<cvt_t>();

		CVT->m_snapshots.push_back(snapshot);

#ifdef FAULT_BINPKTPROCESSOR_PROCESS_CLEANUP
#pragma message("--------------------------------------")
#pragma message("FAULT_BINPKTPROCESSOR_PROCESS_CLEANUP")
#pragma message("--------------------------------------")
#else
		Cleanup();
#endif

#ifdef FAULT_BINPKTPROCESSOR_PROCESS_SLEEP
		this_thread::sleep_for(chrono::milliseconds(200));
#pragma message("--------------------------------------")
#pragma message("FAULT_BINPKTPROCESSOR_PROCESS_SLEEP")
#pragma message("--------------------------------------")
#endif

		notify = DataReady();
	}
	done_mtx.unlock();

	if (notify)
	{
		done_cv.notify_one();
	}

#ifdef FAULT_PROCESSOR_PROCESS_BUSY_UNLOCK
#pragma message("FAULT_PROCESSOR_BUSY_UNLOCK")
#else
	m_busy.unlock();
#endif

	m_isBusy = false;
}

bool CBinPktProcessor::DataReady()
{
	NetworkPktRecvBIN header = CBinPktProcessor::Header;
	bool ret = false;

	if (CVT)
	{
		if (CVT->m_snapshots.size() > 2)
		{
			double basetimestamp = header.timestamp_ms - (header.window_size_msec);
			auto snap = CVT->m_snapshots.begin();
			while (snap != CVT->m_snapshots.end())
			{
				if ((*snap)->base_timestamp < basetimestamp)
				{
					(*snap)->m_values.clear();
					snapshot_t * st = *snap;
					snap++;
					CVT->m_snapshots.remove(st);
					delete st;
					ret = true;
				}
				else
					snap++;				
			}
		}
	}
	return ret;
}

 shared_ptr<CBinLockResults> CBinPktProcessor::GetCVTWindow(shared_ptr<cvt_t> & cvt)
{
	condition_variable & cv = done_cv;
	mutex & mtx = done_mtx;

	std::unique_lock<std::mutex> lck(mtx);

	cv.wait(lck);

	shared_ptr<CBinLockResults> lock = make_shared<CBinLockResults>();

	cvt = CVT;

	return lock;
}