#include <math.h>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;



double Seed = 7;
double myrand();
double max(double x, double y);
double min(double x, double y);



#define NumofUsers 5
#define SimFrame 1e7
#define NumofSimPoints 16
#define T 3              //t time slots
#define LocalRate 1


int main(){ 
	 
	string filename;

	cout << "Enter the result file name: ";
	cin >> filename;
	

	ofstream ofile;
	ofile.open( filename.c_str() );
    
	//cout << "Lambda \tAvgVirtualQueue \tAvgPower \tArrivalRate \tServiceRate \tDropRate" << endl;
	//ofile << "Lambda \tAvgVirtualQueue \tAvgPower \tArrivalRate \tServiceRate \tDropRate" << endl;

	cout << "Lambda \tAvgVirtualQueue \tAvgPower" << endl;
	ofile << "Lambda \tAvgVirtualQueue \tAvgPower" << endl;

	/*************** Symstem Parameters ******************/
	//double theta[NumofSimPoints] = {0.1, 0.2, 0.3, 0.4, 0.5,0.6, 0.7, 0.8, 0.9, 0.95};   // arrival intensity

	//double FadingProb[NumofUsers] = {0.9, 0.9, 0.9, 0.9, 0.9,  0.9, 0.9, 0.9, 0.9, 0.9 };   // symmetric case
	//double Lambda[NumofSimPoints] = { 0.28, 0.56, 0.84, 1.12, 1.4, 1.68, 1.96, 2.24, 2.52, 2.8, 2.9 };
	double Lambda[NumofSimPoints] = {0.38,0.76,1.14,1.52,1.9,2.28,2.66,3.04,3.42,3.5,3.6,3.7,3.75,3.78,3.80,3.81};
	//double Lambda[NumofSimPoints] = { 1, 2, 3, 4, 5, 6, 7, 8, 8.2, 8.4, 8.6, 8.7};
	//double Lambda[NumofSimPoints] = { 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};
	//double Lambda[NumofSimPoints] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	//double Lambda[NumofSimPoints] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	//double Lambda[NumofSimPoints] = { 2 };
	//double Lambda[NumofSimPoints] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
	//double Lambda[NumofSimPoints] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1,1.1, 1.2, 1.22, 1.23, 1.24, 1.25, 1.26, 1.27, 1.28, 1.29,1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2};
	//double Lambda[NumofSimPoints] = { 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2 };

	//double Lambda[NumofSimPoints] = { 1.1, 1.2, 1.22, 1.23, 1.24, 1.25, 1.26, 1.27, 1.28, 1.29 };
	//double Lambda[NumofSimPoints] = {1.31,1.32,1.33,1.34,1.35,1.36,1.37,1.38,1.39};
	double rho = 0.1;
	//double mu[NumofUsers] = {0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4,  0.4, 0.4, 0.4 };   // local processing speed
	//double rho[NumofUsers] = { 0.1, 0.1, 0.1, 0.1, 0.15, 0.15, 0.15,  0.15, 0.15, 0.15};  // dropping rate

	double eL = 7;     // power consumption for local computation
	double eE = 4;     // power consumption for wireless transmission 

	/****************************************************/

	/************System variables ***********************/
	double A[NumofUsers];   // arrival
	double A_cur[NumofUsers];
	double A_next[NumofUsers];
	double A_L[NumofUsers]; //local arrival
	double A_E[NumofUsers]; //edge arrival
	
	double C[NumofUsers];   // channel rate
	double W[NumofUsers];  //weight 
	double Drop[NumofUsers]; //dropping packets
	double L[NumofUsers]; //transmission service

	double X_cur[NumofUsers];   //virtual queue-length
	double X_next[NumofUsers];   //virtual queue-length

	double B[NumofUsers];   // virtual service
	double P[NumofUsers] = {0};  // power consumption 

	/**********System Statistics*******************/

	double meanX[NumofSimPoints] = { 0 };
	double meanP[NumofSimPoints] = { 0 };
	double meanDropRate[NumofSimPoints] = { 0 };
	double meanArrivalRate[NumofSimPoints] = { 0 };
	double meanServiceRate[NumofSimPoints] = { 0 };


	int edgeCount[NumofUsers];  //edge count in k frame
	int localCount[NumofUsers]; //local count in k frame 

	long double totalX;
	long double totalP;
	long double totalD;
	long double totalA;
	long double totalB;

	/**********************************************/

	/********temporary variables******************/
	int i,n,t, index;
	long long k;
	double u;
	double MaxWeight;
	
	/*********************************************/

	for (i = 0; i < NumofSimPoints; i++)    // number of arrival intensity
	{
		/**********Initialization*****************/
		totalX = 0;
		totalP = 0;
		totalA = 0;
		totalB = 0;
		totalD = 0;
		for (n = 0; n < NumofUsers; n++)
		{
			X_cur[n] = 0;
			P[n] = 0;
		}
		

		for (k = 0; k < SimFrame; k++)   // simulation starts
		{
			/*************Generating system variables****************************/
			for (n = 0; n < NumofUsers; n++)
			{
				localCount[n] = 0;
				edgeCount[n] = 0;
				/********* generating arrivals *********/
				u = myrand();
				if (u < (Lambda[i] * 1.0 / 7))
				{
					A[n] = 7;
					A_cur[n] = A[n];
				}
				else
				{
					A[n] = 0;
					A_cur[n] = A[n];
				}
				//C[n] = 4;
				/********************************************/
				u = myrand();
				if (u < 0.9)
				{
					C[n] = 4;
				}
				else
				{
					C[n] = 0;
				}
				/********* generating virtual service ************/
				if (myrand() < (rho*Lambda[i]))
				{
					B[n] = 1;
				}
				else
				{
					B[n] = 0;
				}			
			}
/******************edge process first**********************/
			for (t = 0; t < T; t++) 
			{
				MaxWeight = -1;
				for (n = 0; n < NumofUsers; n++)
				{
					W[n] = X_cur[n] * min(A_cur[n], C[n]);
					if (MaxWeight < W[n])
					{
						MaxWeight = W[n];
						index = n;
					}
				}
				for (n = 0; n < NumofUsers; n++)
				{
					if (n == index)
					{
						A_E[n] = min(A_cur[n], C[n]);
						if (A_E[n] > 0)
						{
							edgeCount[n] = edgeCount[n] + 1;
						}
						A_cur[n] = A_cur[n] - A_E[n];  //update current after doing edge
					}
				}
/******************local process**********************/
				for (n = 0 ;  n < NumofUsers; n++)
				{
					A_L[n] = min(A_cur[n], LocalRate);
					if (A_L[n] > 0)
					{
						localCount[n] = localCount[n] + 1;
					}
					A_cur[n] = A_cur[n] - A_L[n];  //next slot in k frame
				}	
			}

			for (n = 0; n < NumofUsers; n++)
			{
				Drop[n] = A_cur[n];
				X_next[n] = X_cur[n] + Drop[n] - B[n];
				if (X_next[n] < 0)
				{
					X_next[n] = 0;
				}
				X_cur[n] = X_next[n];
				P[n] = edgeCount[n] * eE + localCount[n] * eL;
			}

			/*******System statistics collection*********/

			for (n = 0; n < NumofUsers; n++)
			{
				totalX = totalX + X_cur[n];
				totalP = totalP + P[n];
				totalA = totalA + A[n];
				totalB = totalB + B[n];
				totalD = totalD + Drop[n];
			}	
		}
		
	    /******** Data Collection *******************/
	    
		meanX[i] = totalX * 1.0 / (NumofUsers * SimFrame);
		meanP[i] = totalP * 1.0 / (NumofUsers * SimFrame);
		meanArrivalRate[i] = totalA * 1.0 / (SimFrame* NumofUsers);
		meanServiceRate[i] = totalB * 1.0 / (SimFrame* NumofUsers);
		meanDropRate[i] = totalD * 1.0 / (SimFrame * NumofUsers);


		//cout << Lambda[i] << '\t' << setw(12) << setprecision(10) << meanX[i] << '\t' << setw(12) << setprecision(10) << meanP[i] << '\t' << setw(12) << setprecision(10) << meanArrivalRate[i] << '\t' << setw(12) << setprecision(10) << meanServiceRate[i] << '\t' << setw(12) << setprecision(10) << meanDropRate[i] << endl;
		//ofile << Lambda[i] << '\t' << setw(12) << setprecision(10) << meanX[i] << '\t' << setw(12) << setprecision(10) << meanP[i] << '\t' << setw(12) << setprecision(10) << meanArrivalRate[i] << '\t' << setw(12) << setprecision(10) << meanServiceRate[i] << '\t' << setw(12) << setprecision(10) << meanDropRate[i] << endl;

		cout << Lambda[i] << '\t' << setw(12) << setprecision(10) << meanX[i] << '\t' << setw(12) << setprecision(10) << meanP[i] << endl;
		ofile << Lambda[i] << '\t' << setw(12) << setprecision(10) << meanX[i] << '\t' << setw(12) << setprecision(10) << meanP[i] << endl;
	}
	return 0;
}

double myrand()
{
	double p;
	p = fmod(3125.0*Seed, 34359738337.0);
	Seed = p;
	p = p / 34359738337.0;
	return p;
}

double min(double x, double y)
{
	double z ;
	if (x <= y)
	{
		z = x;
	}
	else
	{
		z = y;
	}
	return z;
}

double max(double x, double y)
{
	double z;
	if (x <= y)
	{
		z = y;
	}
	else
	{
		z = x;
	}
	return z;
}