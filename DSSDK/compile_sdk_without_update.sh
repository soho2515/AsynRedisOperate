#!/bin/bash
# This is a sh which compile the DSSDK lib

target=libDSSDK.a
sure_compile=n

   echo 'Now begin compile the project:'
 #  eval cp ../linux_compile/convert.py ./convert.py
   eval chmod +x ./convert.py
   eval pwd
   eval make dist clean
   eval rm -f *.o
   autoscan
   aclocal
   autoconf
   ./convert.py
   touch NEWS
   touch README
   touch AUTHORS
   touch ChangeLog
   automake --add-missing
   ./configure
   make -j4
   if [ -f $target ];then
      mv -f libDSSDK.a ./lib/
      echo 'The DSSDK have compiled.'
   else
      echo 'The DSSDK compile error.'
   fi

