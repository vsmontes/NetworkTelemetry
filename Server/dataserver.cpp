/*
	YAT SERVER

	NETWORK DATA SERVER
	Rev 27/02/2019
*/

#include "DataServer.h"
#include "BinProcessor.h"
#include "CsvProcessor.h"
#include <QtNetwork>
#include <thread>
#include <chrono>

using namespace std::chrono;

extern string OUTPUT_BIN;
extern string OUTPUT_CSV;

extern CDefs * defs;

void RunWaitforBINJobThread(void*  p)
{
	DataServer * s = (DataServer*)p;
	s->waitForBINJobDone();
}

void RunWaitforCSVJobThread(void* p, int id)
{
	DataServer * s = (DataServer*)p;
	s->waitForCSVJobDone(id);
}

DataServer::DataServer(QObject *parent)	: QObject(parent), m_loop(true)
{
	connect(&m_tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

DataServer::~DataServer()
{
	delete m_Dispatcher;
}

void DataServer::run()
{
	if (!m_tcpServer.isListening() && !m_tcpServer.listen(QHostAddress::LocalHost, 1234))
	{
		return;
	}
	qDebug() << "Server Active";
}

void DataServer::acceptConnection()
{
	qDebug() << "new connection";
	QTcpSocket *socket =  m_tcpServer.nextPendingConnection();
	m_Dispatcher = new CPktDispatcher(socket);
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
	waitBINDoneThread = std::thread(&RunWaitforBINJobThread,this);
	int id = 0;
	for (auto &it : defs->jobs)
	{
		if (it.type == "CSV") // gets the first csv config as the target one
		{
			id = it.id;
			break;
		}
	}
	waitCSVDoneThread = std::thread(&RunWaitforCSVJobThread,this,id);
	waitBINDoneThread.detach();
	waitCSVDoneThread.detach();
}

void DataServer::displayError(QAbstractSocket::SocketError socketError)
{
	qDebug() << socketError;
}

void DataServer::waitForBINJobDone()
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	shared_ptr<cvt_t> cvt;
	shared_ptr<CBinLockResults> lock = CBinPktProcessor::GetCVTWindow(cvt);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	qDebug() << "BIN window available at " << duration << " milliseconds";
	
	if (cvt)
	{
		FILE * f = fopen((OUTPUT_BIN + ".dump.bin").c_str(), "w");

		for (auto &it : cvt->m_snapshots)
		{
			int n = 0;
			for (auto & samples : it->m_values)
			{
				fprintf(f, "%d \t ", samples.first);
				sample_t & v = samples.second;
				fprintf(f, "mean:%f median:%f mode:%f max:%f min:%f \t", v.mean, v.median, v.mode, v.max, v.min);
				fprintf(f, "\n");
			}
		}

		fclose(f);
	}
}

void DataServer::waitForCSVJobDone(int id)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	const shared_ptr<tables_t> DumpData = CCsvPktProcessor::GetCurrentReport(id);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	qDebug() << "CSV report available at " << duration << " milliseconds";

	if (DumpData)
	{
		FILE *f;
		f = fopen((OUTPUT_CSV + to_string(id) + ".dump.csv").c_str(), "w");
		for (const auto & it : DumpData->m_Headers)
			fprintf(f, "%s;", it.c_str());
		fprintf(f, "\n");
		for (const auto & line : DumpData->m_Table)
		{
			for (const auto & it : line.line)
				fprintf(f, "%s;", it.c_str());

			fprintf(f, "\n");
		}

		fclose(f);
	}
}