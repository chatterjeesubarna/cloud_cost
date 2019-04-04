#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>
#include"design_template.h"
#include "workload.h"
#include "user_sphere.h"


design* newDesign(int T, int K, int Z, int D, int L, int Y, double M, double M_B, double M_F, double M_F_HI, double M_F_LO, double M_FP, double M_BF, double FPR_sum, double update_cost, double read_cost, char* msg)
{
	design* newnode = (design*)malloc(sizeof(design));
	newnode->T = T;
	newnode->K = K;
	newnode->Z = Z;
	newnode->D = D;
	newnode->L = L;
	newnode->Y = Y;
	newnode->M = M;
	newnode->M_B = M_B;
	newnode->M_F = M_F;
	newnode->M_F_HI = M_F_HI;
	newnode->M_F_LO = M_F_LO;
	newnode->M_FP = M_FP;
	newnode->M_BF = M_BF;
	newnode->FPR_sum = FPR_sum;
	newnode->update_cost = update_cost;
	newnode->read_cost = read_cost;
	newnode->total_cost = (write_percentage*query_count*update_cost/100) + (read_percentage*read_cost*query_count/100);
	newnode->next = NULL;
	newnode->down = NULL;
	newnode->msg = msg;
    //printf("\nFPR_sum:%f", FPR_sum);
	return newnode;
}

void showDesigns(design** d_list)
{
	int no_of_designs = 0, d_id=0;
	design* temp = *d_list, *temp1 = NULL;
	printf("\n");
	while(temp)
	{
		no_of_designs = 1; ++d_id;
		printf("\n%d. *************** Total cost: %f *********************\n", d_id, (temp->update_cost + temp->read_cost));
		printf("T=%d K=%d Z=%d D=%d L=%d Y=%d M=%f M_B=%f M_F=%f M_FP=%f M_BF=%f FPR_sum=%f update_cost:%f read_cost:%f total_cost:%f msg=%s\n", temp->T, temp->K, temp->Z, temp->D, temp->L, temp->Y, temp->M/(1024*1024*1024), temp->M_B/(1024*1024*1024), temp->M_F/(1024*1024*1024), temp->M_FP/(1024*1024*1024), temp->M_BF/(1024*1024*1024), temp->FPR_sum, temp->update_cost, temp->read_cost, temp->total_cost, temp->msg);
		temp1 = temp->down;
		while(temp1)
		{
			no_of_designs++;
			printf("T=%d K=%d Z=%d D=%d L=%d Y=%d M=%f M_B=%f M_F=%f M_FP=%f M_BF=%f FPR_sum=%f update_cost:%f read_cost:%f total_cost:%f msg=%s\n", temp1->T, temp1->K, temp1->Z, temp1->D, temp1->L, temp1->Y, temp1->M/(1024*1024*1024), temp1->M_B/(1024*1024*1024), temp1->M_F/(1024*1024*1024), temp->M_FP/(1024*1024*1024), temp->M_BF/(1024*1024*1024), temp1->FPR_sum, temp1->update_cost, temp1->read_cost, temp1->total_cost, temp1->msg);
			temp1 = temp1->down;
		}
		printf("\nTotal no: of designs: %d\n\n", no_of_designs);
		//printf("\n%f\t%f\t%f", temp->total_cost, temp->update_cost, temp->read_cost);
		temp = temp->next;
		temp1 = NULL;
	}
}

void logUpdateCost(design** d_list, int T, int K, int Z, int L, int Y, double M, double M_B, double M_F, double M_F_HI, double M_F_LO, double M_FP, double M_BF, double update_cost, double read_cost, char* msg)
{
	//printf("\n%f\t%f", total_budget, update_cost);
	design* temp = *d_list, *temp1=NULL;
	design* previous = NULL;
	design* new_design = newDesign(T, K, Z, 1, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, M_FP, M_BF, 0.0, update_cost, read_cost, msg);
	//printf("\nlogged %f", update_cost);
	if(!*d_list)
	{
		*d_list = new_design;
		return;
	}
	while(temp)
	{
		if(update_cost < temp->update_cost)
		{
			if(!previous) // insert deisgn at the beginning of the list
			{
				new_design->next = *d_list;
				*d_list = new_design;
			}
			else // insert deisgn somewhere at the middle of the list
			{
				previous->next = new_design;
				new_design->next = temp;
			}
			return;
		}
		else if(update_cost == temp->update_cost)
		{
			temp1 = temp;
			while(temp1->down)
			{
				temp1 = temp1->down;
			}
			temp1->down = new_design;
			//showDesigns(d_list);
			return;
		}
		previous = temp;
		temp = temp->next;
	}
	if(!temp) // insert at the end of the list
	{
		previous->next = new_design;
	}
}

