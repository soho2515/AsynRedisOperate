#!/bin/bash

targEt=PlayerDataServer
code_path=/home/digsomething/code/Server/PlayerDataServer
code_public=/home/digsomething/code/Public
lib_path=/home/digsomething/code/DSSDK/lib/libDSSDK.a

echo 'The $target path is:'$code_path
echo 'The public path is:'$code_public
echo 'The common path is:'$code_common
if [ "$1" != "Y" ] && [ "$1" != "y" ];then
echo 'Do you want update code from svn?(Y/N)'
read update_svn
   if [ "$update_svn" = "Y" ] || [ "$update_svn" = "y" ];then
      echo 'Now update Public and comman and PlayerDataServer from svn:'
      eval svn update $code_public
      #eval svn update $code_common
      eval svn update $code_path
      echo 'Now the code have update to the newest.'
   fi
fi
if [ "$1" = "Y" ] || [ "$1" = "y" ];then
   sure_compile='y'
else
   echo 'Do you sure and want to compile it?(Y/N)'
   read sure_compile
fi
if [ "$sure_compile" = "Y" ] || [ "$sure_compile" = "y" ];then
   echo 'Now begin compile the project:'
   #eval cd $code_path
   eval chmod +x convert.py
   eval pwd
   eval make clean
   eval rm -f *.o
   autoscan
   aclocal
   autoconf
   eval ./convert.py
   sed 's:$(ALLLIBS):/home/digsomething/code/DSSDK/lib/libDSSDK.a @PTHREAD_LIBS@:g' Makefile.am > 111tmp.file
   mv -f 111tmp.file Makefile.am
   touch NEWS
   touch README
   touch AUTHORS
   touch ChangeLog
   automake --add-missing
   ./configure
#   sed 's/CFLAGS = -g -O2/CFLAGS = -g -O -DDEBUG/g' Makefile > 222tmp.file
#   sed 's/CXXFLAGS = -g -O2/CXXFLAGS = -g -O -DDEBUG/g' 222tmp.file> 333tmp.file
#   mv -f 333tmp.file Makefile
#   rm -f 111tmp.file, 222tmp.file, 333tmp.file
   make -j4
   if [ -f $target ];then
      echo 'The PlayerDataServer have compiled suscess, you can get it in /home/digsomething/code/bin/Server'
      eval mv -f PlayerDataServer /home/digsomething/code/bin/Server/playerdataserver
   else
      echo 'The PlayerDataServer compile error.'
   fi
else
   echo 'Not compile.'
fi
