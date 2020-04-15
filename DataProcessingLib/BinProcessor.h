/* Rev 27/02/2019 */
#pragma once

#include "pkt.h"
#include "DataProcessor.h"
#include "BinLockResults.h"
#include <thread>
#include <queue>
#include <list>
#include <unordered_map>
#include <mutex>
#include <map>

using namespace std;

struct sample_t
{
	double timestamp;
	double mean;
	double median;
	double mode;
	double max;
	double min;
};

struct snapshot_t
{
	double base_timestamp;
	map<int/*id*/, sample_t/*values*/> m_values;
};

struct cvt_t
{	
	list <snapshot_t * > m_snapshots;
};

class CBinPktProcessor
{
	friend class CDataProcessor;

	friend class CBinLockResults;

public:

	// External interface
	static shared_ptr<CBinLockResults> GetCVTWindow(shared_ptr<cvt_t> & cvt);

	CBinPktProcessor();

	virtual ~CBinPktProcessor();

// For use of CDataProcessor class
protected:

	void Process(NetworkPktRecvBIN * pkt);

	bool DataReady();

	bool m_isBusy;

	mutex m_busy;

private:

	static mutex results_mtx;

	static mutex done_mtx;

	static condition_variable done_cv;

	static shared_ptr<cvt_t> CVT;

	static NetworkPktRecvBIN Header;

	thread m_Worker;

	char * m_pkt;

	char * m_payload;

	int m_pktlen;

	int m_payload_len;

	void Cleanup();

	void ProcessData();
};

