#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>

#ifndef vaccineMonitorUtilHeader
#define vaccineMonitorUtilHeader
#include "../lib/headers/vaccineMonitorUtil.h"
#endif

#ifndef DataStructHeader
#define DataStructHeader
#include "../lib/structures/DataStruct.h"
#endif

#ifndef hashTableHeader
#define hashTableHeader
#include "../lib/headers/hashTable.h"
#endif

#ifndef skipListHeader
#define skipListHeader
#include "../lib/headers/skipList.h"
#endif

#ifndef myLibHeader
#define myLibHeader
#include "../lib/headers/myLib.h"
#endif

#ifndef bloomFilterHeader
#define bloomFilterHeader
#include "../lib/headers/bloomFilter.h"
#endif

#ifndef hashFunctionsHeader
#define hashFunctionsHeader
#include "../lib/headers/hashFunctions.h"
#endif

void parametersCheck(DataPointer* Universal, int argc, char **argv)
{
    int *bloomSize = Universal->bloomSizePointer; 
    char **filename = Universal->filenamePointer;
    if(argc<4){
        printf("Wrong parameters\n./vaccineMonitor -c citizenRecordsFile –b bloomSize\n");
        exit(1);
    }
    if(!strcmp("-b", argv[1]))
        (*bloomSize)=atoi(argv[2]);
    else if(!strcmp("-b", argv[3]))
        (*bloomSize)=atoi(argv[4]);
    else{
        printf("Wrong parameters\n./vaccineMonitor -c citizenRecordsFile –b bloomSize\n");
        exit(1);
    }
    if(!strcmp("-c", argv[1])){
        (*filename)=(char*) malloc(sizeof(char)*(strlen(argv[2])+1));
        strcpy((*filename), argv[2]);
    }
    else if(!strcmp("-c", argv[3])){
        (*filename)=(char*) malloc(sizeof(char)*(strlen(argv[4])+1));
        strcpy((*filename), argv[4]);
    }
    else{
        printf("Wrong parameters\n./vaccineMonitor -c citizenRecordsFile –b bloomSize\n");
        exit(1);
    }

    srand(time(NULL)); //initialize pseudo-random number generator for coinFlip function that will be used in the skipList creation
}

void parseInputFile(DataPointer* Universal)
{
    int filter, index;
    char *input, **word;
    char *citizenID = NULL, *name = NULL, *surname = NULL, *country = NULL, *age = NULL, *virus = NULL, *vaccinated = NULL, *date = NULL;
    char **vacced = *(Universal->vaccedPointer); 
    input = (char*) malloc(sizeof(char)*201); 
    word = (char**) malloc(sizeof(char*)*5);
    char *filename = *(Universal->filenamePointer);
    FILE *citizenRecordsFile = fopen(filename,"r");
    Record *NodePointer;
    Record **Registry = *(Universal->RegistryPointer);
    int *inputSize = Universal->inputSizePointer;
    int duplicate = 0; 

    if(citizenRecordsFile==NULL){
        printf("ERROR: %s did not open\n",filename);
        exit(1);
    }

    while (fgets(input,200,citizenRecordsFile)!=NULL){
        //input[strcspn(input, "\n")] = 0;  //delete newline character from the string if it exists
        citizenID = strtok (input," \t\r\n");
        name = strtok (NULL, " \t\r\n");
        surname = strtok (NULL, " \t\r\n");
        country = strtok (NULL, " \t\r\n");
        age = strtok (NULL, " \t\r\n");
        virus = strtok (NULL, " \t\r\n");
        vaccinated = strtok (NULL, " \t\r\n");
        date = strtok (NULL, " \t\r\n");

        if(vaccinated!=NULL)
        {
            if(!strcmp(vaccinated, "YES"))
                word[4]=vacced[1];
            else
                word[4]=vacced[0];
        }
        else
        {
            printf("ERROR IN RECORD %s %s %s %s %s %s %s %s\n",citizenID, name, surname, country, age, virus, vaccinated, date!=NULL ? date : "");
            continue;
        }

        word[0]=virus;
        word[2]=country;
        if(virus_exists(Universal, &word, &filter, &index)==0)
            create_datatypes(Universal);

        duplicate = existsIn_hashtable(Registry[my_hash(atoi(citizenID))], &NodePointer, citizenID, virus);
        if(!strcmp(word[4], "NO") && date!=NULL){
            printf("ERROR IN RECORD %s %s %s %s %s %s %s %s\n",citizenID, name, surname, country, age, virus, vaccinated, date);
            continue;
        }
        else if(!duplicate) //no duplicates
        {
            insertAt_hashtable(&Registry[my_hash(atoi(citizenID))], citizenID, name, surname, word[3], age, word[1], word[4], date);
            (*inputSize)++;
        }
        else if(duplicate){
            printf("ERROR IN RECORD %s %s %s %s %s %s %s %s\n", citizenID, name, surname, country, age, virus, vaccinated, date!=NULL ? date : "");
            continue;
        }
    }
    fclose(citizenRecordsFile);
    free(input);
    free(word);    
}

