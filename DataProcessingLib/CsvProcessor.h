/* Rev 27/02/2019 */
#pragma once
#include "pkt.h"
#include "faults.h"
#include "DataProcessor.h"
#include <string>
#include <cinttypes>
#include <vector>
#include <unordered_map>
#include <mutex>

using namespace std;

enum FieldType
{
	FIELD_TYPE_VALUE = 0,
	FIELD_TYPE_SEPARATOR,
	FIELD_TYPE_EOL,
	FIELD_TYPE_EOF
};

struct GridLine
{
	vector<string> line;
};

struct tables_t
{
	vector<string> m_Headers;

	vector<GridLine> m_Table;

	vector<unordered_map<string, string>> m_ColumnsDatamap;
};

class CCsvPktProcessor
{
	friend class CDataProcessor;

public:

	// External interface
	static const shared_ptr<tables_t> GetCurrentReport(int id);

	CCsvPktProcessor();

	~CCsvPktProcessor();

// For use of CDataProcessor class
protected:

	void Cleanup();

	void Process();

	void Init(NetworkPktRecvCSV * pkt);

	bool m_initialized;

	bool m_isBusy;

	mutex m_busy;

private:

	static mutex done_mtx;

	static condition_variable done_cv;

	static unordered_map<int, shared_ptr<tables_t>> CurrentReports;

	char * ParseLine(shared_ptr<tables_t> NewData, char * data, GridLine& gline, size_t& header_len);

	char * m_pkt;

	char * m_payload;

	int m_pktlen;

	int m_payload_len;

	int m_Id;

	NetworkPktRecvCSV * m_Header;

	void ProcessData();
};

