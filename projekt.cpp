#include <iostream>
#include <cmath>
using namespace std;

// prototypy funkcji
void showPopulation(int day, int susceptible, int infected, int recovered);
void showMenu();
void simulateDay();
bool checkPopulation(int susceptible, int infected, int recovered);
void changeParameters();

// zmienne globalne stan symulacji (funkcje moga jezmieniac)
int susceptible = 80;   // podatni
int infected = 10;      // zakazeni
int recovered = 10;     // ozdrowiency
int day = 0;            // numer dnia

double infectionRate = 0.20;   // wspolczynnik zakazen
double recoveryRate = 0.10;    // wspolczynnik wyzdrowien

int main() {
    int choice;

    while (true) {
        showPopulation(day, susceptible, infected, recovered);
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            simulateDay();
            if (checkPopulation(susceptible, infected, recovered)) {
                cout << "invalid population state! simulation stopped." << endl;
                return 1;
            }
            break;
        case 2:
            changeParameters();
            break;
        case 3:
            cout << "Exiting..." << endl;
            return 0;   // koniec programu
        default:
            cout << "invalid option. please choose 1, 2 or 3." << endl;
        }
    }
}


void showPopulation(int day, int susceptible, int infected, int recovered) {
    cout << endl;
    cout << "Day: " << day << endl;
    cout << "Susceptible: " << susceptible << endl;
    cout << "Infected: " << infected << endl;
    cout << "Recovered: " << recovered << endl;
    cout << "Total: " << susceptible + infected + recovered << endl;
}

void showMenu() {
    cout << endl;
    cout << "=== Epidemic Simulator ===" << endl;
    cout << "1. Simulate next day" << endl;
    cout << "2. Change model parameters" << endl;
    cout << "3. Exit" << endl;
    cout << "Your choice: ";
}

void simulateDay() {
    // obie wartosci liczymy zanim cokolwiek zmienimy,
    // wiec opieraja sie na stanie z poczatku dnia
    int newInfections = (int)floor(infected * infectionRate);
    int newRecoveries = (int)floor(infected * recoveryRate);

    if (newInfections > susceptible) {
        newInfections = susceptible;
    }
    if (newRecoveries > infected) {
        newRecoveries = infected;
    }

    // dopiero teraz aktualizujemy grupy i dzien
    susceptible = susceptible - newInfections;
    infected = infected + newInfections - newRecoveries;
    recovered = recovered + newRecoveries;
    day++;
}
// sprawdza czy zadna grupa nie jest ujemna i czy suma wszystkiego to 100
bool checkPopulation(int susceptible, int infected, int recovered) {
    return susceptible >= 0 && infected >= 0 && recovered >= 0
        && susceptible + infected + recovered == 100;
}

void changeParameters() {
    // najpierw zmienne tymczasowe, zapis dopiero gdy obie wartosci sa poprawne
    double newInfectionRate, newRecoveryRate;

    cout << "Enter infection rate (0-1): ";
    cin >> newInfectionRate;
    cout << "Enter recovery rate (0-1): ";
    cin >> newRecoveryRate;

    if (newInfectionRate < 0 || newInfectionRate > 1 ||
        newRecoveryRate < 0 || newRecoveryRate > 1) {
        cout << "both values must be between 0 and 1. parameters not changed." << endl;
    }
    else {
        infectionRate = newInfectionRate;
        recoveryRate = newRecoveryRate;
        cout << "parameters updated." << endl;
    }
}
