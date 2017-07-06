#!/bin/bash                                      
# Autor: Stanila Constantin Adrian               
# Date: 06/04/2009                               
# Description: Convert from a ISC dhcp lease file to a standard host configuration
# http://www.randombugs.com  
 
START=false

test -d cut || mkdir cut && 
for i in *.txt;
   #do cut -c30-50 $i > "cut/$i"
   
   do cat $i | while read line
   do
       #Bash pattern
       if [[ "$line" == MEASUREMENT* ]]
       then
           echo $line | awk '{print $4}' >> "cut/$i"
           #echo $line | cut -c30-50 >> "cut/$i"
       fi
   done;
done;
