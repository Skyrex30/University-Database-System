#include "HashTable.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>


using namespace std;

struct YearlyData {
    double totalAverage = 0.0;
    int count = 0;
    string year;

    YearlyData() = default;
    YearlyData(const string& y, double avg, int c) : year(y), totalAverage(avg), count(c) {}


    friend ostream& operator<<(ostream& os, const YearlyData& that) {
        int yearWidth = 10;
        int totalAverageWidth = 20;

        os << setw(yearWidth) << left << that.year
            << setw(totalAverageWidth) << left << that.totalAverage / that.count;

        return os;
    }
};


int getYearIndex(const string& year) {
    return stoi(year) % 100;
}

double calculateYearlyAverage(const vector<vector<YearlyData>>& yearlyAverages, const string& year) {
    int yearIndex = getYearIndex(year);
    double totalAverage = 0.0;
    int count = 0;
    for (auto& data : yearlyAverages[yearIndex]) {
        if (data.year == year) {
            totalAverage += data.totalAverage;
            count += data.count;
        }
    }
    return count > 0 ? totalAverage / count : 0.0;
}

void updateYearlyAverages(vector<vector<YearlyData>>& yearlyAverages, const string& year, double average) {
    int yearIndex = getYearIndex(year);
    bool yearExists = false;
    for (auto& data : yearlyAverages[yearIndex]) {
        if (data.year == year) {
            data.totalAverage += average;
            data.count++;
            yearExists = true;
            break;
        }
    }
    if (!yearExists) {
        yearlyAverages[yearIndex].push_back(YearlyData(year, average, 1));
    }
}

void removeYearlyAverage(vector<vector<YearlyData>>& yearlyAverages, const string& year, double average) {
    int yearIndex = getYearIndex(year);
    for (auto& data : yearlyAverages[yearIndex]) {
        if (data.year == year) {
            data.totalAverage -= average;
            data.count--;
            break;
        }
    }
}

void compareYearlyAverages(const vector<vector<YearlyData>>& yearlyAverages, const string& year) {
    double currentYearAverage = calculateYearlyAverage(yearlyAverages, year);
    double previousYearAverage = calculateYearlyAverage(yearlyAverages, to_string(stoi(year) - 1));

    cout << "Az atlag " << year << "-ben" << ": " << setprecision(3) << currentYearAverage << endl;
    cout << "Az atlag " << stoi(year) - 1 << "-ben" << ": " << setprecision(3) << previousYearAverage << endl;
    if (previousYearAverage != 0.0) {
        if (currentYearAverage > previousYearAverage) {
            cout << "Az atlag novekedett az elozo evhez kepest." << endl;
        }
        else if (currentYearAverage < previousYearAverage) {
            cout << "Az atlag csokkent az elozo evhez kepest." << endl;
        }
        else {
            cout << "Az atlag ugyanannyi mint az elozo evben." << endl;
        }
    }
    else {
        cout << "Nincs adat az elozo evhez." << endl;
    }
}

void updateFile(const string& filename, HashTable<Student>& students, int numberOfStudents, int maxStudents) {
	ofstream fout;
	fout.open(filename);

	if (!fout.is_open()) {
		cerr << "Error: nem lehet megnyitni a filet." << endl;
		return;
	}

	fout << maxStudents << " " << numberOfStudents << endl;


	for (int i = 0; i < students.maxHash; ++i) {
		if (students.a[i].isNormal()) {
			Student s = students.a[i].getValue();
			fout << s.uid << " " << s.lastName << " " << s.firstName << " " << s.year << " " << s.getAverage() << endl;
		}
	}


	fout.close();
}


HashTable<Student> inputFromFile(const string& filename, int& tableSize, int& n, vector<vector<YearlyData>>& yearlyAverages) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Nem letezo filet adott meg, ezert letre lessz hozva." << endl;
        cout << "Kerem adja meg a hallgatok maximalis szamat:" << endl;
        cin >> tableSize;
        n = 0;
        HashTable<Student> table(tableSize);
        return table;
    }

    fin >> tableSize;
    fin >> n;
    HashTable<Student> table(tableSize);

    for (int i = 0; i < n; i++) {
        int uid;
        double average;
        int collisions = 0;
        string lastName, firstName, year;
        fin >> uid >> lastName >> firstName >> year >> average;
        Student s(uid, lastName, firstName, year, average);
        table.insert(uid, s, collisions);

        updateYearlyAverages(yearlyAverages, year, average);
    }

    fin.close();
    return table;
}

