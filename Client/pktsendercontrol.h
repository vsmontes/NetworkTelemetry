/*
	YAT CLIENT

	PACKET SENDER CONTROLLER
	Rev 27/02/2019
*/
#pragma once

#include <QThread>
#include <QtNetwork>
#include <string>
#include "jobnode.h"

#define STEP (1) // 1 ms for timestamp granularity

class PktSenderControl : public QObject
{
	Q_OBJECT

public:

	PktSenderControl(QObject *parent);

	~PktSenderControl();

	QTimer timer;
	QTcpSocket * tcpClient;
	QByteArray datatoSend;
	int bytesToWrite;
	int bytesWritten;

signals:
	void testeSignal();
	
public slots:
	
	void Timer();
	void startTransfer();
	void displayError(QAbstractSocket::SocketError socketError);
	void AddJobBIN(JobInfo);
	void AddJobCSV(JobInfo);

private:

	unsigned long long tick;

	QList<JobNode*> JobList;
};
