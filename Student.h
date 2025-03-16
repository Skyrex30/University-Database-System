#pragma once
#include <iostream>
#include <iomanip>
#include <random>
#include <string>

using namespace std;

class Student {
	double average;
public:
	int uid;
	string firstName;
	string lastName;
	string year;

	Student() {
		average = 0;
		uid = 0;
	}
	// Konstruktor
	Student(int newUid, string newLname, string newFname, string newYear, double newAverage) { 
		uid = newUid;
		firstName = newFname;
		lastName = newLname;
		year = newYear;
		average = newAverage;
	}


	void setAverage(double newAverage) {
		average = newAverage;
		return;
	}

	double getAverage() {
		return average;
	}
	int getUid() const { 
		return uid; 
	}

	string getLastName() const {
		return lastName; 
	}

	string getFirstName() const { 
		return firstName; 
	}

	string getYear() const { 
		return year; 
	}

	friend ostream& operator <<(ostream& os, const Student& that) {
		int uidWidth = 10; 
		int lastNameWidth = 20; 
		int firstNameWidth = 20; 
		int yearWidth = 5;

		os  << setw(uidWidth) << left << that.uid
			<< setw(lastNameWidth) << left << that.lastName
			<< setw(firstNameWidth) << left << that.firstName
			<< setw(yearWidth) << left << that.year << endl;

		return os;
	}

};