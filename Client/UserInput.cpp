/*
	YAT CLIENT

	CREATE INPUTS FROM CONFIGURATION FILE DEFINITIONS
	Rev 27/02/2019
*/

#include "UserInput.h"
#include <iostream>

using namespace std;

extern CDefs* defs;

UserInput::UserInput(QObject *parent, PktSenderControl *sd)
	: QThread(parent)
{
	sender = sd;
	connect(this, SIGNAL(Add_BIN_signal(JobInfo)), sd, SLOT(AddJobBIN(JobInfo)));
	connect(this, SIGNAL(Add_CSV_signal(JobInfo)), sd, SLOT(AddJobCSV(JobInfo)));
}

UserInput::~UserInput()
{
}

void UserInput::run()
{
	bool end = false;

	msleep(500);

	for (auto t : defs->jobs)
	{
		if (t.type == "CSV")
		{
			emit Add_CSV_signal(t);

		}
		else if (t.type == "BIN")
		{			
			emit Add_BIN_signal(t);
		}
		else
			throw (new exception);
	}

	while (!end)
	{
		int interval = 0;
		cout << "Active !" << endl;
		cin >> interval;
		if (interval = 1)
			break;
	}

	emit finished();
}
