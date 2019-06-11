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
double M = 1;
double MaxWeight;
double max(double num1, double num2);
double min(double num1, double num2);
double CA(double X, double A, double mu, double e_l, double e_e, double c, int m);
double CA_A_l(double X, double A, double mu, double e_l, double e_e, double c, int m);
double CA_A_e(double X, double A, double mu, double e_l, double e_e, double c, int m);



#define NumofUsers 5
#define SimFrame 1e6
#define NumofSimPoints 16
#define T 3
#define LocalRate 1


int main(){ 
	 
	string filename;

	cout << "Enter the result file name: ";
	cin >> filename;
	

	ofstream ofile;
	ofile.open( filename.c_str() );
    
	//cout << "Lambda \tAvgVirtualQueue \tAvgPower \tDropRate \tServiceRate" << endl;
    //ofile << "Lambda \tAvgVirtualQueue \tAvgPower \tDropRate \tServiceRate" << endl;

	cout << "Lambda \tAvgVirtualQueue \tAvgPower" << endl;
	ofile << "Lambda \tAvgVirtualQueue \tAvgPower" << endl;

	/*************** System Parameters ******************/
	//double theta[NumofSimPoints] = {0.1, 0.2, 0.3, 0.4, 0.5,0.6, 0.7, 0.8, 0.9, 0.95};   // arrival intensity
	//double Lambda[NumofSimPoints] = { 0.28, 0.56, 0.84, 1.12, 1.4, 1.68, 1.96, 2.24, 2.52, 2.8, 2.9 };
	//double Lambda[NumofSimPoints] = {0.001, 0.002,  0.003, 0.004, 0.005, 0.008, 0.01, 0.02, 0.04, 0.06, 0.08, 0.1};
	double Lambda[NumofSimPoints] = {0.38,0.76,1.14,1.52,1.9,2.28,2.66,3.04,3.42,3.5,3.6,3.7,3.75,3.78,3.80,3.81 };
	//double Lambda[NumofSimPoints] = {0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};
	//double FadingProb[NumofUsers] = {0.9, 0.9, 0.9, 0.9, 0.9,  0.9, 0.9, 0.9, 0.9, 0.9 };   // symmetric case
	//double Lambda[NumofSimPoints] = { 0.4 ,0.8, 1.2, 1.6, 2.0, 2.4, 2.6, 3.0, 3.4, 3.6, 4.0, 4.8, 5.6, 6.4, 7.2, 8, 8.1, 8.2, 8.28 };
	//double Lambda[NumofSimPoints] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	//double Lambda[NumofSimPoints] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1,1.1, 1.2, 1.22, 1.23, 1.24, 1.25, 1.26, 1.27, 1.28, 1.29,1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2};
	//double Lambda[NumofSimPoints] = { 2 };
	//double Lambda[NumofSimPoints] = { 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2 };
   // double Lambda[NumofSimPoints] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	//double Lambda[NumofSimPoints] = { 0.4 ,0.8, 1.2, 1.6, 2.0, 2.4, 2.6, 3.0, 3.4, 3.6, 4.0 };
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
	double u[NumofUsers];  //weight gap
	double Drop[NumofUsers]; //dropping packets

	double X_cur[NumofUsers];   //virtual queue-length
	double X_next[NumofUsers];   //virtual queue-length

	double B[NumofUsers];   // virtual service
	double P[NumofUsers] = {0};  // power consumption 

	/**********System Statistics*******************/

	double meanX[NumofSimPoints] = { 0 };// , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double meanP[NumofSimPoints] = { 0 };// , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double meanDropRate[NumofSimPoints] = { 0 };
	double meanServiceRate[NumofSimPoints] = { 0 };

	//int edgeCount[NumofUsers];  //edge count in k frame
	//int localCount[NumofUsers]; //local count in k frame 

	int m[NumofUsers]; //total number of offloading in a frame
	int m_max[NumofUsers] = { 0 };
	double totalWeight = 0; 

	long double totalX;
	long double totalP;
	long double totalD;
	long double totalA;
	long double totalB;
	double r;
	//double MaxWeight;

	/**********************************************/

	/********temporary variables******************/
	int i, n, t, index;
	long long k;
	
	/*********************************************/

	for (i = 0; i < NumofSimPoints; i++)    // number of arrival intensity
	{
		/**********Initialization*****************/
		totalX = 0;
		totalP = 0;
		totalA = 0;
		totalD = 0;
		totalB = 0;
		Seed = 7;
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
				//localCount[n] = 0;
				//edgeCount[n] = 0;
				P[n] = 0;
				/********* generating arrivals *********/
				r = myrand();
				if (r < (Lambda[i] * 1.0 / 7))
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
				/******** Generating channel fading *********/
				r = myrand();
				if (r < 0.9)
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

			/*******k frame*******/
			MaxWeight = - 1e30;
			

			for (m[0] = 0; m[0] <= T; m[0]++)
			{
				for (m[1] = 0; m[1] <= T-m[0]; m[1]++)
				{
					for(m[2] = 0; m[2] <= T-m[0]-m[1]; m[2]++)
					{
						for(m[3] = 0; m[3] <= T-m[0]-m[1]-m[2]; m[3]++)
						{
							for(m[4] = 0; m[4] <= T-m[0]-m[1]-m[2]-m[3]; m[4]++)
							{
								totalWeight = 0;
								for(n =0; n < NumofUsers; n++)
								{
									if (CA(X_cur[n], A_cur[n], LocalRate, eL, eE, C[n], m[n]) >= 0)
									{
										totalWeight = totalWeight + CA(X_cur[n], A_cur[n], LocalRate, eL, eE, C[n], m[n]);
									}
								}
								if (totalWeight > MaxWeight)
								{
									MaxWeight = totalWeight;
									for (n = 0; n < NumofUsers; n++)
									{
										m_max[n] = m[n];
									}
								}				
							}
						}
					}
				}
			}

			/******update virtual queue at the end of a frame**/

			for (n = 0; n < NumofUsers; n++)
			{
				A_L[n] = CA_A_l(X_cur[n], A_cur[n], LocalRate, eL, eE, C[n], m_max[n]);
				A_E[n] = CA_A_e(X_cur[n], A_cur[n], LocalRate, eL, eE, C[n], m_max[n]);
				Drop[n] = A[n] - A_E[n] - A_L[n];
				X_next[n] = X_cur[n] + Drop[n] - B[n];
				if (X_next[n] < 0)
				{
					X_next[n] = 0;
				}
				X_cur[n] = X_next[n];
				P[n] = ceil(A_L[n]/LocalRate) * eL + m_max[n] * eE;
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
	    
		meanX[i] = totalX * 1.0 / (SimFrame* NumofUsers);
		meanP[i] = totalP * 1.0 / (SimFrame * NumofUsers);
		meanServiceRate[i] = totalB * 1.0 / (SimFrame* NumofUsers);
		meanDropRate[i] = totalD * 1.0 / (SimFrame * NumofUsers);

		//cout  << Lambda[i] << '\t' << setw(12) << setprecision(10) << meanX[i] << '\t' << setw(12) << setprecision(10) << meanP[i] << '\t' << setw(12) << setprecision(10) << meanDropRate[i] << '\t' << setw(12) << setprecision(10) << meanServiceRate[i] << endl;
		//ofile << Lambda[i] << '\t' << setw(12) << setprecision(10) << meanX[i] << '\t' << setw(12) << setprecision(10) << meanP[i] << '\t' << setw(12) << setprecision(10) << meanDropRate[i] << '\t' << setw(12) << setprecision(10) << meanServiceRate[i] << endl;

		cout << Lambda[i] << '\t' << setw(12) << setprecision(10) << meanX[i] << '\t' << setw(12) << setprecision(10) << meanP[i]  << endl;
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

double max(double num1, double num2) 
{
 
   double result;
 
   if (num1 > num2)
      result = num1;
   else
      result = num2;
 
   return result; 
}

double min(double num1, double num2) 
{
 
   double result;
 
   if (num1 < num2)
      result = num1;
   else
      result = num2;
 
   return result; 
}


double CA(double X, double A, double mu, double e_l, double e_e, double c, int m)
{
	double A_e, A_l, weight;
	int temp_m;
	if (A <= m * c)
	{
		temp_m = floor(A/c);
		if(X*min(temp_m * c, m * c) - M * min(temp_m, m) * e_e >= 0)
		{
			if (X * min(A - temp_m * c, c) - M * e_e >= 0)		
			{	
				A_e = A;
				A_l = 0;
			}
			else
			{
				A_e = temp_m *c;
				A_l = 0;
			}
		}
		else
		{
			A_e = 0;
			if (X* min(A, T*mu) - M*min(ceil((A/mu)), T) * e_l>= 0)
			{
				A_l = min(A, T*mu);
			}
			else
			{
				A_l = 0;
			}
			
		}	
	}
	else if (A <= m * c + T * mu)
	{
		if (X*min(A, m * c) - M * m * e_e >= 0)
		{
			A_e = m * c;
			if ( X*min(A - m * c, T*mu) - M * e_l*min(ceil((A - m * c)/mu), T)>=0 )
			{
				A_l = A - m * c;
			}
			else
			{
				A_l = 0;
 			}
		}
		else
		{
			A_e = 0;
			if ( X*min(A, T*mu) - M * e_l*min(ceil((A)/mu), T)>=0 )
			{
				A_l =  min(A, T*mu);
			}
			else
			{
				A_l = 0;
 			}
		}
	}
	else
	{
		if (X*min(A, m * c) - M * m * e_e >= 0)
		{
			A_e = m * c;
			if ( X*min(A - m * c, T*mu) - M * e_l*min(ceil((A - m * c)/mu), T)>=0 )
			{
				A_l = T * mu;
			}
			else
			{
				A_l = 0;
 			}
		}
		else
		{
			A_e = 0;
			if ( X*min(A, T*mu) - M * e_l*min(ceil((A)/mu), T)>=0 )
			{
				A_l =  T * mu;
			}
			else
			{
				A_l = 0;
 			}
		}
	}

	weight = X * min(A_l, T * mu) - M * e_l * min(ceil(A_l/mu), T) + X * min(A_e, m * c) - M * e_e * m;
	return weight;
 }

double CA_A_l(double X, double A, double mu, double e_l, double e_e, double c, int m)
{
	double A_e, A_l, weight;
	if (A <= m * c)
	{
		if(X*min(A, m * c) - M * m * e_e >= 0)
		{
			A_e = A;
			A_l = 0;
		}
		else
		{
			A_e = 0;
			if (X* min(A, T*mu) - M*min(ceil((A/mu)), T)*e_l >= 0)
			{
				A_l = min(A, T*mu);
			}
			else
			{
				A_l = 0;
			}
		}	
	}
	else if (A <= m * c + T * mu)
	{
		if (X*min(A, m * c) - M * m * e_e >= 0)
		{
			A_e = m * c;
			if ( X*min(A - m * c, T*mu) - M * e_l*min(ceil((A - m * c)/mu), T)>=0 )
			{
				A_l = A - m * c;
			}
			else
			{
				A_l = 0;
 			}
		}
		else
		{
			A_e = 0;
			if ( X*min(A, T*mu) - M * e_l*min(ceil((A)/mu), T)>=0 )
			{
				A_l =  min(A, T*mu);
			}
			else
			{
				A_l = 0;
 			}
		}
	}
	else
	{
		if (X*min(A, m * c) - M * m * e_e >= 0)
		{
			A_e = m * c;
			if ( X*min(A - m * c, T*mu) - M * e_l*min(ceil((A - m * c)/mu), T)>=0 )
			{
				A_l = T * mu;
			}
			else
			{
				A_l = 0;
 			}
		}
		else
		{
			A_e = 0;
			if ( X*min(A, T*mu) - M * e_l*min(ceil((A)/mu), T)>=0 )
			{
				A_l =  T * mu;
			}
			else
			{
				A_l = 0;
 			}
		}
	}
	return A_l;
 }

double CA_A_e(double X, double A, double mu, double e_l, double e_e, double c, int m)
{
	double A_e, A_l, weight;
	if (A <= m * c)
	{
		if(X*min(A, m * c) - M * m * e_e >= 0)
		{
			A_e = A;
			A_l = 0;
		}
		else
		{
			A_e = 0;
			if (X* min(A, T*mu) - M*min(ceil((A/mu)), T)*e_l >= 0)
			{
				A_l = min(A, T*mu);
			}
			else
			{
				A_l = 0;
			}
		}	
	}
	else if (A <= m * c + T * mu)
	{
		if (X*min(A, m * c) - M * m * e_e >= 0)
		{
			A_e = m * c;
			if ( X*min(A - m * c, T*mu) - M * e_l*min(ceil((A - m * c)/mu), T)>=0 )
			{
				A_l = A - m * c;
			}
			else
			{
				A_l = 0;
 			}
		}
		else
		{
			A_e = 0;
			if ( X*min(A, T*mu) - M * e_l*min(ceil((A)/mu), T)>=0 )
			{
				A_l = min(A, T*mu);
			}
			else
			{
				A_l = 0;
 			}
		}
	}
	else
	{
		if (X*min(A, m * c) - M * m * e_e >= 0)
		{
			A_e = m * c;
			if ( X*min(A - m * c, T*mu) - M * e_l*min(ceil((A - m * c)/mu), T)>=0 )
			{
				A_l = T * mu;
			}
			else
			{
				A_l = 0;
 			}
		}
		else
		{
			A_e = 0;
			if ( X*min(A, T*mu) - M * e_l*min(ceil((A)/mu), T)>=0 )
			{
				A_l =  T * mu;
			}
			else
			{
				A_l = 0;
 			}
		}
	}
	return A_e;
 }
