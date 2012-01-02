#!/bin/bash -e

EXTENSION=mp3

if [ -n $1 ]; then
    EXTENSION=$1
fi

total=0

IFS='
'
for file in $(find / -name \*.${EXTENSION} 2> /dev/null); do
    stat "$file" > /dev/null 2>&1 || continue
    total=$(( ${total} + $(du -b "$file" | awk '{ print $1 }')))
done

echo Total: $(($total/1024/1024/1024))GB