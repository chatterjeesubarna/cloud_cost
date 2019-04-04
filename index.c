#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "environment_variable.h"
#include "design_template.h"
#include "update_cost.h"
#include "read_cost.h"
#include "design_space_navigation.h"
#include "index.h"
#include "user_sphere.h"
#include "workload.h"

double monthlyPrice(double hourly_price)
{
	return 24*30*hourly_price;
}

void getTotalNoOfDesigns(long int* td)
{
	*td = *td + (B-1)*B*B*((double)N/B);
}

int getMaxInt(int a, int b)
{
	return (a > b? a : b);
}

void setMaxRAMNeeded() 
{
	int i=0;
	int max_RAM_blocks = (int)total_budget/monthlyPrice(RAM_BLOCK_COST);
	max_RAM_needed = (((double)N*E)/(1024.0*1024*1024)); // in GB
	if(MIN_RAM_SIZE*max_RAM_blocks <= max_RAM_needed) // what I can purchase is less than or equal to what I need
	{
		max_RAM_purchased = MIN_RAM_SIZE*max_RAM_blocks;
	}
	else // what I can purchase is more than what I need
	{
		max_RAM_purchased = ceil(max_RAM_needed/MIN_RAM_SIZE)*MIN_RAM_SIZE;
	}
	//printf("\nmax_RAM_needed:%f \tmax_RAM_purchased:%f", max_RAM_needed, max_RAM_purchased);
}

void initParameters()
{
	total_budget = 00; //generateRandomBudget(MIN_BUDGET, MAX_BUDGET);
}

int FitsInMemory(double M)
{
	if(M >= (double)N*E)
		return 1;
	else
		return 0;
}


int main()
{
	initParameters();
	//read_percentage = 0;
	while(total_budget <= 60000)
	{
	int c=0;
	d_list = NULL;
	double M_B=0.0;
	int i, M_B_percent=20;
	long int td=0; 
	initWorkload();
	
	total_budget =total_budget+1000;
		//write_percentage = 100 - read_percentage;
		//read_percentage = read_percentage+5;
	
	

	/*findBestPerformanceWriteOnly();
	free(head);
	exit(0);*/

	//printf("\n *********** Budget:%f ***********", total_budget);
	setMaxRAMNeeded();
	if(FitsInMemory(max_RAM_purchased*1024*1024*1024))
	{
		printf("\nTotal data volume %f fits in %f GB memory", ((double)N*E)/(1024*1024*1024), max_RAM_purchased);
		logUpdateCost(&d_list, 0, 0, 0, 0, 0, max_RAM_purchased, 0, 0, 0, 0, 0, 0, 0, 0, "Fits in memory");
	}
	
	while(M_B < max_RAM_purchased)
	{
		M_B = (double)M_B_percent*max_RAM_purchased/100;
		navigateDesignSpace1(max_RAM_purchased*1024*1024*1024, M_B*1024*1024*1024, &d_list); 
		//printf("\nMemory %f GB, M_B: %f", max_RAM_purchased, M_B);
		M_B_percent = M_B_percent + 20;
	}

	

	//showDesigns(&d_list);
	//showDesignsSortByUpdateCost(&d_list);
	//getBestUpdateDesign(&d_list);
	//getBestReadDesign(&d_list);
	getBestDesign(&d_list);
	//sortUpdateCostAsc(&d_list);
	//sortReadCostAsc(&d_list);
	free(d_list);
}
	return 0;
}

//******************************* main() ends ***************************
