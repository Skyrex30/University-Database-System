#pragma once
#include <iostream>

using namespace std;

enum SlotType {normalSlot, emptySlot, deletedSlot};

template <class T> class Slot {
	int key;
	SlotType type;

public:
	T value;
	Slot() {
		key = 0;
		type = emptySlot;
	}

	Slot(int newkey, T newvalue) {
		key = newkey;
		value = newvalue;
		type = normalSlot;
	}

	void kill() {
		type = deletedSlot;
	}

	int getKey() const {
		return key;
	}

	T getValue() const {
		return value;
	}

	bool isEmpty() const {
		return(type == emptySlot);
	}

	bool isNormal() const {
		return(type == normalSlot);
	}

	bool isDeleted() const {
		return (type == deletedSlot);
	}
};

