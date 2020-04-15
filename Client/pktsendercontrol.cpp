/*
	YAT CLIENT

	PACKET SENDER CONTROLLER
	Rev 27/02/2019
*/

#include "pktsendercontrol.h"
#include <QtNetwork>
#include <qtcpserver.h>
#include <thread>
#include <string>
#include <QVariant>

PktSenderControl::PktSenderControl(QObject *parent)
	: QObject(parent) , tick(1)
{
	tcpClient = new QTcpSocket;
	connect(tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
	connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
	tcpClient->connectToHost(QHostAddress::LocalHost, 1234);
}

PktSenderControl::~PktSenderControl()
{
}

void PktSenderControl::AddJobBIN(JobInfo info)
{
	JobNode * jb = new JobNode(tcpClient,BIN_CVT);
	jb->info = info;
	JobList.push_back(jb);
}

void PktSenderControl::AddJobCSV(JobInfo info)
{
	JobNode * jb = new JobNode(tcpClient, CSV_REPORT);
	jb->info = info;
	JobList.push_back(jb);
}

void PktSenderControl::startTransfer()
{
	qDebug() << "connected:  ";
	connect(&timer, SIGNAL(timeout()), this, SLOT(Timer()));
	timer.setInterval(STEP);
	timer.start();
}

void PktSenderControl::Timer()
{
	for (auto &it : JobList)
	{
		if ((tick % it->info.interval) == 0)
			it->SendData(tick);
	}

	tick += STEP;
}

void PktSenderControl::displayError(QAbstractSocket::SocketError socketError)
{
	if (socketError == QTcpSocket::RemoteHostClosedError)
		return;
}