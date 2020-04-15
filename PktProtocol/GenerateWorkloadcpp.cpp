/*
	COMMON

	WORKLOAD FILE TOOL- GENERATE WORKLOAD FROM CONFIGURATION FILE DEFINITION
	Rev 27/02/2019
*/
#include "workload.h"
#include "defs.h"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 2)
		std::cout << "Usage app.exe config_file.json" << std::endl;
	else
	{
		CDefs defs(argv[1]);

		std::cout << "Reading: " << argv[1] << std::endl;

		for (auto &t : defs.jobs)
		{
			if (t.type == "CSV")
			{
				CWorkload::CreateCSVWorkLoad(t.path.c_str(), t.dim_x, t.dim_y);

			}
			else if (t.type == "BIN")
			{
				CWorkload::CreateBINWorkLoad(t.path.c_str(), t.dim_x, t.dim_y);
			}
			else
				throw (new exception);
		}
	}
	
	return 0;
}