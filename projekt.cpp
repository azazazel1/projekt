#include <iostream>
#include <cmath>
using namespace std;

// deklaracje funkcji
void showPopulation(int day, int susceptible, int infected, int recovered);
void showMenu();
void simulateDay();
bool checkPopulation(int susceptible, int infected, int recovered);
bool isRateValid(double rate);
void changeParameters();
void saveStateToHistory();
void showHistory();

// stan poczatkowy populacji
int susceptible = 80;
int infected = 10;
int recovered = 10;
int day = 0;

// wspolczynniki zakazen i wyzdrowien
double infectionRate = 0.20;
double recoveryRate = 0.10;

// maksymalna liczba dni symulacji
const int MAX_DAYS = 100;

// tablice na historie stanow i dzienne zmiany
int populationHistory[MAX_DAYS + 1][3] = {};
int dailyNewInfections[MAX_DAYS] = {};
int dailyRecoveries[MAX_DAYS] = {};

int main() {
    saveStateToHistory(); // zapisuje dzien 0 do historii

    int choice = 0;

    while (choice != 4) {
        showPopulation(day, susceptible, infected, recovered);
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            // sprawdza czy nie osiagnieto limitu dni
            if (day < MAX_DAYS) {
                simulateDay();

                // sprawdza poprawnosc populacji przed zapisem
                if (checkPopulation(susceptible, infected, recovered) == false) {
                    cout << "ERROR: invalid population state! Simulation stopped." << endl;
                    cout << "Susceptible: " << susceptible << ", infected: " << infected
                        << ", recovered: " << recovered << endl;
                    return 1;
                }
                saveStateToHistory();
            }
            else {
                cout << "Simulation limit reached (" << MAX_DAYS
                    << " days). Cannot simulate the next day." << endl;
            }
            break;
        case 2:
            changeParameters();
            break;
        case 3:
            showHistory(); // wyswietla historie symulacji
            break;
        case 4:
            cout << "Exiting" << endl;
            break;
        default:
            cout << "Invalid option. Please choose 1, 2, 3 or 4." << endl;
            break;
        }
    }

    return 0;
}

// wyswietla aktualny stan populacji
void showPopulation(int day, int susceptible, int infected, int recovered) {
    cout << endl;
    cout << "Day: " << day << endl;
    cout << "Susceptible: " << susceptible << endl;
    cout << "Infected: " << infected << endl;
    cout << "Recovered: " << recovered << endl;
    cout << "Total: " << susceptible + infected + recovered << endl;
}

// wyswietla menu glowne
void showMenu() {
    cout << endl;
    cout << "=== Epidemic Simulator ===" << endl;
    cout << "1. Simulate next day" << endl;
    cout << "2. Change model parameters" << endl;
    cout << "3. Show simulation history" << endl;
    cout << "4. Exit" << endl;
    cout << "Your choice: ";
}

// symuluje jeden dzien i zapisuje zmiany
void simulateDay() {
    int infectedAtStart = infected;

    // oblicza nowe zakazenia i wyzdrowienia
    int newInfections = (int)floor(infectedAtStart * infectionRate + 0.000001);
    int newRecoveries = (int)floor(infectedAtStart * recoveryRate + 0.000001);

    if (newInfections > susceptible) {
        newInfections = susceptible;
    }

    if (newRecoveries > infected) {
        newRecoveries = infected;
    }

    // aktualizuje liczebnosc grup
    susceptible = susceptible - newInfections;
    infected = infected + newInfections - newRecoveries;
    recovered = recovered + newRecoveries;

    day++;

    // zapisuje dzienne zmiany jesli indeks miesci sie w zakresie
    int index = day - 1;
    if (index >= 0 && index < MAX_DAYS) {
        dailyNewInfections[index] = newInfections;
        dailyRecoveries[index] = newRecoveries;
    }
    else {
        cout << "ERROR: index " << index << " is outside the daily changes arrays. Changes not saved." << endl;
    }
}

// sprawdza czy liczby sa nieujemne i sumuja sie do 100
bool checkPopulation(int susceptible, int infected, int recovered) {
    if (susceptible < 0 || infected < 0 || recovered < 0) {
        return false;
    }
    if (susceptible + infected + recovered != 100) {
        return false;
    }
    return true;
}

// sprawdza czy wspolczynnik jest z przedzialu od 0 do 1 (i mozna sobie zmienic)
bool isRateValid(double rate) {
    return rate >= 0 && rate <= 1;
}

// pobiera i zapisuje nowe parametry modelu
void changeParameters() {
    double newInfectionRate, newRecoveryRate;

    cout << "Enter infection rate (0-1): ";
    cin >> newInfectionRate;
    cout << "Enter recovery rate (0-1): ";
    cin >> newRecoveryRate;

    bool infectionOk = isRateValid(newInfectionRate);
    bool recoveryOk = isRateValid(newRecoveryRate);

    if (infectionOk && recoveryOk) {
        infectionRate = newInfectionRate;
        recoveryRate = newRecoveryRate;
        cout << "Parameters updated." << endl;
    }
    else {
        if (infectionOk == false) {
            cout << "Error: infection rate must be between 0 and 1." << endl;
        }
        if (recoveryOk == false) {
            cout << "Error: recovery rate must be between 0 and 1." << endl;
        }
        cout << "Parameters not changed." << endl;
    }
}

// zapisuje aktualny stan do tablicy historii
void saveStateToHistory() {
    if (day >= 0 && day <= MAX_DAYS) {
        populationHistory[day][0] = susceptible;
        populationHistory[day][1] = infected;
        populationHistory[day][2] = recovered;
    }
    else {
        cout << "ERROR: day " << day << " is outside the history range. State not saved." << endl;
    }
}

// wyswietla historie od dnia 0 do biezacego dnia (maks do stu dni -1 zeby nie bylo 101)
void showHistory() {
    cout << endl;
    cout << "=== Simulation history ===" << endl;
    cout << "Day | Susceptible | Infected | Recovered | New infections | New recoveries" << endl;

    for (int d = 0; d <= day; d++) {
        cout << d << " | "
            << populationHistory[d][0] << " | "
            << populationHistory[d][1] << " | "
            << populationHistory[d][2] << " | ";

        if (d == 0) {
            cout << "- | -" << endl; // brak zmian w dniu 0
        }
        else {
            cout << dailyNewInfections[d - 1] << " | "
                << dailyRecoveries[d - 1] << endl;
        }
    }
}
