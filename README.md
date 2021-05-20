# vaccineMonitor
In this project I was asked to implement a program that will accept, process, record and answer questions about vaccinations of citizens. More specifically, I implemented a set of structures (bloom filters, linked lists, skip lists) that allow the import and query of large volumes of records.

The application can be run as follows (inside the src subdirectory):
  
  ./vaccineMonitor -c citizenRecordsFile –b bloomSize
or:
  
  make -s cleanall all run
  
The application first opens the citizenRecordsFile file, reads it line by line, verifies its validity and then initializes and stores in memory the data structures to use against executing queries. When the application finishes proccesing the citizenRecordsFile file, it will wait for input from
the user from the keyboard. The user will be able to give the following commands (arguments in [] are optional):

● /vaccineStatusBloom citizenID virusName
The application checks the bloom filter associated with virusName and prints a message whether
a citizen with a citizenID ID number has been vaccinated against the virusName.
  
● /vaccineStatus citizenID virusName
H εφαρμογή θα ελέγχει τη skip list που σχετίζεται με virusName και θα τυπώνει μήνυμα για το αν ο πολίτης
με αριθμό ταυτότητας citizenID έχει κάνει το εμβόλιο κατά του virusName.

● /vaccineStatus citizenID
The application checks the skip list associated with virusName and prints a message about whether the citizen
with citizenID ID number has been vaccinated against virusName.

● /populationStatus [country] virusName date1 date2
If no country argument is given, the application prints the number of citizens for the virusName disease
in each country vaccinated within [date1 ... date2] and the percentage of the population of the vaccinated country. 
If a country argument is given, the application prints for the disease virusName, the number of people who have 
been vaccinated and the percentage of the country's population that has vaccinated within [date1 ... date2]. 
If there is a definition for date1 there should also be a definition for date2, 
otherwise an error message is printed to the user.

● /popStatusByAge [country] virusName date1 date2
If no country argument is given, the application will print the virusName number for the disease
vaccinations by age group in each country and the percentage of the population in the age group
who has been vaccinated in space [date1 ... date2]. 
If a country argument is given, the application prints for virusName disease, the number of vaccinations 
by age group and percentage of the population of the age group vaccinated within [date1 ... date2] in
country country. 
If there is a definition for date1 there should be a definition for date2, otherwise,
an error message is printed to the user.

● /insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]
The application will insert in the bloom filter and in the appropriate skip list related to the virusName virus
a new record with its details. 

● /vaccinateNow citizenID firstName lastName country age virusName
The application checks if the citizen with citizenID ID number has already been vaccinated against the virus
virusName and if so, prints an error message.
Otherwise, the application inserts the record in the bloom filter and the appropriate skip list associated with
the virusName virus.
  
● /list-nonVaccinated-Persons virusName
The application will access the appropriate skip list related to the virusName virus and will print
all citizens who have not been vaccinated against virusName. 

● /exit
The application will free all allocated memory and exit.
  
There also exists a bash script that creates a citizenRecordsFile file that is used as input for the main application.
The script can be run as follows:
  ./testFile.sh virusesFile countriesFile numLines duplicatesAllowed 
