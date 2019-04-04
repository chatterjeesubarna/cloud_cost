#ifndef ENVIRONMENT_VARIABLE_GUARD
#define ENVIRONMENT_VARIABLE_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// ****************************** MACROs ******************************
//#define NO_RAM_BLOCKS 8
//#define RAM_BLOCKS_BASE 1.55
#define MIN_BUDGET 7000
#define MAX_BUDGET 8000
#define RECORD_SIZE 128 //byte, RocksDB
#define DISK_BLOCK_SIZE 512 // byte
#define AVG_CREDIT_SIZE 6*1024 // byte, 6KB
#define AVG_CREDIT_COST 0.000444 // $
#define PER_IO_COST (AVG_CREDIT_COST/AVG_CREDIT_SIZE)*DISK_BLOCK_SIZE
#define PER_GB_STORAGE_COST 0.285 //$ per GB per month]
#define MAX_STORAGE_SIZE 500 // GB
#define MIN_RAM_SIZE 16 // GB
#define RAM_BLOCK_COST 0.091 // per RAM block per hour
#define FREE_STORAGE_TIER 25 // GB
#define IOPS 54.6875

//******************************* ENVIRONMENTAL PARAMETERS ***************************

#define E 128 // B
#define B 4*1024/E // 4KB is default block size
#define F 64 // B
#define N 100000000000 
//#define D 1
#define PRINT(x) ('x') 


//******************************* End of ENVIRONMENTAL PARAMETERS ***************************

#endif
