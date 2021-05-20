OBJS 		= vaccineMonitor.o	vaccineMonitorUtil.o myLib.o  hashTable.o  bloomFilter.o  skipList.o  hashFunctions.o
SOURCE		= vaccineMonitor.c	vaccineMonitorUtil.c myLib.c  hashTable.c  bloomFilter.c  skipList.c  hashFunctions.c
HEADER  	= ../lib/vaccineMonitorUtil.h ../lib/myLib.h  ../lib/hashTable.h  ../lib/hashStruct.h ../lib/bloomFilter.h  ../lib/skipList.h  ../lib/skipStruct.h ../lib/hashFunctions.h
OUT  		= vaccineMonitor
CC			= gcc
FLAGS  		= -O3 -c -std=c11
DEBUGFLAGS	= -g -O3 -c -Wextra -Wall

debug 				?= 0
virusesfile 		?= smallvirusesFile
countriesfile	 	?= smallcountriesFile
inputfile 			?= ../script/inputFile
numlines 			?= 200
duplicatesallowed 	?= 1
bloomsize 			?= 100000

all:	script	\
	$(OUT)

run:
	./$(OUT) -c $(inputfile) -b $(bloomsize)	

.PHONY:	valgrind
valgrind:
	valgrind --trace-children=yes --track-fds=yes --show-leak-kinds=all --leak-check=full --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(OUT) -c $(inputfile) -b $(bloomsize)

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@ -lm

vaccineMonitor.o: vaccineMonitor.c
ifeq	($(debug),2)
	$(CC) $(DEBUGFLAGS) vaccineMonitor.c 2>/dev/null
else
ifeq	($(debug),1)
	$(CC) $(DEBUGFLAGS) vaccineMonitor.c 
else
	$(CC) $(FLAGS) vaccineMonitor.c  
endif
endif

vaccineMonitorUtil.o: vaccineMonitorUtil.c 
ifeq	($(debug),2)
	$(CC) $(DEBUGFLAGS) vaccineMonitorUtil.c 2>/dev/null
else
ifeq	($(debug),1)
	$(CC) $(DEBUGFLAGS) vaccineMonitorUtil.c 
else
	$(CC) $(FLAGS) vaccineMonitorUtil.c  
endif
endif

myLib.o: myLib.c
ifeq	($(debug),2)
	$(CC) $(DEBUGFLAGS) myLib.c 2>/dev/null
else
ifeq	($(debug),1)
	$(CC) $(DEBUGFLAGS) myLib.c
else
	$(CC) $(FLAGS) myLib.c
endif
endif

hashTable.o: hashTable.c 
ifeq	($(debug),2)
	$(CC) $(DEBUGFLAGS) hashTable.c 2>/dev/null
else
ifeq	($(debug),1)
	$(CC) $(DEBUGFLAGS) hashTable.c
else
	$(CC) $(FLAGS) hashTable.c
endif
endif

bloomFilter.o: bloomFilter.c 
ifeq	($(debug),2)
	$(CC) $(DEBUGFLAGS) bloomFilter.c 2>/dev/null
else
ifeq	($(debug),1)
	$(CC) $(DEBUGFLAGS) bloomFilter.c
else
	$(CC) $(FLAGS) bloomFilter.c
endif
endif

skipList.o: skipList.c
ifeq	($(debug),2)
	$(CC) $(DEBUGFLAGS) skipList.c 2>/dev/null
else
ifeq	($(debug),1)
	$(CC) $(DEBUGFLAGS) skipList.c
else
	$(CC) $(FLAGS) skipList.c
endif
endif

hashFunctions.o: hashFunctions.c 
ifeq	($(debug),2)
	$(CC) $(DEBUGFLAGS) hashFunctions.c 2>/dev/null
else
ifeq	($(debug),1)
	$(CC) $(DEBUGFLAGS) hashFunctions.c
else
	$(CC) $(FLAGS) hashFunctions.c
endif
endif

.PHONY:	script
script:	../script/testFile.sh
	cd ../script; ./testFile.sh $(virusesfile) $(countriesfile) $(numlines) $(duplicatesallowed); cd ../src;

.PHONY:	clean
clean:
	rm -f $(OBJS) $(OUT) 

.PHONY:	cleanall
cleanall:
	rm -f $(OBJS) $(OUT) 
	rm -f ../script/inputFile 
	rm -f valgrind-out.txt 

