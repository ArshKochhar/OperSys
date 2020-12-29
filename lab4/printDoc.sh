#!/bin/sh

for file in `find . -name *.c -o -name *.h`
do
    echo "============================================================================================="
    sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 's/\/\/[+-]*//' #removing comments and blocks of comments
done