void storeDataIntoDatatypes(DataPointer* Universal)
{
    Record *NodePointer = NULL;
    Record **Registry = *(Universal->RegistryPointer);
    Sentinel **skiplist = *(Universal->skipListPointer);
    char **bloomfilter = *(Universal->bloomFilterPointer);
    char *citizenID = NULL, *name = NULL, *surname = NULL, *country = NULL, *age = NULL, *virus = NULL, *vaccinated = NULL, *date = NULL;
    char **vacced = *(Universal->vaccedPointer); 
    char **word = (char**) malloc(sizeof(char*)*4);
    char *bloomString = (char*) malloc(sizeof(char));
    int j=0, k=1, filter, index, bloomCheck, first=1;
    int inputSize = *(Universal->inputSizePointer);

    if(inputSize>=2)
        k = abs(log2(inputSize)); 
    for(int i=0; i<*(Universal->NskipListsPointer); i++)
    {            
        skiplist[i]->NSkipLevels = k;
        skiplist[i]->next = (Node**) realloc(skiplist[i]->next, sizeof(Node*)*k);
        for(int j=0; j<k; j++)
            skiplist[i]->next[j] = NULL;
    } 
    int in=1;
    for(int i=0; i<10; i++)
    {
        k=1;
        j=0;
        while(k)
        {
            k = exportFrom_hashtable(Registry[i], &NodePointer, &citizenID/*, &name, &surname, &country, &age*/, &word[0], &vaccinated/*, &date*/, j, first);
            if(k==0) break;
            if(first)
                first--;
            //if(NodePointer==NULL)
            //    continue;
            //printf("%d %s\n",in++,citizenID);
            word[2]=NULL;
            virus_exists(Universal, &word, &filter, &index); //is called to get the index of the bloomFilter of the current virus
            if(vaccinated==vacced[1])
            {
                bloomString = (char*) realloc(bloomString, sizeof(char)*( strlen(citizenID) + strlen(word[1]) + 1 ) );
                snprintf(bloomString, strlen(citizenID)+strlen(word[1]) + 1, "%s%s", citizenID, word[1]);
                insertAt_bloomFilter( bloomfilter,  filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
                bloomCheck = isItemIn_bloomFilter( bloomfilter, filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
                //printf("Check for '%s' in filter=%d: %d\n",citizenID,filter,bloomCheck);
                insertAt_skipList(&(skiplist[2*filter]), NodePointer);
                //printll(skiplist[2*filter]->next[0],0);
            }
            else if(vaccinated==vacced[0]){
                insertAt_skipList(&(skiplist[2*filter+1]), NodePointer);
                //printll(skiplist[2*filter+1]->next[0],0);
            }
            j++;
        }
    }
    free(word);
    free(bloomString);
    free(citizenID);
}

void readInputFromKeyboard(DataPointer* Universal)
{
    char *input, **argument;
    input = (char*) malloc(sizeof(char)*101); 
    argument = (char**) malloc(sizeof(char*)*9);
    int false = 0;
    if(input==NULL){
        printf("Memory Allocation Error\n");
        exit(1);
    }
    while(1)
    {
        printf("\033[1;34mvaccineMonitor\033[0m:\033[1;32m~\033[0m$ ");
        fgets(input,101 , stdin);
        input[strcspn(input, "\n")] = 0;  //delete newline character from the string if it exists
        
        //divide the line into tokens-words
        argument[0] = strtok(input," ");
        argument[1] = strtok(NULL, " ");
        argument[2] = strtok(NULL, " ");
        argument[3] = strtok(NULL, " ");
        argument[4] = strtok(NULL, " ");
        argument[5] = strtok(NULL, " ");
        argument[6] = strtok(NULL, " ");
        argument[7] = strtok(NULL, " ");
        argument[8] = strtok(NULL, " ");

        //call the right function according to the arguments
        if(!strcmp(argument[0], "/vaccineStatusBloom"))
            vaccineStatusBloom(Universal, argument[1], argument[2]);
        else if(!strcmp(argument[0], "/vaccineStatus"))
            vaccineStatus(Universal, argument[1], argument[2]);
        else if(!strcmp(argument[0], "/populationStatus"))
            populationStatus(Universal, argument[1], argument[2], argument[3], argument[4]);
        else if(!strcmp(argument[0], "/popStatusByAge"))
            popStatusByAge(Universal, argument[1], argument[2], argument[3], argument[4]);
        else if(!strcmp(argument[0], "/list-nonVaccinated-Persons"))
            list_nonVaccinated_Persons(Universal, argument[1]);      
        else if(!strcmp(argument[0], "/insertCitizenRecord"))
            insertCitizenRecord(Universal, argument[1], argument[2], argument[3], argument[4], argument[5], argument[6], argument[7], argument[8]);
        else if(!strcmp(argument[0], "/vaccinateNow"))
            vaccinateNow(Universal, argument[1], argument[2], argument[3], argument[4], argument[5], argument[6]);
        else if(!strcmp(input, "/exit") || !strcmp(input, "quit") || !strcmp(input, "q")){
            exitNow(&Universal);
            break;
        }else{
            printf("vaccineMonitor: %s: command not found\n", input);
            if(false)
                //printf("valid commands are:\n\033[0;32m/vaccineStatusBloom \033[0;0mor\033[0;32m\n/vaccineStatus \033[0;0mor\033[0;32m\n/populationStatus \033[0;0mor\033[0;32m\n"
                //        "/popStatusByAge \033[0;0mor\033[0;32m\n/list-nonVaccinated-Persons \033[0;0mor\033[0;32m\n/insertCitizenRecord \033[0;0mor\033[0;32m\n/exit\n\033[0;0m");
                printf("valid commands are: \033[0;32m/vaccineStatusBloom \033[0;0mor\033[0;32m " 
                       "/vaccineStatus \033[0;0mor\033[0;32m /populationStatus \033[0;0mor\033[0;32m "
                       "/popStatusByAge \033[0;0mor\033[0;32m /list-nonVaccinated-Persons \033[0;0mor "
                       "\033[0;32m /insertCitizenRecord \033[0;0mor\033[0;32m /exit\n\033[0;0m");
            false++;
        }
    }
    free(input);
    free(argument);
}

void vaccineStatusBloom(DataPointer *Universal, char *citizenID, char *virusName)
{
    char **bloomfilter = *(Universal->bloomFilterPointer);
    int filter, index, bloomCheck;
    if(citizenID==NULL)
    {
        printf("ERROR: citizenID not given\n");
        return;
    }
    if(virusName==NULL)
    {
        printf("ERROR: virusName not given\n");
        return;
    } 
    char *bloomString = (char*) malloc(sizeof(char));
    char **word = (char**) malloc(sizeof(char*)*4);
    word[0]=virusName;
    word[2]=NULL;
    if(virus_exists(Universal, &word, &filter, &index))
    {
        bloomString = (char*) realloc(bloomString, sizeof(char)*( strlen(citizenID) + strlen(word[1]) + 1 ) );
        snprintf(bloomString, strlen(citizenID)+strlen(word[1]) + 1, "%s%s", citizenID, word[1]);
        bloomCheck=isItemIn_bloomFilter(bloomfilter, filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
        //printf("Check for '%s' in filter=%d: %d\n",citizenID,filter,bloomCheck);
        if(!bloomCheck)
            printf("\x1b[31mNOT VACCINATED\x1b[0m\n");
        else
            printf("\x1b[32mMAYBE\x1b[0m\n");
    }
    else
        printf("Bloom filter for the virus %s is non-existent.\n",virusName);
    free(word);
    free(bloomString);
}

void vaccineStatus(DataPointer *Universal, char *citizenID, char *virusName)
{
    char **word;
    if(citizenID==NULL)
    {
        printf("ERROR: citizenID not given\n");
        return;
    }
    word = (char**) malloc(sizeof(char*)*3); 
    int filter, index; 
    Sentinel **skiplist = *(Universal->skipListPointer);
    char **virusData = *(Universal->virusDataPointer);

    if(virusName==NULL)
    {
        //printf("vaccineStatus NULL_virus\n");
        for(int i=0; i<*(Universal->NskipListsPointer); i++)
        {
            if(i%2==0)
            {
                word[0] = citizenID;
                word[1] = virusData[i/2];
                if(search_skipList(&(skiplist[i]->next[0]), word, (skiplist[i])->NSkipLevels))
                {
                    printf("\x1b[32m%s YES %s\x1b[0m\n", virusData[i/2], word[2]);
                }
                else
                {
                    printf("\x1b[31m%s NO\x1b[0m\n", virusData[i/2]);
                }  
            }
        }
    }
    else if(virusName!=NULL)
    {
        //printf("vaccineStatus 2_args\n");
        word[0]=virusName;
        word[2]=NULL;
        if(virus_exists(Universal, &word, &filter, &index))
        {
            //("Searching in Skip List %d\n",2*filter);
            word[0] = citizenID;
            word[1] = virusName;
            if(search_skipList(&(skiplist[2*filter]->next[0]), word, (skiplist[2*filter])->NSkipLevels))
            {
                printf("\x1b[32mVACCINATED ON %s\x1b[0m\n", word[2]);
                
            }
            else
                printf("\x1b[31mNOT VACCINATED\x1b[0m\n");
        }
        else
        {
            printf("Skip List for the virus %s is non-existent.\n",virusName);
        }
    }
    free(word);
}

void populationStatus(DataPointer *Universal, char *argument1, char *argument2, char *argument3, char *argument4)
{
    if(argument1==NULL)
    {
        printf("ERROR: argument not given\n");
        return;
    }
    /*if(argument3!=NULL && argument4==NULL)
    {
        printf("ERROR: wrong date arguments\n");
        return;
    }*/
    int countryFound=0, virusFound=0, index, filter;
    char **virusData = *(Universal->virusDataPointer);
    char **countryData = *(Universal->countryDataPointer);
    //check if first argument is a country
    if(argument1!=NULL) 
    {       
        for(int i=0; i<*(Universal->NcountriesPointer); i++)
        {
            if(!strcmp(argument1, countryData[i])){
                index=i;
                countryFound = 1;
            }
        }
    }
    //if no arguments were given print error
    else{
        printf("ERROR: wrong arguments\n");
        return;
    }
    //if first argument is a country, check if second argument is a virus
    if(countryFound)
    {   
        countryFound=0;
        //printf("%s =%s\n",countryData[index],argument1);
        if(argument2!=NULL)        
            for(int i=0; i<*(Universal->NbloomFiltersPointer); i++)
            {
                if(!strcmp(argument2, virusData[i]))
                {
                    filter=i;
                    virusFound = 1;
                }    
            }
        else{
            printf("ERROR: no virus\n");
            return;
        }
        if(virusFound){
            virusFound=0;
            get_Statistics(*(Universal->skipListPointer), argument1, argument3, argument4, 2*filter, *(Universal->countryDataPointer), *(Universal->NcountriesPointer), 0);
        }
    }
    //if first argument is not a country, check if first argument is a virus
    else
    { 
        if(argument1!=NULL)        
            for(int i=0; i<*(Universal->NbloomFiltersPointer); i++)
            {
                if(!strcmp(argument1, virusData[i]))
                {
                    filter=i;
                    virusFound = 1;
                }    
            }
        else{
            printf("ERROR: no virus\n");
            return;
        }
        if(virusFound){
            virusFound=0;
            get_Statistics(*(Universal->skipListPointer), NULL, argument2, argument3, 2*filter, *(Universal->countryDataPointer), *(Universal->NcountriesPointer), 0);
        }
    }
}

void popStatusByAge(DataPointer *Universal, char *argument1, char *argument2, char *argument3, char *argument4)
{
    if(argument1==NULL)
    {
        printf("ERROR: argument not given\n");
        return;
    }
    /*if(argument3!=NULL && argument4==NULL)
    {
        printf("ERROR: wrong date arguments\n");
        return;
    }*/
    int countryFound=0, virusFound=0, index, filter;
    char **virusData = *(Universal->virusDataPointer);
    char **countryData = *(Universal->countryDataPointer);
    //check if first argument is a country
    if(argument1!=NULL) 
    {       
        for(int i=0; i<*(Universal->NcountriesPointer); i++)
        {
            if(!strcmp(argument1, countryData[i])){
                index=i;
                countryFound = 1;
            }
        }
    }
    else{
        printf("ERROR: wrong arguments\n");
        return;
    }
    if(countryFound)
    {   
        countryFound=0;
        if(argument2!=NULL)        
            for(int i=0; i<*(Universal->NbloomFiltersPointer); i++)
            {
                if(!strcmp(argument2, virusData[i]))
                {
                    filter=i;
                    virusFound = 1;
                }    
            }
        else{
            printf("ERROR: no virus\n");
            return;
        }
        if(virusFound){
            virusFound=0;
            get_Statistics(*(Universal->skipListPointer), argument1, argument3, argument4, 2*filter, *(Universal->countryDataPointer), *(Universal->NcountriesPointer), 1);
        }
    }
    else
    { 
        if(argument1!=NULL)        
            for(int i=0; i<*(Universal->NbloomFiltersPointer); i++)
            {
                if(!strcmp(argument1, virusData[i]))
                {
                    filter=i;
                    virusFound = 1;
                }    
            }
        else{
            printf("ERROR: no virus\n");
            return;
        }
        if(virusFound){
            virusFound=0;
            get_Statistics(*(Universal->skipListPointer), NULL, argument2, argument3, 2*filter, *(Universal->countryDataPointer), *(Universal->NcountriesPointer), 1);
        }
    }
}

void insertCitizenRecord(DataPointer *Universal, char *citizenID, char *firstName, char *lastName, char *country, char *age, char *virusName, char *vaccinated, char *date)
{
    if(citizenID==NULL || firstName==NULL || lastName==NULL || country==NULL || age==NULL || virusName==NULL || vaccinated==NULL)
    {
        printf("ERROR: wrong arguments given\n");
        return;
    }
    if(!strcmp(vaccinated, "YES") && date==NULL)
    {
        printf("ERROR: no date given\n");
        return;
    }
    int filter, index, bloomCheck, duplicate;
    char **word = (char**) malloc(sizeof(char*)*5); 
    char **vacced = *(Universal->vaccedPointer);
    char *bloomString = (char*) malloc(sizeof(char));
    Record* NodePointer;
    Record **Registry = *(Universal->RegistryPointer);
    Sentinel **skiplist = *(Universal->skipListPointer);
    word[0]=virusName;
    word[2]=country;            
    if(!strcmp(vaccinated, "YES"))
        word[4]=vacced[1];
    else
        word[4]=vacced[0];
    duplicate = existsIn_hashtable(Registry[my_hash(atoi(citizenID))], &NodePointer, citizenID, virusName);

    //if record has typo error
    if(!strcmp(word[4], "NO") && date!=NULL)
        printf("ERROR IN RECORD %s %s %s %s %s %s %s %s\n",citizenID, firstName, lastName, country, age, virusName, vaccinated, date);
    //if record doesnt already exist in the registry add it to the datatypes, if it does exist returns 0 and we have a pointer to the specific node
    if(!duplicate) 
    {                         
        if(virus_exists(Universal, &word, &filter, &index)==0)
            create_datatypes(Universal);
        skiplist = *(Universal->skipListPointer);
        NodePointer = insertAt_hashtable(&Registry[my_hash(atoi(citizenID))], citizenID, firstName, lastName, word[3], age, word[1], word[4], date);
        if(!strcmp(vaccinated, "YES"))
        {
            bloomString = (char*) realloc(bloomString, sizeof(char)*( strlen(citizenID) + strlen(word[1]) + 1 ) );
        snprintf(bloomString, strlen(citizenID)+strlen(word[1]) + 1, "%s%s", citizenID, word[1]);
            insertAt_bloomFilter( *(Universal->bloomFilterPointer),  filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
            bloomCheck = isItemIn_bloomFilter( *(Universal->bloomFilterPointer), filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
            //printf("Check for '%s' in filter=%d: %d\n",citizenID,filter,bloomCheck);
            insertAt_skipList(&(skiplist[2*filter]), NodePointer);
        }
        else if(!strcmp(vaccinated, "NO"))
            insertAt_skipList(&(skiplist[2*filter+1]), NodePointer);            
    }
    //if record does exist 
    else if(duplicate)
    {
        //if current record does not define citizen as vaccinated for the virus and new insertion changes that, 
        //alter the Record node and remove it from the non-vaccinated skip list and add it to the vaccinated skip list
        if(!strcmp(NodePointer->vaccinated, "NO") && !strcmp(vaccinated, "YES"))
        {
            virus_exists(Universal, &word, &filter, &index); //is called to get the exact skipList and bloomFilter used for this virus
            deleteFrom_skipList(&(skiplist[2*filter+1]), NodePointer);
            alter_hashNode(NodePointer, word[4], date);
            insertAt_skipList(&(skiplist[2*filter]), NodePointer);
            bloomString = (char*) realloc(bloomString, sizeof(char)*( strlen(citizenID) + strlen(word[1]) + 1 ) );
            snprintf(bloomString, strlen(citizenID)+strlen(word[1]) + 1, "%s%s", citizenID, word[1]);
            insertAt_bloomFilter( *(Universal->bloomFilterPointer),  filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
            bloomCheck = isItemIn_bloomFilter( *(Universal->bloomFilterPointer), filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
            //printf("Check for '%s' in filter=%d: %d\n",citizenID,filter,bloomCheck);            
        }
        //if current record states that citizen is already vaccinated for the virus show error 
        else if(!strcmp(NodePointer->vaccinated, "YES"))
            printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s\n", NodePointer->citizenID, NodePointer->date);
        //dikh mou prosthiki, if current record states that citizen is non-vaccinated and new record tells the same , show error 
        else if(!strcmp(NodePointer->vaccinated, "NO") && !strcmp(vaccinated, "NO"))
            printf("ERROR: CITIZEN %s STATUS IS ALREADY DESCRIBED AS NON-VACCINATED FOR THE %s VIRUS\n", NodePointer->citizenID, NodePointer->virus);
    }
    free(word);
    free(bloomString);
}

void vaccinateNow(DataPointer *Universal, char *citizenID, char *firstName, char *lastName, char *country, char *age, char *virusName)
{
    if(citizenID==NULL || firstName==NULL || lastName==NULL || country==NULL || age==NULL || virusName==NULL)
    {
        printf("ERROR: wrong arguments given\n");
        return;
    }
    int filter, index, bloomCheck, duplicate; 
    int *inputSize = Universal->inputSizePointer;
    char *todays_date = (char*) malloc(sizeof(char)*11); 
    char **word = (char**) malloc(sizeof(char*)*5);
    char **vacced = *(Universal->vaccedPointer);
    char *bloomString = (char*) malloc(sizeof(char));  
    Record* NodePointer;
    Record **Registry = *(Universal->RegistryPointer);
    Sentinel **skiplist = *(Universal->skipListPointer);
    word[0]=virusName;
    word[2]=country;

    //resolve today's date 
    time_t t = time(NULL); 
    time(&t);
    struct tm tm = *localtime(&t);
    snprintf(todays_date, 11, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    //printf("Date of vaccination is %s\n",todays_date);

    duplicate = existsIn_hashtable(Registry[my_hash(atoi(citizenID))], &NodePointer, citizenID, virusName);
    //if record for citizenID and this virus exists
    if(duplicate)
    {
        //if citizenID is already vaccinated for the virus, show error
        if(!strcmp(NodePointer->vaccinated, "YES"))
        {
            printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s\n", NodePointer->citizenID, NodePointer->date);
        }
        //if citizenID is not vaccinated for the virus, vaccinate now
        else if(!strcmp(NodePointer->vaccinated, "NO"))
        {
            virus_exists(Universal, &word, &filter, &index); //is called to get the exact skipList used for this virus
            bloomString = (char*) realloc(bloomString, sizeof(char)*( strlen(citizenID) + strlen(word[1]) + 1 ) );
            snprintf(bloomString, strlen(citizenID)+strlen(word[1]) + 1, "%s%s", citizenID, word[1]);
            insertAt_bloomFilter( *(Universal->bloomFilterPointer),  filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
            bloomCheck = isItemIn_bloomFilter( *(Universal->bloomFilterPointer), filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
            //printf("Check for '%s' in filter=%d: %d\n",citizenID,filter,bloomCheck);
            deleteFrom_skipList(&(skiplist[2*filter+1]), NodePointer); 
            alter_hashNode(NodePointer, vacced[1], todays_date);
            insertAt_skipList(&(skiplist[2*filter]), NodePointer);            
        }
    }
    //if record for current citizenID and virus does not exist add to the datatypes
    else if(!duplicate)
    {
        if(virus_exists(Universal, &word, &filter, &index)==0)
            create_datatypes(Universal);
        NodePointer = insertAt_hashtable(&Registry[my_hash(atoi(citizenID))], citizenID, firstName, lastName, word[3], age, word[1], vacced[1], todays_date);
        (*inputSize)++;
        bloomString = (char*) realloc(bloomString, sizeof(char)*( strlen(citizenID) + strlen(word[1]) + 1 ) );
        snprintf(bloomString, strlen(citizenID)+strlen(word[1]) + 1, "%s%s", citizenID, word[1]);
        insertAt_bloomFilter( *(Universal->bloomFilterPointer),  filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
        bloomCheck = isItemIn_bloomFilter( *(Universal->bloomFilterPointer), filter, *(Universal->bloomSizePointer), (unsigned char*) bloomString);
        //printf("Check for '%s' in filter=%d: %d\n",citizenID,filter,bloomCheck);
        skiplist = *(Universal->skipListPointer);
        insertAt_skipList(&(skiplist[2*filter]), NodePointer);
    }
    free(todays_date);
    free(word);
    free(bloomString);
}

void list_nonVaccinated_Persons(DataPointer *Universal, char *virusName)
{
    char **virusData = *(Universal->virusDataPointer);
    Sentinel **skiplist = *(Universal->skipListPointer);
    for(int i=0; i<(*(Universal->NskipListsPointer))/2; i++)
    {
        if(strcmp(virusName, virusData[i]))
            continue;
        printll(skiplist[2*i+1]->next[0],0);
    }
}

void exitNow(DataPointer **Universal)
{
    DataPointer *temp = *Universal;
    Sentinel **skiplist = *(temp->skipListPointer);
    Record** Registry = *(temp->RegistryPointer);
    char **virusData = *(temp->virusDataPointer);
    char **countryData = *(temp->countryDataPointer);
    char **vacced = *(temp->vaccedPointer);
    char **bloomfilter = *(temp->bloomFilterPointer);
    char *filename = *(temp->filenamePointer);
    //free hashTable
    for(int i=0; i<10; i++)
		free_hashtable(Registry[i]);
    free(Registry);
    //free skipList
    for(int i=0; i<*(temp->NskipListsPointer); i++)
        free_skipList(&(skiplist[i]));
    free(skiplist);
    //free bloomFilter
    free_bloomFilter(&bloomfilter, *(temp->NbloomFiltersPointer));
    //free countryData array
    for(int i=0; i<*(temp->NcountriesPointer); i++)
        free(countryData[i]);
    free(countryData);
    //free virusData array
    for(int i=0; i<*(temp->NbloomFiltersPointer); i++)
        free(virusData[i]);
    free(virusData);
    //free vacced array
    for(int i=0; i<2; i++)
        free(vacced[i]);
    free(vacced);
    //free filename string
    free(filename);
    //free the Universal Datapointer 
    free(*Universal);
    //printf("\033[1;37mThanks for using the vaccineMonitor app!\n");
    setlocale(LC_ALL, "");
    printf("%ls", L"\033[1;37mΕυχαριστούμε που χρησιμοποιήσατε την εφαρμογή vaccineMonitor. Εις το επανιδείν!\033[0m\n");
}