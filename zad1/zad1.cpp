#include "pch.h"
#include <iostream>
#include <math.h>
#include <ctime>

#define SIZE 300

using namespace std;

long double A[SIZE][SIZE];
long double B[SIZE][SIZE];
long double C[SIZE][SIZE];
long double D[SIZE][SIZE];

long double AB_0[SIZE][SIZE];
long double ABC_0[SIZE][SIZE];
long double ABCD_0[SIZE][SIZE];

long double AB_1[SIZE][SIZE];
long double CD_1[SIZE][SIZE];
long double ABCD_1[SIZE][SIZE];

long double BC_2[SIZE][SIZE];
long double BCD_2[SIZE][SIZE];
long double ABCD_2[SIZE][SIZE];

long double BC_3[SIZE][SIZE];
long double ABC_3[SIZE][SIZE];
long double ABCD_3[SIZE][SIZE];

long double S0[SIZE][SIZE];
long double S1[SIZE][SIZE];
long double S2[SIZE][SIZE];
long double S3[SIZE][SIZE];
long double S4[SIZE][SIZE];

time_t pocz;

void czekaj(int milis) {
	time_t koniec;
	koniec = clock() + milis;
	while (koniec > clock()) {};
}


int main()
{
	cout.precision(20);

	//Starting matrix A, B, C, D
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			A[i][j] = (long double)i / (i + j + 20.0);
			B[i][j] = (long double)j / (i + j + 30.0);
			C[i][j] = (long double)(i + j + 5) / (i + j + 40.0);
			D[i][j] = (long double)(i + j) / (i + j + 50.0);
		}
	}

	//0		Control		AB -> ABC -> ABCD
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				AB_0[i][j] += A[i][k] * B[k][j];
			}
		}
	}
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				ABC_0[i][j] += AB_0[i][k] * C[k][j];
			}
		}
	}
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				S0[i][j] += ABC_0[i][k] * D[k][j];
			}
		}
	}


	//1					AB -> CD -> ABCD
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				AB_1[i][j] += A[i][k] * B[k][j];
				CD_1[i][j] += C[i][k] * D[k][j];
			}
		}
	}
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				S1[i][j] += AB_1[i][k] * CD_1[k][j];
			}
		}
	}



	//2					BC -> BCD -> ABCD
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				BC_2[i][j] += B[i][k] * C[k][j];
			}
		}
	}
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				BCD_2[i][j] += BC_2[i][k] * D[k][j];
			}
		}
	}
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				S2[i][j] += A[i][k] * BCD_2[k][j];
			}
		}
	}



	//3					BC -> ABC -> ABCD
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				BC_3[i][j] += B[i][k] * C[k][j];
			}
		}
	}
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				ABC_3[i][j] += A[i][k] * BC_3[k][j];
			}
		}
	}
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < SIZE; i++) {
		czekaj(1);
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				S3[i][j] += ABC_3[i][k] * D[k][j];
			}
		}
	}



#pragma omp barrier
	cout.precision(20);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (abs(S0[i][j] - S1[i][j]) > 0.00001 ||
				abs(S0[i][j] - S2[i][j]) > 0.00001 ||
				abs(S0[i][j] - S3[i][j]) > 0.00001) {
				cout << "\nMacierze nie sa takie same:";
				cout << "\nS0: " << S0[i][j];
				cout << "\nS1: " << S1[i][j];
				cout << "\nS2: " << S2[i][j];
				cout << "\nS3: " << S3[i][j];
				return 0;
			}
		}
	}

	cout << "\nMacierze sa takie same.";
	cout << "\nCzas: " << clock() - pocz;
	return 0;
}


