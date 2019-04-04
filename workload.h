#ifndef WORKLOAD_GUARD
#define WORKLOAD_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "design_template.h"

//******************************* Beginning of variables ***************************
typedef struct continuum
{
	double M;
	double update_cost;
	double price;
	struct continuum* next;
} continuum;

continuum* head;

double max_RAM_needed; // in GB
design* d_list;
long int query_count;
double read_percentage, write_percentage;

//******************************* End of variables ***************************

void initWorkload();
void findBestPerformanceWriteOnly();
void getM_B(double* M_B, int T, int L);
void addToContinuum(continuum** head, double M, double update_cost, double price);
continuum* newPointInContinuum(double M, double update_cost, double price);
void getContinuum(continuum** head);
void getIntegralContinuum(continuum** head);

#endif
