#include "pch.h"
#include <iostream>
#include <math.h>
#include <ctime>

#define PI 3.14159				//definicja sta³ej PI
#define E 2.71828				//definicja sta³ej E - liczby Eulera
#define DICE_AMOUNT 600			//iloœæ wszystkich koœci
#define DICE_SIDES 6			//iloœæ sciañ na pojedyñczej koœci

using namespace std;

int dice[DICE_AMOUNT];			//tablica wszystkich koœci
int numbersCount[DICE_SIDES];	//tablica do zliczania koœci o konkretnych oczkach
double mianownik;				//do entropii (tak naprawdê to ln(mianownik)
double licznik;					//do entropii (tak naprawdê to ln(licznik)
int diceCount = 0;				//do zliczania koœci po iteracji

time_t pocz;

void czekaj(int milis) {
	time_t koniec;
	koniec = clock() + milis;
	while (koniec > clock()) {};
}

//ustawienie wszystkich koœci na t¹ sam¹ liczbê oczek (1)
void resetDice() {
	for (int i = 0; i < DICE_AMOUNT; i++) dice[i] = 1;
}

//liczenie entropii
double entropy() {
	mianownik = 0;
	licznik = 0;
	//mianownik
	for (int i = 0; i < DICE_SIDES; i++) {
#pragma omp parallel for reduction(+:mianownik) schedule(dynamic)
		for (int j = 1; j <= numbersCount[i]; j++) {
			czekaj(1);
			mianownik += log(j);
		}
	}
	//licznik
#pragma omp parallel for reduction(+:licznik) schedule(dynamic,150)
	for (int j = 1; j <= DICE_AMOUNT; j++) {
		czekaj(1);
		licznik += log(j);
	}
	return  licznik - mianownik;
}


//zliczanie koœci po iteracji
void countDice() {
#pragma omp parallel for
	for (int i = 0; i < DICE_SIDES; i++) {
		numbersCount[i] = 0;
	}

	diceCount = 0;
#pragma omp parallel for shared(numbersCount) schedule(dynamic,150)
	for (int i = 0; i < DICE_AMOUNT; i++) {
#pragma omp critical
		numbersCount[dice[i] - 1] += 1;
		czekaj(1);
	}

	for (int i = 0; i < DICE_SIDES; i++) {
		cout << numbersCount[i] << '\t';
		diceCount += numbersCount[i];
	}

	cout << "ENTROPY: " << entropy();
	cout << "\t\tTIME: " << clock() - pocz << '\t';
	cout << diceCount;
	if (diceCount != DICE_AMOUNT) cout << "<---------";
	cout << endl;
}

//wirtualne uderzenie w stó³
void hitTable() {
#pragma omp parallel for schedule(dynamic,150)
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
	}
}

