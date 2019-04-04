#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "design_template.h"
#include "environment_variable.h"
#include "design_space_navigation.h"
#include "update_cost.h"
#include "read_cost.h"
#include "workload.h"

/*int FitsInMemory(double M_B)
{
	if(M_B >= (double)N*E)
		return 1;
	else
		return 0;
}*/
void getNoOfLevels(int* L, double M_B, int T)
{	
	*L = (int)ceil(log(N*((double)E) / (M_B))/log(T));
	//printf("N:%d E:%d M_B:%f T:%d L:%d\n", N, E, M_B, T, *L);
}

void getX(double* X, int T, int K, int Z)
{
	*X = ((double)log(T)/(T-1)) + (double)((double)(log(K) - log(Z))/T)/(2*log(2))*8; // convert to bits
	if(*X < 0)
		*X = 0;
	//if(*X<0) printf("\n%f: T:%d logK:%f logZ:%f", *X, T, log(K), log(Z));
}

void getY(int* Y, double* M_FP, double M_F, double M_F_HI, double M_F_LO, double X, int T, int L)
{
	//printf("\n\n Newwwww");
	int i, h, j, c;
	double temp_M_FP, act_M_FP;
	if(M_F >= M_F_HI)
	{
		c=1;
		*Y = 0;
		*M_FP = (double)N*(F)/(B);
	}
	else if(M_F > M_F_LO && M_F < M_F_HI)
	{
		c=2;
		*Y = L-1;
		*M_FP = M_F_LO;
		for(i=L-2;i>=1;i--)
		{
			h = L - i;
			temp_M_FP = M_F_LO;
			for(int j = 2;j<=h; j++)
			{
				temp_M_FP = temp_M_FP + (temp_M_FP*T);
			}
			if(temp_M_FP <= M_F)
			{
				*Y = i;
				*M_FP = temp_M_FP;
				//printf("\ncondition true M_F:%f M_FP:%f Y:%d L:%d", M_F/(1024*1024*1024), (*M_FP)/(1024*1024*1024), *Y, L);
			}
		}		
	}
	else
	{
		c=3;
		*Y = L-1;
		*M_FP = M_F_LO;
	}
	/*if(*Y == 0)
	{
		*Y = 1;
		*M_FP = ((double)N*(F)/(B))/(T);
	}*/
	//printf("\nFinally c:%d Y:%d, L:%d M_Flo:%f M_F:%f M_F_HI:%f M_FP:%f", c, *Y, L, M_F_LO/(1024*1024*1024), M_F/(1024*1024*1024), M_F_HI/(1024*1024*1024), (*M_FP)/(1024*1024*1024));
}

void getM_FP(double* M_FP, int T, int L, int Y, double M_B, double M_F_LO)
{
	int h, j;
	double temp_M_FP;
	if(Y == L-1)
		*M_FP = (double)(M_B*(F)*T)/((B)*(E));
	else
	{
		h = L - Y;
		temp_M_FP = M_F_LO;
		for(int j = 2;j<=h; j++)
		{
			temp_M_FP = temp_M_FP + (temp_M_FP*T);
		}
		*M_FP = temp_M_FP; 
		//*M_FP = M_F_LO*(pow(T, L-Y) - 1)/(T-1);
	}
}

void getM_BF(double* M_BF, double M_F, double M_FP)
{
	const double MARGIN = 2;
	*M_BF = 0;
	//printf("\nbefore  M_F:%f, M_BF:%f, M_FP:%f, have to allocate:%f", M_F/(1024*1024*1024), (*M_BF)/(1024*1024*1024), M_FP/(1024*1024*1024), (M_F -M_FP)/(1024*1024*1024));
	if(M_F - M_FP > 0)
		*M_BF = M_F - M_FP - MARGIN;
	else
		*M_BF = 0.0;
	//if((*M_BF)+M_FP > M_F)
	//	printf("\nafter  M_F:%f, M_BF:%f, M_FP:%f", M_F/(1024*1024*1024), (*M_BF)/(1024*1024*1024), M_FP/(1024*1024*1024));
}

int validateFilterMemoryLevels(double M_F, double M_F_HI, double M_F_LO)
{
	if(M_F_LO > M_F_HI) 
		{
			printf("\nagain");
			return 0;
		}

		return 1;
}

void set_M_B(double* M_F, double* M_B, double M, double M_F_HI, double M_F_LO)
{
	if(*M_F > M_F_HI) 
		*M_F = M_F_HI;
	if(*M_F < M_F_LO)
		*M_F = M_F_LO;
	if(M - (*M_F) <= (B*E))
		*M_F = M - (B*E);
	*M_B = (M - *M_F);
}

