#include <stdio.h>

#ifndef skipStructHeader
#define skipStructHeader
#include "skipStruct.h"
#endif

#ifndef hashTableStructHeader
#define hashTableStructHeader
#include "hashTableStruct.h"
#endif

typedef struct DataPointer{
    Record ***RegistryPointer;
    Sentinel ***skipListPointer;
    char ***bloomFilterPointer;
    char ***countryDataPointer;
    char ***virusDataPointer;
    char ***vaccedPointer;
    char **filenamePointer;
    int *NbloomFiltersPointer, *NskipListsPointer, *bloomSizePointer, *NcountriesPointer, *inputSizePointer;
}DataPointer;



