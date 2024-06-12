#pragma once

#include <Arduino.h>

struct Person {
    char name[10];
    int caps;
    char id[10];
};

struct PersonCaps {
    char name[10];
    char caps;
};

struct ListCaps {
    PersonCaps* list;
    char size;
};



Person addPerson(Person p);

bool removePerson(char person_id);

int addCaps(int quantity, char person_id[10]);

int borrowCaps(char quantity, char person_id[10]);

int seeCaps(char person_id[10]);

ListCaps seeAllCaps();

void setuDb();



