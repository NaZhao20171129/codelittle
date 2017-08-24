#! usr/bin/bash

if  grep hello  testFile > /dev/null
then 
   echo "found"
else
   echo "not found"
fi
 
