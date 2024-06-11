#include "db.h"

#include <EEPROM.h>

char persons_size = 0;

Person* persons;

bool addPerson(Person p) {
    persons[persons_size] = p;
    persons_size++;
    return true;
}
// bool removePerson(char person_id);

char addCaps(char quantity, int person_id) {
    if (quantity <= 0)
        return -1;
    for (char i = 0; i < persons_size; i++) {
        if (persons[i].id == person_id) {
            persons[i].caps += quantity;
            return persons[i].caps;
        }
    }
    return -1;
}

char borrowCaps(char quantity, int person_id) {
    if (quantity <= 0)
        return -1;
    for (char i = 0; i < persons_size; i++) {
        if (persons[i].id == person_id && persons[i].caps >= quantity) {
            persons[i].caps += quantity;
            return persons[i].caps;
        }
    }
    return -1;
}

char seeCaps(char person_id) {
    for (char i = 0; i < persons_size; i++, persons++) {
        if (persons[persons_size].id == person_id) {
            return persons[i].caps;
        }
    }
    return -1;
}

ListCaps seeAllCaps() {
    ListCaps list;
    list.size = persons_size;
    for (char i = 0; i < persons_size; i++) {
        PersonCaps allCaps;
        memccpy(allCaps.name, persons[i].name, 0, 10);
        allCaps.caps = persons[i].caps;
        // allCaps.name = persons[i].name;

        list.list = &allCaps;
        list.list++;
    }
    return list;
}

void setuDb() {
    
}

void saveDb() {
}