int main() {
    string filename;
    cout << "Kerem adja meg a bemeneti filet: " << endl;
    cin >> filename;

    int numberOfStudents, maxStudents;
    //Egy 2 dimenzios tombot hasznalunk az atlag szamitasara.
    //Az ev utolso ket szamjegyenek indexere helyezzuk be az az evi atlagot, es hogyha akad meg egy ev azzal a ket szamjeggyel kibovitjuk azt az indexet
    vector<vector<YearlyData>> yearlyAverages(100);

    HashTable<Student> students = inputFromFile(filename, maxStudents, numberOfStudents, yearlyAverages);

    bool ok = false;
    string input;
    while (!ok) {
        cout << "Adja meg a parancs sorszamat: " << endl;
        cout << "1. Beillesztes" << endl;
        cout << "2. Torles" << endl;
        cout << "3. Kereses" << endl;
        cout << "4. Hallgatok kiirasa" << endl;
        cout << "5. Evi atlagok osszehasonlitasa" << endl;
        cout << "6. Kilepes" << endl << endl;
        cin >> input;
        system("cls");

        if (input == "1") { // Beillesztes
            string lastname, firstname, year;
            int uid;
            double average;
            cout << "Kerem adjon meg egy vezeteknevet: ";
            cin >> lastname;
            cout << "Kerem adjon meg egy keresztnevet: ";
            cin >> firstname;
            cout << "Kerem adjon meg egy azonositoszamot: ";
            cin >> uid;
            while (cin.fail()) { // Ha a UID nem int, akkor addig kerjuk ameddig helyes lesz
                cout << "Az azonositoszam nem egy szam." << endl;
                cin.clear();
                cin.ignore(256, '\n'); // ha helytelen a bemenet a bufferben meg benne lesz, ezert ignoralnunk kell.
                cout << "Adja meg ujra az azonositoszamot: ";
                cin >> uid;
            }

            cout << "Adja meg az evfolyamot: ";
            cin >> year;
            cout << "Kerem adja meg a hallgato atlagat: ";
            cin >> average;
            while (average > 10 || average < 0) {
                cout << "Helytelen a megadott atlag. Kerem adja meg ujra: ";
                cin >> average;
            }
            Student student(uid, lastname, firstname, year, average);
            int collisions = 0;
            int yearlyDataCollisions = 0;
            if (students.insert(uid, student, collisions)) {
                updateYearlyAverages(yearlyAverages, year, average);
                numberOfStudents++;
                cout << "Hallgato hozzaadva" << endl;
            }
            else {
                cout << "Sikertelen hozzaadas. Letezik mar vagy a helyek szama betelt." << endl;
            }
        }
        if (input == "2") { // Torles
            int uid;
            cout << "Kerem adja meg az azonositoszamot amelyet torolni szeretne: ";
            cin >> uid;
            while (cin.fail()) { // Ha a UID nem int, akkor addig kerjuk ameddig helyes lesz
                cout << "Az azonositoszam nem egy szam." << endl;
                cin.clear();
                cin.ignore(256, '\n'); // ha helytelen a bemenet a bufferben meg benne lesz, ezert ignoralnunk kell.
                cout << "Adja meg ujra az azonositoszamot: ";
                cin >> uid;
            }
            cout << "Kereses...";
            Student student;
            if (students.find(uid, student)) {
                removeYearlyAverage(yearlyAverages, student.year, student.getAverage());
                students.remove(uid);
                numberOfStudents--;
                cout << "Hallgato torolve." << endl;
            }
            else {
                cout << "Nincs talalat erre az azonositoszamra." << endl;
            }
        }
        else if (input == "3") { //Kereses
            int uid;
            Student returned; //Ez lesz a student akit visszaterit a fuggveny
            cout << "Adja meg a keresendo szemely azonositoszamat: ";
            cin >> uid;
            while (cin.fail()) { //Ha a UID nem int, akkor addig kerjuk ameddig helyes lesz
                cout << "Az azonositoszam nem egy szam." << endl;
                cin.clear();
                cout << "Adja meg ujra az azonositoszamot: ";
                cin >> uid;
            }
            cout << "Kereses... ";
            if (students.find(uid, returned)) {
                cout << "Hallgato megtalalva" << endl;
                cout << "----------------------------" << endl;
                cout << "Csaladnev: " << returned.lastName << endl;
                cout << "Keresztnev: " << returned.firstName << endl;
                cout << "Azonositoszam: " << returned.uid << endl;
                cout << "Ev: " << returned.year << endl;
                cout << "----------------------------" << endl;
            }
            else {
                cout << "Nincs talalat erre az azonositoszamra." << endl;
            }
        }
        else if (input == "4") { // Hallgatok kiirasa
            cout << setw(10) << left << "Azonosito " << setw(20) << left << "Csaladnev " << setw(20) << left << "Keresztnev " << setw(5) << left << "Ev" << endl;
            cout << "--------------------------------------------------------" << endl;
            for (int i = 0; i < students.maxHash; ++i) {
                if (students.a[i].isNormal()) {
                    cout << students.a[i].getValue();
                }
            }
        }
        else if (input == "5") { // Evi atlagok osszehasonlitasa
            string year;
            cout << "Adja meg az evet amelyet ossze szeretne hasonlitani az elozo evvel: ";
            cin >> year;
            compareYearlyAverages(yearlyAverages, year);
        }
        else if (input == "6") { // Kilepes
            ok = true;
            updateFile(filename, students, numberOfStudents, maxStudents);
        }
        else {
            cout << "Helytelen parancs." << endl;
        }
    }

    return 0;
}