void logReadCost(design** d_list, int T, int K, int Z, int L, int Y, double M, double M_B, double M_F, double M_F_HI, double M_F_LO, double M_FP, double M_BF, double FPR_sum, double update_cost, double read_cost, char* msg)
{
	design* temp = *d_list, *temp1=NULL;
	design* previous = NULL;
	design* new_design = newDesign(T, K, Z, 1, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, M_FP, M_BF, FPR_sum, update_cost, read_cost, msg);
	if(!*d_list)
	{
		*d_list = new_design;
		return;
	}
	while(temp)
	{
		if(read_cost < temp->read_cost)
		{
			if(!previous) // insert deisgn at the beginning of the list
			{
				new_design->next = *d_list;
				*d_list = new_design;
			}
			else // insert deisgn somewhere at the middle of the list
			{
				previous->next = new_design;
				new_design->next = temp;
			}
			return;
		}
		else if(read_cost == temp->read_cost)
		{
			temp1 = temp;
			while(temp1->down)
			{
				temp1 = temp1->down;
			}
			temp1->down = new_design;
			//showDesigns(d_list);
			return;
		}
		previous = temp;
		temp = temp->next;
	}
	if(!temp) // insert at the end of the list
	{
		previous->next = new_design;
	}
}

void logTotalCost(design** d_list, int T, int K, int Z, int L, int Y, double M, double M_B, double M_F, double M_F_HI, double M_F_LO, double M_FP, double M_BF, double FPR_sum, double update_cost, double read_cost, char* msg)
{
	design* temp = *d_list, *temp1=NULL;
	design* previous = NULL;
	double total_cost = (write_percentage*query_count*update_cost/100) + (read_percentage*read_cost*query_count/100);
	design* new_design = newDesign(T, K, Z, 1, L, Y, M, M_B, M_F, M_F_HI, M_F_LO, M_FP, M_BF, FPR_sum, update_cost, read_cost, msg);
	if(!*d_list)
	{
		*d_list = new_design;
		return;
	}
	while(temp)
	{
		if(total_cost < temp->total_cost)
		{
			if(!previous) // insert deisgn at the beginning of the list
			{
				new_design->next = *d_list;
				*d_list = new_design;
			}
			else // insert deisgn somewhere at the middle of the list
			{
				previous->next = new_design;
				new_design->next = temp;
			}
			return;
		}
		else if(total_cost == temp->total_cost)
		{
			temp1 = temp;
			while(temp1->down)
			{
				temp1 = temp1->down;
			}
			temp1->down = new_design;
			//showDesigns(d_list);
			return;
		}
		previous = temp;
		temp = temp->next;
	}
	if(!temp) // insert at the end of the list
	{
		previous->next = new_design;
	}
}




void getBestUpdateDesign(design** d_list)
{
	design* temp = *d_list;
	//double min_cost = temp->total_cost;
	printf("\nMinimum cost: %f\n", temp->update_cost);
	//printf("\nsecond term:%f\n", (double)(temp->T/temp->Z)*(temp->Y + 1));
	while(temp)
	{
		printf("T=%d K=%d Z=%d D=%d L=%d Y=%d M=%f M_B=%f M_F=%f M_F_HI=%f M_F_LO=%f update_cost:%f read_cost:%f total_cost:%f msg=%s\n", temp->T, temp->K, temp->Z, temp->D, temp->L, temp->Y, temp->M/(1024*1024*1024), temp->M_B/(1024*1024*1024), temp->M_F/(1024*1024*1024), temp->M_F_HI/(1024*1024*1024), temp->M_F_LO/(1024*1024*1024), temp->update_cost, temp->read_cost, temp->total_cost, temp->msg);
		//printf("\nlog(M_F_HI/M_F):%f log(T):%f", log(temp->M_F_HI/temp->M_F), log(temp->T));
		temp = temp->down;
	}
	printf("\n Total IOs:%f", (*d_list)->total_cost);
	printf("\n Total time taken to run workload:%f sec\n", (*d_list)->total_cost/(IOPS));
}

void getBestReadDesign(design** d_list)
{
	design* temp = *d_list;
	int td = 0;
	printf("\nMinimum cost: %f\n", temp->read_cost);
	while(temp)
	{
		printf("T=%d K=%d Z=%d D=%d L=%d Y=%d M=%f M_B=%f M_F=%f M_FP=%f M_BF=%f FPR_sum=%f update_cost:%f read_cost:%f total_cost:%f msg=%s\n", temp->T, temp->K, temp->Z, temp->D, temp->L, temp->Y, temp->M/(1024*1024*1024), temp->M_B/(1024*1024*1024), temp->M_F/(1024*1024*1024), temp->M_FP/(1024*1024*1024), temp->M_BF/(1024*1024*1024), temp->FPR_sum, temp->update_cost, temp->read_cost, temp->total_cost, temp->msg);
		temp = temp->down;
		td++;
	}
	printf("\ntotal designs:%d", td);
	printf("\n Total IOs:%f", (*d_list)->total_cost);
	printf("\n Total time taken to run workload:%f sec\n", (*d_list)->total_cost/(IOPS));
}

