#include <stdio.h>
#include <math.h>

#ifndef skipStructHeader
#define skipStructHeader
#include "../structures/skipStruct.h"
#endif

#ifndef DataStructHeader
#define DataStructHeader
#include "../structures/DataStruct.h"
#endif

#define K 16
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
(byte & 0x80 ? '1' : '0'), \
(byte & 0x40 ? '1' : '0'), \
(byte & 0x20 ? '1' : '0'), \
(byte & 0x10 ? '1' : '0'), \
(byte & 0x08 ? '1' : '0'), \
(byte & 0x04 ? '1' : '0'), \
(byte & 0x02 ? '1' : '0'), \
(byte & 0x01 ? '1' : '0')

void uploadData(DataPointer** ,Record***, Sentinel***, char***, char***, char***, char***, char**, int*, int*, int*, int*, int*);
int virus_exists(DataPointer*, char***, int*, int*);
int coinFlip();
int in_dateSpan(int, int, int, char*, int, int, int);
int compare_dates(int, int, int, int, int, int);
float value(float);
void create_datatypes(DataPointer*);

