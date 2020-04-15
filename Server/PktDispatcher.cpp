
/*
	YAT SERVER

	NETWORK PACKETS DISPATCHER
	Rev 27/02/2019
*/

#include "PktDispatcher.h"

CPktDispatcher::CPktDispatcher(QTcpSocket * _tcpServerConnection, QObject *parent) : QObject(parent)
{
	bytesReceived = 0;
	recvdata = nullptr;

	tcpServerConnection = _tcpServerConnection;

	connect(tcpServerConnection, SIGNAL(readyRead()),
		this, SLOT(updateServerProgress()));

	m_DataProcessor = new CDataProcessor;
}

CPktDispatcher::~CPktDispatcher()
{
	delete m_DataProcessor;
}

void CPktDispatcher::DispatchPkt(void * rawpkt)
{
	NetworkPktRecvRAW * pkt = (NetworkPktRecvRAW*) rawpkt;
	pkt->payload = (char*)(&(pkt->payload) + 1);
	m_DataProcessor->Add(pkt);
}

void CPktDispatcher::updateServerProgress()
{
	if (bytesReceived == 0)
	{
		recvdata = new QByteArray;
		tcpServerConnection->read((char*)&PktHeader,sizeof(NetworkPktRecvRAW));
		bytesReceived += sizeof(NetworkPktRecvRAW);
		recvdata->append((char*)&PktHeader,sizeof(NetworkPktRecvRAW));
	}

	int bytesRemaining = PktHeader.pkt_len - bytesReceived;

	if (tcpServerConnection->bytesAvailable() >= bytesRemaining)
	{
		recvdata->append(tcpServerConnection->read(bytesRemaining));
		DispatchPkt(recvdata->data());
		bytesReceived = 0;
		recvdata->clear();
		delete recvdata;
	}
	else
	{
		bytesReceived += (int)tcpServerConnection->bytesAvailable();
		bytesRemaining -= bytesReceived;
		recvdata->append(tcpServerConnection->readAll());
	}
}