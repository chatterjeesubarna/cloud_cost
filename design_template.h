#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>


//#define D 1

int g, p;
#define IOPS 54.6875
typedef struct design
{
	int T;
	int K;
	int Z;
	int D; 
	int L;
	int Y;
	double M, M_B, M_F, M_F_HI, M_F_LO, M_FP, M_BF, FPR_sum;
	double update_cost, read_cost, total_cost;
	char* msg;
	struct design* next;
	struct design* down;
} design;

//int total_no_of_designs=0;

design* newDesign(int T, int K, int Z, int D, int L, int Y, double M, double M_B, double M_F, double M_F_HI, double M_F_LO, double M_FP, double M_BF, double FPR_sum, double update_cost, double read_cost, char* msg);
void showDesigns(design** d_list);
void logUpdateCost(design** d_list, int T, int K, int Z, int L, int Y, double M, double M_B, double M_F, double M_F_HI, double M_F_LO, double M_FP, double M_BF, double update_cost, double read_cost, char* msg);
void getBestUpdateDesign(design** d_list);
void sortUpdateCostAsc(design** d_list);
void showDesignsSortByUpdateCost(design** d_list);
void logReadCost(design** d_list, int T, int K, int Z, int L, int Y, double M, double M_B, double M_F, double M_F_HI, double M_F_LO, double M_FP, double M_BF, double FPR_sum, double update_cost, double read_cost, char* msg);
void getBestReadDesign(design** d_list);
void sortReadCostAsc(design** d_list);
void logTotalCost(design** d_list, int T, int K, int Z, int L, int Y, double M, double M_B, double M_F, double M_F_HI, double M_F_LO, double M_FP, double M_BF, double FPR_sum, double update_cost, double read_cost, char* msg);
void getBestDesign(design** d_list);


