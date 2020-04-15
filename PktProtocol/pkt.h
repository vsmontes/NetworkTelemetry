/*
	COMMON

	NETWORK PACKETS DEFINITIONS FILE
	Rev 27/02/2019
*/
#pragma once
#include "faults.h"
#include <stdint.h>

enum NetworkPktTypeEnum
{
	CSV_REPORT=0,
	BIN_CVT,
	PKT_TYPE_COUNT
};

#pragma pack(push,1)

struct NetworkPktSendCSV
{
	uint32_t source_id;
	uint32_t pkt_len;
	uint32_t payload_len;
	uint32_t timestamp_ms;
	uint32_t hashL;
	uint32_t hashH;
	uint32_t report_id;
	char type;
	char * payload; 
};
typedef NetworkPktSendCSV NetworkPktRecvCSV;

struct NetworkPktSendBIN
{
	uint32_t source_id;
	uint32_t pkt_len;
	uint32_t payload_len;
	uint32_t timestamp_ms;
	uint32_t window_size_msec;
	uint32_t acq_num;
	uint16_t samples_per_channel_id;
	uint16_t samples_interval_usec;
	char type;
	char * payload;
};
#if defined(FAULT_NETWORK_PKT_WINDOWSIZE_ACQNUM_SWAP)
struct NetworkPktRecvBIN
{
	uint32_t source_id;
	uint32_t pkt_len;
	uint32_t payload_len;
	uint32_t window_size_msec;
	uint32_t timestamp_ms;
	uint32_t acq_num;
	uint16_t samples_per_channel_id;
	uint16_t samples_interval_usec;
	char type;
	char * payload;
};
#else
typedef NetworkPktSendBIN NetworkPktRecvBIN;
#endif

struct NetworkPktSendRAW
{
	uint32_t source_id;
	uint32_t pkt_len;
	uint32_t payload_len;
	uint32_t reserved1;
	uint32_t reserved2;
	uint32_t reserved3;
	uint32_t reserved4;
	char type;
	char * payload;
};

typedef NetworkPktSendRAW NetworkPktRecvRAW;
#pragma pack(pop)