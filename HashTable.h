#pragma once
#include <iostream>
#include <algorithm>
#include "Student.h"
#include "Slot.h"

using namespace std;

template <class T> class HashTable {
private:
	int slotCount = 0;
	//hash fuggveny
	//forras: https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
	unsigned int hash(unsigned int x, int tableSize) {
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		return x % tableSize;
	}

	void rehash() {
		int oldMaxHash = maxHash;
		maxHash = nextPrime(maxHash * 2); //megduplazzuk a mostani meretet es megkeressuk a legkozelebbi prim szamot

		Slot<T>* newTable = new Slot<T>[maxHash];
		Slot<T>* oldTable = a;

		a = newTable;
		slotCount = 0;

		for (int i = 0; i < oldMaxHash; i++) {
			if (oldTable[i].isNormal()) {
				int newLocation = hash(oldTable[i].getKey(), maxHash);
				while (!newTable[newLocation].isEmpty() && !newTable[newLocation].isDeleted()) {
					newLocation++;
					if (newLocation == maxHash) newLocation = 0;
				}
				newTable[newLocation] = oldTable[i];
				slotCount++;
			}
		}

		delete[] oldTable;
	}

	int nextPrime(int n) {
		while (true) {
			if (isPrime(n)) return n;
			n++;
		}
	}

	bool isPrime(int n) {
		if (n <= 1) return false;
		if (n <= 3) return true;
		if (n % 2 == 0 || n % 3 == 0) return false;
		for (int i = 5; i * i <= n; i += 6) {
			if (n % i == 0 || n % (i + 2) == 0) return false;
		}
		return true;
	}
public:
	int maxHash;
	//Slot: egy olyan osztaly amely tartalmazza a kulcsot, az erteket, es hogy milyen tipusu az adott slot (hasznalt, ures, torolt)
	Slot<T>* a; 

	//konstruktor
	HashTable(int max) {
		maxHash = nextPrime(max); 
		a = new Slot<T>[maxHash]; 
	}

	//destruktor
	~HashTable() {
		delete[] a; 
	}


	// Behelyez egy kulcsot egy ertekkel es visszajelzi ha sikeres volt vagy sem
	bool insert(int key, T value, int& collisions) {
		if (slotCount >= maxHash * 0.6) { // ha betelt a tabla 60%-a rehashelunk
			rehash();
		}
		int location = hash(key, maxHash);
		int startLocation = location;
		bool inserted = false;
		if (slotCount == maxHash) { //hogyha megtelt nem lehet beszurni
			return false;
		}
		while (!inserted) {
			if (a[location].isEmpty() || a[location].isDeleted()) {
				a[location] = Slot<T>(key, value); 
				slotCount++;
				return true;
			}
			else if (a[location].isNormal()) {
				if (a[location].getKey() == key) { 
					return false;
				}
				else {
					collisions++;
					location++;
					if (location == maxHash) location = 0;
					if (location == startLocation) return false;
				}
			}
		}
		return false;
	}
	// Torol valamit egy kulcs alapjan es visszateriti ha sikeres
	bool remove(int key) {
		int location = hash(key, maxHash);
		int startLocation = location;
		bool removed = false;
		while (!removed) {
			if (a[location].getKey() == key) {
				a[location] = Slot<T>(); //toroljuk
				a[location].kill(); //Atallitjuk az allapotat deleted-e
				slotCount--;
				return true;
			}
			if (a[location].isDeleted() || a[location].isEmpty()) {
				return false;
			}
			if (a[location].isNormal()) { //Ha nem egyezik meg a kulcs azt jelenti tovabb kell mennunk
				location++;
				if (location == maxHash) location = 0;
				if (location == startLocation) return false;
			}
		}
		return false;
	}
	// Kereso fuggveny amely visszateriti ha sikeres
	bool find(int key, T& value) {
		int location = hash(key, maxHash);
		int startLocation = location;
		bool found = false;
		while (!found) {
			if (a[location].getKey() == key) { //Ha megvan a kulcs akkor a value erteket atirjuk
				value = a[location].getValue();
				return true;
			}
			if (a[location].isDeleted() || a[location].isEmpty()) { //Ha a hely torolve van vagy ures akkor nincs mit visszateriteni
				return false;
			}
			if (a[location].isNormal()) {
				location++;
				if (location == maxHash) location = 0;
				if (location == startLocation) return false;
			}
		}
		return false;
	}


	int getSlotCount() {
		return slotCount;
	}

	friend ostream& operator<<(ostream& os, const HashTable& that) {
		for (int i = 0; i < maxHash; i++) { 
			if (that.a[i].isNormal()) { 
				os << "Slot: " << i << " Kulcs: " << that.a[i].getKey(); 
				os << that.a[i].getValue();
			}
		}
		return os;
	}
};