/*
	YAT SERVER

	NETWORK DATA SERVER
	Rev 27/02/2019
*/
#pragma once

#include "PktDispatcher.h"
#include <QThread>
#include <QtNetwork>
#include <unordered_map>
#include <string>
#include <list>
#include <thread>

using namespace std;

class DataServer : public QObject
{
	Q_OBJECT

public:

	DataServer(QObject *parent = nullptr);

	~DataServer();

	void run();

	// For testing purposes

	void waitForBINJobDone();

	void waitForCSVJobDone(int id);

private slots:

	void acceptConnection();

public slots:

	void displayError(QAbstractSocket::SocketError socketError);

protected:

	std::thread waitBINDoneThread;
	std::thread waitCSVDoneThread;
	CPktDispatcher * m_Dispatcher;
	QTcpServer m_tcpServer;
	bool m_loop;
};