void set_M_F(double* M_F, double* M_B, double M, double M_F_HI, double M_F_LO)
{
	*M_F = M - *M_B;
	if(*M_F > M_F_HI) 
		*M_F = M_F_HI;
	if(*M_F < M_F_LO)
		*M_F = M_F_LO;
}

double min(double a, double b)
{
	return a < b? a : b;
}

void getM_F_LO(double* M_F_LO, double *M_B, double M, int T)
{
	if(((double)N)/(B) < (*M_B)*T/(B*E))
	{
		*M_F_LO = ((double)N/(B))*F;
	}
	else
		*M_F_LO = (double)((*M_B)*(F)*T)/((B)*(E));
	/*if(*M_B + *M_F_LO > M)
	{
		*M_B = M/(1 + ((double)F*T)/(B*E));
		*M_F_LO = (double)((*M_B)*(F)*T)/((B)*(E));
	}*/
}

void navigateDesignSpace1(double M, double M_B, design** d_list)
{
	int T, K, Z, L, Y=0;
	double X, read_cost=0.0, update_cost=0.0;
	double M_F_HI;
	double M_F; // = ((B*E + (M - M_F)) > 0 ? B*E + (M - M_F) : (B*E)); // byte
	double M_F_LO; // = (double)(M_B*(F)*T)/((B)*(E));
	double M_BF, M_FP;
	double FPR_sum=1.0;
	for(T=2;T<=B;T++)
	//T=10;
	{
		for(K=1;K<=T-1;K++)
		//K=6;
		{
			for(Z=1;Z<=T-1;Z++)
			//Z=1;
			{
				getX(&X, T, K, Z);
				M_F_HI = (double)N*((X/8)/T + (double)(F)/(B));
				getM_F_LO(&M_F_LO, &M_B, M, T);	
				if(M_B + M_F_LO > M)
					continue;;	
				set_M_F(&M_F, &M_B, M, M_F_HI, M_F_LO);
				getNoOfLevels(&L, M_B, T);
				getY(&Y, &M_FP, M_F, M_F_HI, M_F_LO, X, T, L);
				getM_BF(&M_BF, M_F, M_FP);
				
				//printf("\nBut M_F:%f M_FP:%f Y:%d", M_F/(1024*1024*1024), M_FP/(1024*1024*1024), Y);

				//printf("\nM_Fp:%f M_F:%f", M_FP/(1024*1024*1024), M_F/(1024*1024*1024));
				//printf("\nM_B:%f, M_F_LO:%f, M_F:%f, M_F_HI:%f M_FP:%f, M_BF:%f, Y:%d, L:%d, T:%d", M_B/(1024*1024*1024), M_F_LO/(1024*1024*1024), M_F/(1024*1024*1024), M_F_HI/(1024*1024*1024), M_FP/(1024*1024*1024), M_BF/(1024*1024*1024), Y, L, T);
				//printf("\n%f\t%f\t%f\t%f", pow(T, L-Y)-1, ((double)F), (M_B/((double)E*(B))/(1024*1024*1024)), ((double)T/(T-1)));
	
				if(Y == 0)
				{
					if(write_percentage != 0)
					{
						analyzeUpdateCost(&update_cost, T, K, 0, L, Y, M, M_F, M_B, M_F_HI, M_F_LO, d_list);
						//logUpdateCost(d_list, T, K, 0, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, update_cost, read_cost, "");
					}
					if(read_percentage != 0)
					{
						analyzeReadCost(&read_cost, &FPR_sum, T, K, 0, L, Y, M, M_B, M_F, M_BF, d_list);
						//logReadCost(d_list, T, K, 0, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, M_FP, M_BF, FPR_sum, update_cost, read_cost, "");
					}
					logTotalCost(d_list, T, K, 0, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, M_FP, M_BF, FPR_sum, update_cost, read_cost, "");
					//logTotalCostSortByUpdateCost(d_list, T, K, 0, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, update_cost, read_cost, "");
				}
				else
				{
					if(write_percentage != 0)
					{
						analyzeUpdateCost(&update_cost, T, K, Z, L, Y, M, M_F, M_B, M_F_HI, M_F_LO, d_list);
						//logUpdateCost(d_list, T, K, Z, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, update_cost, read_cost, "");
					}
					if(read_percentage != 0)
					{
						analyzeReadCost(&read_cost, &FPR_sum, T, K, Z, L, Y, M, M_B, M_F, M_BF, d_list);
						//logReadCost(d_list, T, K, Z, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, M_FP, M_BF, FPR_sum, update_cost, read_cost, "");
					}
					logTotalCost(d_list, T, K, Z, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, M_FP, M_BF, FPR_sum, update_cost, read_cost, "");
					//logTotalCostSortByUpdateCost(d_list, T, K, 0, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, update_cost, read_cost, "");
				}
			}
		}
	}
	
}

