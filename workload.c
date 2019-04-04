#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "workload.h"
#include "environment_variable.h"
#include "design_space_navigation.h"
#include "index.h"


void initWorkload()
{
	query_count = 100;
	read_percentage = 50;
	write_percentage = 50;
	head = NULL;
}

void getM_B(double* M_B, int T, int L)
{
	*M_B = ((double)N*E)/pow(T, L);
	//if (*M_B == 0) printf("\n%f\t%f\t%f", (double)N*E, pow(T, L), *M_B);
}

continuum* newPointInContinuum(double M, double update_cost, double price)
{
	continuum* node = (continuum*)malloc(sizeof(continuum));
	node->M = M;
	node->update_cost = update_cost;
	node->price = price;
	node->next = NULL;
	return node;
}

void addToContinuum(continuum** head, double M, double update_cost, double price)
{
	continuum* temp = *head, *previous = NULL;
	continuum* node = newPointInContinuum(M, update_cost, price);
	if(*head == NULL) // continuum is empty
	{
		*head = node;
		return;
	}
	while(temp)
	{
		if(update_cost == temp->update_cost) 
			return;
		if(update_cost < temp->update_cost) 
		{
			if(!previous) // insert at the beginning
			{
				node->next = temp;
				return;
			}
			// insert in the middle
			previous->next = node;
			node->next = temp;
			return;
		}
		previous = temp;
		temp = temp->next;
	}
	if(!temp) // insert at the end of the list
	{
		previous->next = node;
	}
}

void getContinuum(continuum** head)
{
	continuum* temp = *head;
	while(temp)
	{
		printf("\n%f\t%f", temp->update_cost, temp->price);
		temp = temp->next;
	}
}

void getIntegralContinuum(continuum** head)
{
	continuum* temp = *head;
	int performance;
	double min_price;
	while(temp)
	{
		performance = ceil(temp->update_cost);
		min_price = temp->price;
		temp = temp->next;
		while(temp && (performance == ceil(temp->update_cost)))
		{
			if(temp->price < min_price)
			{
				min_price = temp->price;
			}
			temp = temp->next;
		}
		printf("\n%d\t%f",performance, min_price);
	}
}

void findBestPerformanceWriteOnly()
{
	int freq=0;
	int T=B, K=B-1, Z=B-1, L, Y;
	double M_B = 1000000.0;
	double M_F_HI, M_F_LO, X, M_F=0.0, M;
	int L_max = 10;
	double update_cost, price;
	for(L=0; L<L_max; L++)
	{
		if(L==0)
		{
			update_cost = 0.0;
		}
		else
		{
			update_cost = ((double)(T*L)/(K*(B))); 
		}
		M_B = (((double)N*(E))/pow(T, L) > (B*E) ? ((double)N*(E))/pow(T, L) : (B*E));
		if(L != 0)
		{
			M_F = (M_B*F*T)/(B*E);
		}
		M = M_B + M_F;
		price = monthlyPrice(ceil(((double)M/(1024*1024*1024))/(MIN_RAM_SIZE)));
		//printf("\n%f\t%f\t%f", M_B/(1024*1024*1024), M_F/(1024*1024*1024), pow(T, L));
		printf("\n%f\t%f", update_cost, price);
	}

	for(T=2;T<=B;T++)
	{
		//printf("\nT=%d", T);
		for(K=1;K<=T-1;K++)
		{
			for(Z=1;Z<=T-1;Z++)
			{
				for(L=0; L<L_max; L++)
				{
					if(L==0)
					{
						update_cost = 0.0;
					}
					else
					{
						update_cost = ((double)(T*L)/(K*(B))); 
					}
				}
			}
		}
	}
}