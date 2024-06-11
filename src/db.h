#pragma once

#include <Arduino.h>

struct Person {
    char name[10];
    char caps;
    int id;
};

struct PersonCaps {
    char name[10];
    char caps;
};

struct ListCaps {
    PersonCaps* list;
    char size;
};



bool addPerson(Person p);

bool removePerson(char person_id);

char addCaps(char quantity, int person_id);

char borrowCaps(char quantity, int person_id);

char seeCaps(char person_id);

ListCaps seeAllCaps();

void setuDb();



