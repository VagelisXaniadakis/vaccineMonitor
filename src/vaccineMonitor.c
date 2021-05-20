#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef myLibHeader
#define myLibHeader
#include "../lib/headers/myLib.h"
#endif

#ifndef vaccineMonitorUtilHeader
#define vaccineMonitorUtilHeader
#include "../lib/headers/vaccineMonitorUtil.h"
#endif

int main(int argc, char *argv[])
{
    //dataTypes
    DataPointer *Universal = NULL;              //a struct that keeps pointers to the important data of the program  
    Record **Registry = NULL;                   //hashTable dataType
    Sentinel **skipList = NULL;                 //skipList dataType
    char **bloomFilter = NULL;                  //bloomFilter dataType 

    //pluralities
    int bloomSize = 0;                          //size of bloomFilter in bytes (defined by the user)
    int inputSize = 0;                          //plurality of citizens kept in the dataBase (no duplicates)
    int NskipLists = 0;                         //plurality of skipLists
    int NbloomFilters = 0;                      //plurality of bloomFilters == plurality of viruses stored in array virusData
    int Ncountries = 0;                         //plurality of countries stored in array countryData

    //arrays that store data to avoid information reoccurrence
    char **virusData = NULL;                    //virusData array keeps track of the viruses that exist
    char **countryData = NULL;                  //countryData array keeps track of the countries that exist
    char **vacced = NULL;                       //vacced array has two items "YES" and "NO"
    
    //save pointers to all the important data of the program in a struct for easy access
    char *filename = NULL;                      //inputFile name
    uploadData(&Universal, &Registry, &skipList, &bloomFilter, &countryData, &virusData, 
               &vacced, &filename, &NbloomFilters, &NskipLists, &bloomSize, &Ncountries, &inputSize);

    //check the parameters for correctness                             
    parametersCheck(Universal, argc, argv);
   
    //read inputFile and save the citizens' data in a hashtable, 
    //also create the required datatypes 
    //(i.e. create two arrays to store the viruses' and the countries' names, create the bloomFilters and the skipLists)
    parseInputFile(Universal);     

    //insert the citizens' data in the bloomFilters and the skipLists
    storeDataIntoDatatypes(Universal);

    //receive commands from the user and execute on them
    readInputFromKeyboard(Universal);
    
    //exit normally
    return 0;
}
