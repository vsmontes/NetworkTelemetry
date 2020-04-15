/*
	YAT SERVER

	NETWORK PACKETS DISPATCHER
	Rev 27/02/2019
*/
#pragma once

#include "pkt.h"
#include "DataProcessor.h"

#include <QtNetwork>
#include <vector>
#include <unordered_map>
#include <thread>

using namespace std;

class CPktDispatcher : public QObject
{
	Q_OBJECT

public:

	CPktDispatcher(QTcpSocket *, QObject *parent = nullptr);

	~CPktDispatcher();

	void DispatchPkt(void * rawpkt);

public slots:

	void updateServerProgress();

private:

	QTcpSocket *tcpServerConnection;

	QByteArray * recvdata;

	NetworkPktRecvRAW PktHeader;

	int bytesReceived;

	CDataProcessor * m_DataProcessor;
};

