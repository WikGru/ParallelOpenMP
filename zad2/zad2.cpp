#include "pch.h"
#include <iostream>
#include <math.h>
#include <ctime>

#define EULER 2.718282
#define K 30000		//ITERATIONS
#define yp 1
#define yk 300		//INFINITY
#define xp 0
#define xk 3


using namespace std;

void czekaj(int milis) {
	time_t koniec;
	koniec = clock() + milis;
	while (koniec > clock()) {};
}

double calka(double x, double y, int k) {
	return pow(EULER, -(pow(x, 2) + 3 * pow(y, 2))) / sqrt(pow(x, 2) + pow(y, 2) + 1);
}

time_t pocz;

int main()
{
	pocz = clock();
	srand((unsigned)time(NULL));
	double s = 0, result = 0;
	double dx = (xk - xp) / (double)K;
	double dy = (yk - yp) / (double)K;
	double x = 0;
	double y = 0;

	for (int iy = 0; iy < K; iy++) {
		y = yp + iy * dy;
		s = 0;
#pragma omp parallel for reduction(+:s)  schedule(dynamic,1000)
		for (int ix = 0; ix < K; ix++) {
			x = xp + ix * dx;
			s += calka(x, y, ix)*dx;
		}
		result += s * dy;
	}


	cout.precision(10);
	cout << "RESULT:\t" << result;
	cout << "\n\nX_RECTWIDTH:\t" << dx;
	cout << "\nY_RECTWIDTH:\t" << dy;
	cout << "\n\nK:\t\t" << K;
	cout << "\n\nX_START:\t" << xp;
	cout << "\nX_FINISH:\t" << xk;
	cout << "\n\nY_START:\t" << yp;
	cout << "\nY_FINISH:\t" << yk << endl;

}