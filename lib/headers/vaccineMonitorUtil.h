#include <stdio.h>

#ifndef DataStructHeader
#define DataStructHeader
#include "../structures/DataStruct.h"
#endif

void parametersCheck(DataPointer*, int, char**);
void parseInputFile(DataPointer*);
void storeDataIntoDatatypes(DataPointer*);
void readInputFromKeyboard(DataPointer*);

void vaccineStatusBloom(DataPointer*, char*, char*); 
void vaccineStatus(DataPointer*, char*, char*);
void populationStatus(DataPointer*, char*, char*, char*, char*);
void popStatusByAge(DataPointer*, char*, char*, char*, char*);
void insertCitizenRecord(DataPointer*, char*, char*, char*, char*, char*, char*, char*, char*);
void vaccinateNow(DataPointer*, char*, char*, char*, char*, char*, char*);
void list_nonVaccinated_Persons(DataPointer*, char*);
void exitNow(DataPointer**);

