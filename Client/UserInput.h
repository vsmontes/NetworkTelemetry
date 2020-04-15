/*
	YAT CLIENT

	CREATE INPUTS FROM CONFIGURATION FILE DEFINITIONS
	Rev 27/02/2019
*/
#pragma once

#include <QObject>
#include "pktsendercontrol.h"

class UserInput : public QThread
{
	Q_OBJECT

public:
	UserInput(QObject *parent, PktSenderControl *sd);
	virtual ~UserInput();

public slots:
	void run();

signals:
	void Add_CSV_signal(JobInfo);
	void Add_BIN_signal(JobInfo);
	void finished();

private:

	PktSenderControl * sender;
};