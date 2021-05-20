#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#ifndef DataStructHeader
#define DataStructHeader
#include "../lib/structures/DataStruct.h"
#endif

#ifndef skipStructHeader
#define skipStructHeader
#include "../lib/structures/skipStruct.h"
#endif

//stores pointers to the important data of the program to a single struct so that access to them can be easily accomplished
void uploadData(DataPointer **Universal, Record ***Registry, Sentinel ***skipList, char ***bloomFilter, char ***countryData, 
                char ***virusData, char ***vacced, char **filename, int *NbloomFilters, int *NskipLists, int *bloomSize, int *Ncountries, int *inputSize)
{
    (*Universal) = (DataPointer*) malloc(sizeof(DataPointer));
    (*Universal)->RegistryPointer=Registry;
    (*Universal)->skipListPointer=skipList;
    (*Universal)->bloomFilterPointer=bloomFilter;
    (*Universal)->countryDataPointer=countryData;
    (*Universal)->virusDataPointer=virusData;
    (*Universal)->vaccedPointer=vacced;
    (*Universal)->NbloomFiltersPointer=NbloomFilters; 
    (*Universal)->NskipListsPointer=NskipLists;
    (*Universal)->bloomSizePointer=bloomSize; 
    (*Universal)->NcountriesPointer=Ncountries;
    (*Universal)->inputSizePointer=inputSize;
    (*Universal)->filenamePointer=filename;

    //allocate and nullify the hash table array (pointers to linkedLists)
    (*Registry) = (Record**) malloc(10 * sizeof(Record*));
    for(int i=0;i<10;i++)
		  (*Registry)[i] = NULL;
    //allocated the vaccinated array which stores the values "YES" or "NO" to avoid information reoccurence
    //the citizen records' vaccinated field will point towards this array
    (*vacced) = (char**) malloc(sizeof(char*)*2);
    (*vacced)[0] = (char*) malloc(sizeof(char)*3);
    strcpy((*vacced)[0], "NO");
    (*vacced)[1] = (char*) malloc(sizeof(char)*4);
    strcpy((*vacced)[1], "YES");
}

//simple coinFlip function
int coinFlip()
{
    return (rand()%INT_MAX)%2;
}

//this function checks if current virus is stored in the array virusData and if current country is stored in the array countryData
//if they do not exist it creates them and informs the caller about their position in the arrays with the filter and index variables
//returns 1 if virus already exists and 0 if virus didn't exist in the virusData array so that new skipList and bloomFilter can be allocated for the current virus
//receives virus in word[0] and country in word[2] and returns pointers to the virusData array in word[1] and to the countryData array in word[3]
int virus_exists(DataPointer *Universal, char ***word, int *filter, int *index)
{
    //printf("virus=%s and country=%s\n", (*word)[0], (*word)[2]);         
    char ***virusData = Universal->virusDataPointer;
    char ***countryData = Universal->countryDataPointer;
    int Nviruses = *(Universal->NbloomFiltersPointer);
    int *Ncountries = Universal->NcountriesPointer;
    int n = Nviruses;
    int nc = *Ncountries;
    int countryFound = 0;
    int virusFound = 0;
    //allocate the char array that will store the names of the viruses
    if((*virusData)==NULL && Nviruses==0){
        (*virusData) = (char**) malloc(sizeof(char*)*1);
        (*virusData)[0] = (char*) malloc(sizeof(char)*(strlen((*word)[0])+1));
        strcpy((*virusData)[0], (*word)[0]);
        (*filter) = 0;
        (*word)[1] = (*virusData)[0];
        //printf("virus =%s and virusData =%s\n", (*word)[1], (*virusData)[0]);         
        //return 0;
    }                
    //allocate the char array that will store the names of the countries
    if((*word)[2]!=NULL)
    {
        if(countryData!=NULL && nc==0){
            (*countryData) = (char**) malloc(sizeof(char*)*1);    
            (*countryData)[0] = (char*) malloc(sizeof(char)*(strlen((*word)[2])+1));    
            strcpy((*countryData)[0], (*word)[2]);    
            (*index)=0;    
            (*word)[3] = (*countryData)[0];    
            //printf("country =%s and countryData =%s\n", (*word)[3], (*countryData)[0]); 
            (*Ncountries)++;        
        }
    }                
    //search if country exists in the array
    if((*word)[2]!=NULL)
    {
        for(int i=0; i<nc; i++)
        {
            if(!strcmp((*word)[2], (*countryData)[i])){
                (*index)=i;
                countryFound = 1;
                (*word)[3] = (*countryData)[i];
            }
        }
    }         
    //search if virus exists in the array
    for(int i=0; i<n; i++)
    {
        if(!strcmp((*word)[0], (*virusData)[i]))
        {
            (*filter)=i;
            virusFound = 1;
            (*word)[1] = (*virusData)[i];
            //return 1;
        }    
    }              
    if(n>0)
    {
        //if virus dont exist in the array put it
        if(!virusFound)
        {
            (*virusData) = (char**) realloc((*virusData), sizeof(char*)*(n+1));
            (*virusData)[n] = (char*) malloc(sizeof(char)*(strlen((*word)[0])+1));
            strcpy((*virusData)[n], *word[0]);
            (*filter)=n;
            (*word)[1] = (*virusData)[n];
            //printf("virus =%s and virusData =%s\n", (*word)[1], (*virusData)[n]);         

        } 
        //if country dont exist in the array put it
        if(!countryFound && (*word)[2]!=NULL)
        {
            (*countryData) = (char**) realloc((*countryData), sizeof(char*)*(nc+1));
            (*countryData)[nc] = (char*) malloc(sizeof(char)*(strlen((*word)[2])+1));
            strcpy((*countryData)[nc], (*word)[2]);
            (*index)=nc;
            (*word)[3] = (*countryData)[nc];
            //printf("country =%s and countryData =%s\n", (*word)[3], (*countryData)[nc]); 
            (*Ncountries)++;
        } 
    } 
    //printf("virusout =%s and countryout=%s\n", (*word)[1], (*word)[3]);         
    if(virusFound)
        return 1;
    return 0;
}

