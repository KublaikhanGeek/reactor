#!/bin/sh

if [ $# -ne 2 ]
then
    echo "usage: $0 input output"
    exit 1
fi

input=$1
output=$2

sed 's/<\/a>/<\/a>\n/g' $input |\
    sed 's/.*\(<a href=.*>.*<\/a>\).*/\1/g' |\
    grep -E "<a.*<\/a>" | \
    sed s/"<a.*href="/''/g |\
    sed s/"<\/a>.*"/''/g |\
    sed s/">.*"/''/g |\
    grep -E "http://" |\
    awk -F'"' '{print $2}' |\
    sort -u > $output
