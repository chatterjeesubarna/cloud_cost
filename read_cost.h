#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "design_template.h"


void analyzeReadCost(double* read_cost, double* FPR_sum, int T, int K, int Z, int L, int Y, double M, double M_B, double M_F, double M_BF, design** d_list);
double noOfEntries(int i, int T, int L);
double sumOfEntriesRecursive(double M_B, int T, int L);