/*
	YAT CLIENT

	MAIN FILE
	Rev 27/02/2019
*/
#include <QtCore/QCoreApplication>
#include <iostream>
#include <thread>
#include "ClientDefs.h"
#include "UserInput.h"
#include "pktsendercontrol.h"

CDefs * defs;

int main(int argc, char *argv[])
{
	if (argc != 2)
		qDebug() << "Usage app.exe config_file.json";
	else
	{
		QCoreApplication a(argc, argv);
		defs = new CDefs(argv[1]);
		qRegisterMetaType<JobInfo>("JobInfo");
		PktSenderControl *sd = new PktSenderControl(&a);
		UserInput *user = new UserInput(&a,sd);
		user->start();

		return a.exec();
	}

	return 0;
}
