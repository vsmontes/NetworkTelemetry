/*
	YAT CLIENT

	JOB PROCESSING - SEND DATA TO SERVER
	Rev 27/02/2019
*/
#pragma once

#include <QtCore>
#include <qstring.h>
#include <QtNetwork>
#include <sstream>
#include "pkt.h"

class JobNode : public QObject
{
	Q_OBJECT

public:

	JobNode(QTcpSocket * _socket, NetworkPktTypeEnum _type);

	~JobNode();

	QTcpSocket * m_socket;

	NetworkPktTypeEnum m_type;

	JobInfo info;

	void SendData(int tick);
};

