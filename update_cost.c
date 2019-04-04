#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "update_cost.h"
// #include "design_template.h"
//#include "remain.h"
#include "environment_variable.h"



void analyzeUpdateCost(double* update_cost, int T, int K, int Z, int L, int Y, double M, double M_F, double M_B, double M_F_HI, double M_F_LO, design** d_list)
{
	if(Y == 0)
	{
		*update_cost = ((double)T*L)/(K*B); 
	}
	else
	{
		*update_cost = (((double)(T*(L-Y-1))/K) + ((double)T/Z)*(Y+1))/B; 
	}
}

