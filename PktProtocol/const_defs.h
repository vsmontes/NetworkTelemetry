/*
	COMMON

	DEFS - DEPRECATED
	Rev 27/02/2019
*/
#pragma once

#pragma region CONFIGURATION

//#define BIN_INPUT_ONLY
//#define CSV_INPUT_ONLY
#define CSV_AND_BIN_INPUTS

#define HEAVY_WORKLOAD

//#define BURST_MODE
//#define DEBUG_MODE

#pragma endregion


// DEFINES

#ifndef HEAVY_WORKLOAD
#define LIGHT_WORKLOAD
#endif // !HEAVY_WORKLOAD


#ifdef BURST_MODE
#define CSV_INTERVAL_ms 100
#define BIN_INTERVAL_ms 10
#elif defined(DEBUG_MODE)
#define CSV_INTERVAL_ms 4000
#define BIN_INTERVAL_ms 4000
#else
#define CSV_INTERVAL_LIGHT_ms 4000
#define CSV_INTERVAL_HEAVY_ms 5000
#define BIN_INTERVAL_ms 50
#endif

#define BIN_WINDOW_SIZE 3	//	 2 seconds

 //original
#define LCOLS		200
#define LLINES		500
#define LMETERIDS	10
#define LSAMPLES	100

#define HCOLS		 (LCOLS)
#define HLINES		 (10*LLINES)

#define HMETERIDS	 (10*LMETERIDS)
#define HSAMPLES	 (10*LSAMPLES)

#ifdef HEAVY_WORKLOAD
#define COLS		HCOLS		
#define LINES  		HLINES		
#define METERIDS  	HMETERIDS	
#define SAMPLES  	HSAMPLES	
#elif defined(LIGHT_WORKLOAD)
#define COLS		LCOLS  
#define LINES  		LLINES  
#define METERIDS	LMETERIDS
#define SAMPLES 	LSAMPLES 
#endif

#define MAGIC_NUM (0xC0FF33)

