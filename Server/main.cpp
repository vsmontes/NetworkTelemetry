/*
	YAT SERVER

	MAIN FILE
	Rev 27/02/2019
*/
#include <QtCore/QCoreApplication>

#include "dataserver.h"
#include "defs.h"
#include <string>
#include <algorithm>

using namespace std;

string APPNAME;
string STARTINPATH;

string OUTPUT_BIN;
string OUTPUT_CSV;

CDefs * defs;

int main(int argc, char *argv[]) 
{
	if (argc != 2)
		qDebug() << "Usage app.exe config_file.json";
	else
	{
		qDebug() << "Start Server App";
		QCoreApplication a(argc, argv);
		defs = new CDefs(argv[1]);
		string tmp = argv[0];
		std::replace(tmp.begin(), tmp.end(), '\\', '/');
		APPNAME = (tmp.substr(tmp.find_last_of('/') + 1));
		qDebug() << "APPNAME" << APPNAME.c_str();
		STARTINPATH = defs->output_path;
		OUTPUT_BIN = STARTINPATH + "bin/" + APPNAME;
		OUTPUT_CSV = STARTINPATH + "csv/" + APPNAME;
		qDebug() << "OUTPUT DUMP : BIN" << OUTPUT_BIN.c_str();
		qDebug() << "OUTPUT DUMP : CSV" << OUTPUT_CSV.c_str();

		DataServer server;
		server.run();

		return a.exec();
	}

	return 0;
}
