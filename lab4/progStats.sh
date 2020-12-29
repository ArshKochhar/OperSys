#!/bin/sh
echo $#

mainFunc=false 
inMod=false

if [ $# -ne 1 ]; then
    echo "Usage: $0 ProgStats"
    
fi

if [ -d "$1" ]; then #if the arg is a directory
    echo "Main files: "
    for file in `find "$1" -type f -name '*.c'`; do #finding files that end with .c
        if grep -wq "int main" $file; then #finds all files that has int main in them
            mainFunc=true #set to true
            if [ "$mainFunc" == true ]; then 
                COUNTPRINTF=$(grep -wc "printf" $file -s) #searches for printf, -wc used to count its occurances
                COUNTFPRINTF=$(grep -wc "fprintf" $file -s) #searches for printf, -wc used to count its occurances
                echo = "${file}: ${COUNTPRINTF}, ${COUNTFPRINTF}" | awk '{printf "%s %d,%d\n", $1, $2, $3}' #print
            fi
        fi
    done
    
    if [ "$mainFunc" == false ]; then 
        echo "error: Main files not found"
    fi 
    
    echo "Module Files: "
    for file in `find "$1" -type f -name '*.c'`; do #finding files that end with .c
        if grep -wq "init_module" $file; then #running a search for init_module
            inMod=true #set to true
            if [ "$inMod" == true ]; then
                PRINTKLN=$(grep -wn "printk" $file -s | sed -e 's/:.*$//')
                echo "${file}: " | awk '{printf "%s", $1}'
                echo "${PRINTKLN}" | awk '{printf " %d", $1}'
                echo " "
            fi
        fi
    done
    
    if [ "$inMod" == false ]; then
        echo "error: Module files not found\n"
    fi 
    
else
    echo "The first arg is not a directory\n"
    exit $1
fi 
    
