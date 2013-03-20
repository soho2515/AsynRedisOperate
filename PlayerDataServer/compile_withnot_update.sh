#!/bin/bash

targEt=PlayerDataServer
code_path=/home/digsomething/code/Server/PlayerDataServer
code_public=/home/digsomething/code/Public
lib_path=/home/digsomething/code/DSSDK/lib/libDSSDK.a

   echo 'Now begin compile the project:'
   #eval cd $code_path
   eval chmod +x convert.py
   eval pwd
   eval make distclean
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
      eval mv -f /home/digsomething/code/Server/PlayerDataServer/PlayerDataServer /home/digsomething/code/bin/Server/playerdataserver
   else
      echo 'The PlayerDataServer compile error.'
   fi
