#include "pch.h"
#include <iostream>
#include <math.h>
#include <ctime>

#define PI 3.14159
#define E 2.71828
#define DICE_AMOUNT 600
#define DICE_SIDES 6

int dice[DICE_AMOUNT];
int numbersCount[DICE_SIDES];
double mianownik;
double licznik;
double liczbaKostekNr;

time_t pocz;
using namespace std;

void czekaj(int milis) {
	time_t koniec;
	koniec = clock() + milis;
	while (koniec > clock()) {};
}

double entropy() {
	mianownik = 0;
	licznik = 0;
	//mianownik
	for (int i = 0; i < DICE_SIDES; i++) {
		liczbaKostekNr = 0;
		for (int j = 1; j <= numbersCount[i]; j++) {
			liczbaKostekNr += log(j);
		}
		mianownik += liczbaKostekNr;
	}
	//licznik
	for (int j = 1; j <= DICE_AMOUNT; j++) {
		licznik += log(j);
	}
	return  licznik - mianownik;
}

void resetDice() {
	for (int i = 0; i < DICE_AMOUNT; i++) dice[i] = 1;
}

void countDice() {
	for (int i = 0; i < DICE_SIDES; i++) {
		numbersCount[i] = 0;
	}
	for (int i = 0; i < DICE_AMOUNT; i++) {
		numbersCount[dice[i] - 1]++;
	}
	for (int i = 0; i < DICE_SIDES; i++) {
		cout << numbersCount[i] << '\t';
	}
	cout << "ENTROPY: " << entropy();
	cout << "\t\tTIME: " << clock() - pocz << endl;
}

void hitTable() {
	for (int i = 0; i < DICE_AMOUNT; i++) {
		czekaj(1);
		if (rand() % 100 <= 5) {
			dice[i] = rand() % DICE_SIDES + 1;
		}
	}
}



int main()
{
	cout.precision(5);
	resetDice();
	while (true) {
		pocz = clock();
		hitTable();
		countDice();
		//czekaj(400);
	}
}

