#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "read_cost.h"
//#include "remain.h"
#include "environment_variable.h"

double sumOfEntriesRecursive(double M_B, int T, int L)
{
	double first = T*(M_B/E), sum = first;
	//if(L == 1)
	//	return T*(M_B/E);
	for(int i = 2;i<=L;i++)
	{
		sum = sum + first*pow(T, i-1);
	}
	return sum; //(T)*sumOfEntriesRecursive(M_B, T, L-1);
}


double noOfEntries(int i, int T, int L)
{
	return (double)N*(T-1)/(pow(T, L-i-1));
}

void analyzeReadCost(double* read_cost, double* FPR_sum, int T, int K, int Z, int L, int Y, double M, double M_B, double M_F, double M_BF, design** d_list)
{
	double entries_in_hot_level=0.0;
	/*for(int i=1;i<=L-Y;i++)
	{
		entries_in_hot_level = entries_in_hot_level + noOfEntries(i, T, L);
	}*/
	entries_in_hot_level = sumOfEntriesRecursive(M_B, T, L-Y);
	double bits_per_entry = M_BF*8/entries_in_hot_level;
	//double extra_term = pow(Z, (T-1)/T)*pow(K, 1/T)*pow(T, (T/(T-1)))/(T-1);
	//*read_cost = 1.0 + (Y*Z) + (Z*exp((-M_BF/N)*pow(T, Y))*extra_term);
	//*FPR_sum = exp((-M_BF*8/entries_in_hot_level)*(2*log(2)/log(2.7182)));
	*FPR_sum = exp((-M_BF*8/N)*(2*log(2)/log(2.7182))*pow(T, Y)) * pow(Z, (T-1)/T) * pow(K, 1/T) * pow(T, (T/(T-1)))/(T-1);
	*read_cost =  1.0 + (Y*Z) + (*FPR_sum);
	//printf("\nFPR sum: %f, FPR_sum_without_cold_levels:%f L:%d, Y:%d T:%d entries_in_hot_level:%f", FPR_sum, FPR_sum_without_cold_levels, L, Y, T, entries_in_hot_level);
	//printf("\n%f\t%f", FPR_sum, FPR_sum_without_cold_levels);
	//printf("\n%f", *read_cost);
}
