#include "db.h"

#include <EEPROM.h>

unsigned char persons_size = 0;

Person* persons = new Person[10];

Person addPerson(Person p) {
    for (unsigned char i = 0; i< persons_size; i++){
        if (String(p.id) == String(persons[i].id)) 
            return p;
    }
    persons[persons_size] = p;
    persons_size++;
    return p;
}
// bool removePerson(char person_id);

int addCaps(int quantity, char person_id[10]) {
    if (quantity <= 0 || quantity >= 255)
        return -1;
    for (unsigned char i = 0; i < persons_size; i++) {
        if (String(persons[i].id) == String(person_id)) {
            persons[i].caps += quantity;
            return persons[i].caps;
        }
    }
    return -1;
}

int borrowCaps(char quantity, char person_id[10]) {
    if (quantity <= 0)
        return -1;
    for (unsigned char i = 0; i < persons_size; i++) {
        if (String(persons[i].id) == String(person_id)) {
            persons[i].caps -= quantity;
            return persons[i].caps;
        }
    }
    return -1;
}

int seeCaps(char person_id[10]) {
    for (unsigned char i = 0; i < persons_size; i++, persons++) {
        if (persons[persons_size].id == person_id) {
            return persons[i].caps;
        }
    }
    return -1;
}

ListCaps seeAllCaps() {
    ListCaps list;
    list.size = persons_size;
    for (unsigned char i = 0; i < persons_size; i++) {
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