/*
	YET CLIENT

	JOB PROCESSING - SEND DATA TO SERVER
	Rev 27/02/2019
*/

#include "jobnode.h"
#include "pkt.h"
#include "workload.h"
#include <string>

JobNode::JobNode(QTcpSocket * _socket, NetworkPktTypeEnum _type)
{
	m_type = _type;
	m_socket = _socket;

	srand(time(NULL));
}

JobNode::~JobNode()
{
}

void JobNode::SendData(int tick)
{
	NetworkPktSendRAW nsend;
	nsend.source_id = 42;
	stringstream csvpayload;
	char * binpayload = nullptr;
	size_t binsize = 0;
	
	int32_t ts = tick + 1546300800000; // 1 January, 2019;

	if (m_type == CSV_REPORT)
	{
		CWorkload::LoadCSVWorkLoad(csvpayload,info.path);
		NetworkPktSendCSV * csvpkt = (NetworkPktSendCSV *) &nsend;
		csvpkt->type = CSV_REPORT;
		csvpkt->hashL = UINT32_MAX - 1;	// hash must be almost anything so we work with a fixed large number
		csvpkt->hashH = UINT32_MAX -1;
		csvpkt->report_id = info.id;
		csvpkt->timestamp_ms = ts;
	}
	else
	{
		CWorkload::LoadBINWorkLoad(binpayload, binsize, info.path);
		NetworkPktSendBIN * binpkt = (NetworkPktSendBIN *)&nsend;
		binpkt->type = BIN_CVT;
		binpkt->acq_num = tick  % INT32_MAX;
		binpkt->samples_per_channel_id = info.dim_y;
		binpkt->samples_interval_usec = 100;				// this value doesn't really matter
		binpkt->window_size_msec = info.window_size_msec;
		binpkt->timestamp_ms = ts;
	}
	
	nsend.payload = nullptr;

	QByteArray senddata = (m_type == CSV_REPORT) ? QByteArray::fromStdString(csvpayload.str()) : QByteArray::fromRawData(binpayload, binsize);

	nsend.payload_len = senddata.size();
	nsend.pkt_len = sizeof(NetworkPktSendRAW) + senddata.size();

	QByteArray sendpacket;
	sendpacket.append((char*)&nsend, sizeof(NetworkPktSendRAW));
	sendpacket.append(senddata);
	m_socket->write(sendpacket);

	if (binpayload)
		delete binpayload;
}




