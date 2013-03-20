AsynRedisOperate
================

利用Hiredis实现的Redis数据库访问封装。

目前redis数据库使用越来越多，Hiredis是一般很好用的C语言访问库，但在实际项目应用中往往还需要再次进行业务封装才能保证
数据访问的效率控制和业务需求。本代码以PlayerDataServer为使用实例对Hiredis进行了一层业务封装。主要模块如下：

1. DSSDK是一版跨平台库的封装，全部来自自己以往的项目积累，封装了跨平台的线程、时间、文件、锁等操作，还提供了一个日志库。
2. PlayerDataServer中主线程负责业务逻辑，通过DBThreadPool开了DB访问的线程池，每个线程有一个Redis连接。使用时要注意数据库
访问的互斥问题，如果数据库访问压力不是特别大，可以只开一个线程。
3. DBCommand是DB指令的封装，任何访问数据库的操作都继承自DBCommand，DBCommand_Player提供了访问Player数据的例子。访问流程是
这样的：在主线程中创造DBCommand，并进行初始化，然后填入DB线程的操作队列。对于需要回调的操作，需要创建DBCommandCallBack，并
与DBCommand绑定，见4.
4. DBCommandCallback是DBCommand执行完回调的封装，任何回调操作都继承自此类。在DBCommand产生后如果绑定了Callback，命令执行完
会自动调用Callback。
5. 为了降低new和delete的频率，所有DBCommand和DBCommandCallback的工厂都是初始化时产生好的（中间使用过程中不够时增加不再建设），并
复用的。所以所有DBCommand和DBCommandCallback需要在程序启动时注册。参考DBCommandFactoryRegister。
6. 代码是windows和linux平台跨平台的。提供的convert.py程序会自动将vcproj文件转化问linux需要的makefile.am文件，进而可以通过
automake和configuretion产生makefile文件。此过程已经封装在compile***.sh文件中，在编译时可能需要根据自己的路径结构进行适当修改。