//creates new bloomFilter and skipLists
void create_datatypes(DataPointer *Universal)
{
    char ***bloomFilter = Universal->bloomFilterPointer;
    Sentinel ***skipList = Universal->skipListPointer;
    int *Nbloom = Universal->NbloomFiltersPointer;
    int *Nskip = Universal->NskipListsPointer;
    int bloomSize = *(Universal->bloomSizePointer);
    int inputSize = *(Universal->inputSizePointer);
    int nb = *Nbloom;
    //i initialize skipLists with 1 level and I then alter it according to the inputSize
    int skipLevels=1;
    if(inputSize>=2)
        skipLevels = abs(log2(inputSize));

    (*Nbloom)++;
    int ns = *Nskip;
    (*Nskip) = (*Nskip)+2;
    if(nb==0 && ns==0)
    {   
        (*bloomFilter) = (char**) malloc(sizeof(char*)*1);    
        (*skipList) = (Sentinel**) malloc(sizeof(Sentinel*)*2);
    }
    else
    {
        (*bloomFilter) = (char**) realloc((*bloomFilter), sizeof(char*)*(nb+1));
        (*skipList) = (Sentinel**) realloc((*skipList), sizeof(Sentinel*)*(ns+2));
    }
    (*bloomFilter)[nb] = (char*) malloc(sizeof(char)*bloomSize);
    (*skipList)[ns] = (Sentinel*) malloc(sizeof(Sentinel));
    (*skipList)[ns]->next = (Node**) malloc(sizeof(Node*)*skipLevels);
    (*skipList)[ns]->data = NULL;
    (*skipList)[ns]->NSkipLevels = skipLevels;
    (*skipList)[ns+1] = (Sentinel*) malloc(sizeof(Sentinel));
    (*skipList)[ns+1]->next = (Node**) malloc(sizeof(Node*)*skipLevels);
    (*skipList)[ns+1]->data = NULL;
    (*skipList)[ns+1]->NSkipLevels = skipLevels;
    for(int i=0; i<skipLevels; i++){
        (*skipList)[ns]->next[i] = NULL;
        (*skipList)[ns+1]->next[i] = NULL;
    }
    //printf("Just created %d bloomfilter and %d,%d linkedlists\n",nb,ns,ns+1);
}

//compares two dates 
//returns 1 if date1>date2
//returns -1 if date1<date2
//returns 0 if date1==date2
int compare_dates(int d1, int m1, int y1, int d2, int m2, int y2)
{
    if(y1>y2)
        return 1;
    else if(y1<y2)
        return -1;
    else if(y1==y2)
    {
        if(m1>m2)
            return 1;
        else if(m1<m2)
            return -1;
        else if(m1==m2)
        {
            if(d1>d2)
                return 1;
            else if(d1<d2)
                return -1;
            else if(d1==d2)
                return 0;
        }
    }
    return -2;
}

//returns 1 if date1 <= date <= date2
int in_dateSpan(int d1, int m1, int y1, char *date, int d2, int m2, int y2)
{
    int d, m ,y;
    if(date==NULL) 
        return -1;
    d = atoi(strtok (date,"-"));
    m = atoi(strtok (NULL, "-"));
    y = atoi(strtok (NULL, "-"));
    //printf("'%d' '%d' '%d' \n",d1,m1,y1);
    //printf("'%d' '%d' '%d' \n",d,m,y);
    //printf("'%d' '%d' '%d' \n",d2,m2,y2);
    if(compare_dates(d,m,y,d1,m1,y1)>=0 && compare_dates(d2,m2,y2,d,m,y)>=0)
        return 1;
    else 
        return -1;
}

//if number is NaN return 0.0
float value(float number)
{
    if(number!=number)
        return 0.0;
    else
        return number;
}


