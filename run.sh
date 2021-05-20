#!/bin/bash

if [[ $# == 0 ]]; then
    echo "Using my bash script to create an inputFile."
    cd src; make -s cleanall all run; 
else
    if [ "$1" = "clean" ]
        then echo "Deleting the temporary files.";
        cd src; make cleanall;
        exit 0 
    fi
    if [ -f "script/$1" ]
        then echo "'$1' exists in the /script subdirectory.";
    else echo -e "'$1' doesn't exist in the /script subdirectory. \nPlease move the inputFile of your preference in the /script subdirectory and try again!";
        exit 1
    fi
   echo "Will use '$1' as an inputFile for the vaccineMonitor app."
   cd src; make -s cleanall all run inputfile="../script/$1"; 
fi

exit 0 