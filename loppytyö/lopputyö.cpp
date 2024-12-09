#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <locale>
using namespace std;
//Veikka Rantamaa lopputyö 4
struct Varaus {
    int varausNumero;
    string varaajanNimi;
    int huoneNumero;
    int oidenMaara;
    double loppusumma;
};

int arvoHuoneidenMaara() {
    return (rand() % 131 + 40) / 2 * 2; // Arvotaan parillinen huoneiden määrä väliltä 40-300
}

int arvoAlennus() {
    int alennus = rand() % 3; // Arvotaan alennus 0%, 10% tai 20%
    return alennus == 0 ? 0 : (alennus == 1 ? 10 : 20);
}

int arvoVarausNumero() {
    return rand() % 90000 + 10000; // Arvotaan varausnumero väliltä 10000-99999
}

bool onkoVarattu(const vector<bool>& varatutHuoneet, int huoneNumero) {
    return varatutHuoneet[huoneNumero - 1]; //tarkistetaan, että onko huone varattu
}

void varaaHuone(vector<bool>& varatutHuoneet, int hinta, int alennus, ofstream& tiedosto, vector<Varaus>& varaukset) {
    int huoneNumero, oidenMaara;
    string varaajanNimi;
    cout << "Anna varaajan nimi: ";
    cin.ignore();
    getline(cin, varaajanNimi);

    cout << "Anna huoneen numero (1-" << varatutHuoneet.size() << "): ";
    cin >> huoneNumero;

    while (huoneNumero < 1 || huoneNumero > varatutHuoneet.size() || onkoVarattu(varatutHuoneet, huoneNumero)) {
        if (huoneNumero < 1 || huoneNumero > varatutHuoneet.size()) {
            cout << "Virheellinen huonenumero. ";
        }
        else {
            cout << "Huone on jo varattu. ";
        }
        cout << "Valitse toinen huonenumero (1-" << varatutHuoneet.size() << "): ";
        cin >> huoneNumero;
    }

    cout << "Anna öiden määrä: ";
    cin >> oidenMaara;

    varatutHuoneet[huoneNumero - 1] = true;
    double loppusumma = oidenMaara * hinta * (1 - alennus / 100.0);
    int varausNumero = arvoVarausNumero();

    cout << "Laskun loppusumma: " << loppusumma << " euroa (alennus " << alennus << "%) Varausnumero: " << varausNumero << endl;

    // Kirjoitetaan varaustiedot tiedostoon
    tiedosto << "Varausnumero: " << varausNumero << ", Varaajan nimi: " << varaajanNimi << ", Huone numero: " << huoneNumero << ", Öiden määrä: " << oidenMaara << ", Loppusumma: " << loppusumma << " euroa (alennus " << alennus << "%)" << endl;

    // Tallennetaan varaus
    varaukset.push_back({ varausNumero, varaajanNimi, huoneNumero, oidenMaara, loppusumma });
}

void haeVarausNumerolla(const vector<Varaus>& varaukset, int varausNumero) {
    for (const auto& varaus : varaukset) {
        if (varaus.varausNumero == varausNumero) {
            cout << "Varaus löytyi: Varaajan nimi: " << varaus.varaajanNimi << ", Huone numero: " << varaus.huoneNumero << ", Öiden määrä: " << varaus.oidenMaara << ", Loppusumma: " << varaus.loppusumma << " euroa" << endl;
            return;
        }
    }
    cout << "Varausta ei löytynyt varausnumerolla " << varausNumero << endl;
}

void haeVaraajanNimella(const vector<Varaus>& varaukset, const string& varaajanNimi) {
    for (const auto& varaus : varaukset) {
        if (varaus.varaajanNimi == varaajanNimi) {
            cout << "Varaus löytyi: Varausnumero: " << varaus.varausNumero << ", Huone numero: " << varaus.huoneNumero << ", Öiden määrä: " << varaus.oidenMaara << ", Loppusumma: " << varaus.loppusumma << " euroa" << endl;
            return;
        }
    }
    cout << "Varausta ei löytynyt varaajan nimellä " << varaajanNimi << endl;
}

int main() {
    srand(time(0));
    setlocale(LC_ALL, "fi_FI");
    
    int huoneidenMaara = arvoHuoneidenMaara();
    vector<bool> varatutHuoneet(huoneidenMaara, false);
    vector<Varaus> varaukset;

    ofstream tiedosto("varaukset.txt");

    char jatka;
    do {
        int huoneTyyppi;
        cout << "Valitse huonetyyppi (1: yhden hengen, 2: kahden hengen): ";
        cin >> huoneTyyppi;

        int hinta = (huoneTyyppi == 1) ? 100 : 150;
        int alennus = arvoAlennus();

        varaaHuone(varatutHuoneet, hinta, alennus, tiedosto, varaukset);
        cout << "Haluatko varata toisen huoneen? (k/e): ";
        cin >> jatka;
    } while (jatka == 'k' || jatka == 'K');

    tiedosto.close();

    // Hakutoiminto
    char haku;
    cout << "Haluatko tehdä haun varauksiin? (k/e): ";
    cin >> haku;
    if (haku == 'k' || haku == 'K') {
        int hakutapa;
        cout << "Valitse hakutapa (1: varausnumerolla, 2: varaajan nimellä): ";
        cin >> hakutapa;
        if (hakutapa == 1) {
            int varausNumero;
            cout << "Anna varausnumero: ";
            cin >> varausNumero;
            haeVarausNumerolla(varaukset, varausNumero);
        }
        else if (hakutapa == 2) {
            string varaajanNimi;
            cout << "Anna varaajan nimi: ";
            cin.ignore();
            getline(cin, varaajanNimi);
            haeVaraajanNimella(varaukset, varaajanNimi);
        }
        else {
            cout << "Virheellinen hakutapa." << endl;// tarkistetaan, että onko syöte oikein
        }
    }

    return 0;
}
