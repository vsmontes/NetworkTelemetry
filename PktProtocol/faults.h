/*
	COMMON

	FAULTS FILE
	Rev 27/02/2019
*/
#pragma once
#include "defs.h"

//#define HAS_FAULT

#ifdef HAS_FAULT

// EQUIVALENTES

/// NETWORK PACKETS FAULTS
//#define FAULT_NETWORK_PKT_WINDOWSIZE_ACQNUM_SWAP			// good							- NOT EQUIV!!! no bin file - the memory starts below, but increases - good example

/// BIN PAYLODAD PROCESSING FAULTS
//#define FAULT_BINPKTPROCESSOR_PROCESS_CLEANUP			// good							- EQUIV!!!
//#define FAULT_BINPKTPROCESSOR_PROCESS_SLEEP			// good						- not equiv - big little diff at end

/// CSV PAYLODAD PROCESSING FAULTS
//#define FAULT_CSVPKTPROCESSOR_PROCESS_CLEANUP			// good							- EQUIV
//#define FAULT_CSVPKTPROCESSOR_PROCESS_INFINITE		// good							- EQUIV!!

//#define FAULT_PROCESSOR_BUSY_UNLOCK						// good							- EQUIV!

// DATAPROCESSOR PROCESSING FAULTS
#define FAULT_DATAPROCESSOR_RUN_NO_BREAK			// good									- EQUIV!!!
//#define FAULT_DATAPROCESSOR_MONOLITHIC				// good									- EQUIV!!!

#endif