void getBestDesign(design** d_list)
{
	int gap = 1, c=0;
	design* temp = *d_list;
	int td = 0;
	//printf("\nMinimum cost: %f\n", temp->total_cost);
	//printf("\n%f\t%f\t%f\t%f", read_percentage, temp->read_cost, temp->update_cost, temp->total_cost);
	printf("\n%f\t%f", total_budget, temp->total_cost);
	/*while(temp)
	{
		if(c++ == gap) 
		{
			printf("\n%f\t%f\t%f", temp->read_cost, temp->update_cost, temp->total_cost);
			c=0;
		}
		temp = temp->next;
	}*/
	/*while(temp)
	{
		printf("T=%d K=%d Z=%d D=%d L=%d Y=%d M=%f M_B=%f M_F=%f M_FP=%f M_BF=%f FPR_sum=%f update_cost:%f read_cost:%f total_cost:%f msg=%s\n", temp->T, temp->K, temp->Z, temp->D, temp->L, temp->Y, temp->M/(1024*1024*1024), temp->M_B/(1024*1024*1024), temp->M_F/(1024*1024*1024), temp->M_FP/(1024*1024*1024), temp->M_BF/(1024*1024*1024), temp->FPR_sum, temp->update_cost, temp->read_cost, temp->total_cost, temp->msg);
		temp = temp->down;
		td++;
	}*/
	//printf("\n Total IOs:%f", (*d_list)->total_cost);
	//printf("\n Total time taken to run workload:%f sec\n", (*d_list)->total_cost/(IOPS));
}

void sortUpdateCostAsc(design** d_list)
{
	design* temp = *d_list, *temp1 = NULL;
	//printf("\n");
	while(temp)
	{
		//printf("\n%f\t%d\t%d\t%d\t%f\t%f\t%d", temp->update_cost, temp->T, temp->K, temp->Z, temp->M_F/(1024*1024*1024), temp->M_B/(1024*1024*1024), temp->L);
		//printf("\n%f\t%f", temp->M_F/(1024*1024*1024), temp->update_cost);
		printf("\n%f", temp->update_cost);
		//printf("\n%d\t%d\t%f\t%f", temp->T, temp->L, (temp->T/log(temp->T)), temp->update_cost);
		temp = temp->next;
	}
}


void sortReadCostAsc(design** d_list)
{
	design* temp = *d_list, *temp1 = NULL;
	//printf("\n");
	while(temp)
	{
		printf("\n%f", temp->read_cost);
		temp = temp->next;
	}
}


void showDesignsSortByUpdateCost(design** d_list)
{
	int no_of_designs = 0;
	design* temp = *d_list, *temp1 = NULL;
	double min_read_cost = 1000000.0;
	while(temp)
	{
		no_of_designs = 1;
		min_read_cost = temp->read_cost;
		//printf("\n*************** Total cost: %f *********************\n", temp->total_cost);
		//printf("T=%d K=%d Z=%d D=%d L=%d Y=%d M=%f M_B=%f M_F=%f M_F_HI=%f M_F_LO=%f update_cost:%f read_cost:%f msg=%s\n", temp->T, temp->K, temp->Z, temp->D, temp->L, temp->Y, temp->M/(1024*1024*1024), temp->M_B/(1024*1024*1024), temp->M_F/(1024*1024*1024), temp->M_F_HI/(1024*1024*1024), temp->M_F_LO/(1024*1024*1024), temp->update_cost, temp->read_cost, temp->msg);
		temp1 = temp->down;
		while(temp1)
		{
			if(temp1->read_cost < min_read_cost)
				min_read_cost = temp1->read_cost;
			no_of_designs++;
			//printf("T=%d K=%d Z=%d D=%d L=%d Y=%d M=%f M_B=%f M_F=%f M_F_HI=%f M_F_LO=%f update_cost:%f read_cost:%f msg=%s\n", temp1->T, temp1->K, temp1->Z, temp1->D, temp1->L, temp1->Y, temp1->M/(1024*1024*1024), temp1->M_B/(1024*1024*1024), temp1->M_F/(1024*1024*1024), temp1->M_F_HI/(1024*1024*1024), temp1->M_F_LO/(1024*1024*1024), temp1->update_cost, temp1->read_cost, temp1->msg);
			temp1 = temp1->down;
		}
		printf("\n%f\t%f\t%f", ((p*temp->update_cost)+(g*min_read_cost)), temp->update_cost, min_read_cost);
		temp = temp->next;
		temp1 = NULL;
	}
}




