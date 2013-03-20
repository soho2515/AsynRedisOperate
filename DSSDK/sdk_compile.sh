#!/bin/bash
# This is a sh which compile the DSSDK lib

target=libDSSDK.a
sure_compile=n

echo 'The S8SDK path is:'$code_path
#echo 'The public path is:'$code_public
if [ "$1" = "Y" ] || [ "$1" = "y" ];then
   sure_compile='y'
else
   echo 'Do you sure and want to compile it?(Y/N)'
   read sure_compile
fi
if [ "$sure_compile" = "Y" ] || [ "$sure_compile" = "y" ];then
   echo 'Now begin compile the project:'
 #  eval cp ../linux_compile/convert.py ./convert.py
   eval chmod +x ./convert.py
   eval pwd
   eval make clean
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
else
   echo 'Not compile.'
fi

