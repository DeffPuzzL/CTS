                 ==============
                   README.TXT
                 ==============

1. TOPMake概述
    1.1 什么是TOPMake：TOPMake是一套makefile的框架，帮助大型工程的编译。
    1.2 TOPMake主要特点
        1.2.1 规范化makefile的书写，统一风格
        1.2.2 支持多种操作系统平台（目前支持IBM AIX 32bit,64bit/HP-UX 32bit,64bit/HP-UX ia64 32bit,64bit/Sun Solaris/SCO OpenServer/SCO UnixWare/Nonstop-UX/Digital TRU64/Redhat Linux/Turbo Linux）
        1.2.3 支持多种数据库编译(ORACLE/Informix/DB2/Sybase)
        1.2.4 支持中间件(BEA Tuxedo)
        1.2.5 支持 C/C++编译
        1.2.6 所有操作系统、数据库和其他全局性改变只需要做全局设置，不需要逐个修改makefile
        1.2.7 对环境变量的依赖小

2. 工程的目录结构
   ---- mak                makfile框架目录
     |
     |- include            主include目录
     |      |-- include1   模块1 include目录
     |      |-- include2   模块2 include目录
     |- src                主源代码目录
     |   |-- module1       模块1代码目录
     |   |-- module2       模块2代码目录
     |- lib                应用目标库目录
     |- bin                应用目标执行代码目录
         |--tux            应用目标Tuxedo Server目录

3. 环境变量
    3.1 $MKHOME 工程根路径的绝对目录
    3.2 产品的根目录 $ORACLE_HOME(ORACLE)、$INFORMIXDIR(Informix)、$DB2_HOME(DB2)、$TUXDIR(Tuxedo)

4. 各模块的makefile书写
    每一个模块目录可以有一个或多个makefile，每个makefile生成一个目标
    
    4.1 生成库的makefile(参考样板sample.dynamiclib.mak和sample.staticlib.mak)
        "PRGOBJS" 模块所有程序的列表。所有文件用全路径表示（使用MKHOME的相对路径）；文件名使用目标文件名(*.o)，make会自动寻找同名的*.c,*.cpp程序或数据库的嵌入式*.pc *.ec *.sqc文件进行编译
        "PRGTARG" 目标库的名。makefile会自动加上lib前缀，.a或.so后缀，生成在 lib/目录下
        "PRGDEFS" 定义模块需要定义的宏，对模块中的所有程序起作用如(PRGDEFS = -DMYMACRO -DMYSWITCH=1)
        "PRGDBNM" 如模块中有DB2 *.sqc源代码需要定义，说明DB2的dbname，用于连接数据库
    4.2 生成执行程序的makefile(参考样板sample.exec.mak)
        "PRGOBJS" 同上
        "PRGTARG" 执行代码目标文件名，生成在 bin/目录下
        "PRGLIBS" 程序使用的库表，用-lmodule1 -lmodule2 ...表示。为了保证改变操作系统数据库时不修改makefile，非应用的库不要使用操作系统的库如-lsocket等，使用系统已定义的宏：
                    $(UXLIBS)标准Unix库，数学库等底层库
                    $(NETLIBS) socket通讯库
                    $(THREADLIBS) 线程库
                    $(DLLIBS) 动态库使用库(-ldl)
                    $(CURLIBS) curses库
                    $(XLIBS) X lib
                    $(DBLIBS) 数据库使用的库
                    $(TUXLIBS) Tuxedo client库(Tuxedo Server不需要)
        "PRGDEFS" 同上
        "PRGDBNM" 同上
    4.3 生成Tuxedo Server的makefile(参考样板sample.tuxexec.mak)
        "PRGOBJS" 同上
        "PRGTARG" Tuxedo Server目标文件名，生成在 bin/tux/目录下
        "PRGLIBS" 同上
        "PRGDEFS" 同上
        "PRGDBNM" 同上
        "PRGSVCS" Tuxedo Server提供的Services名(参考Tuxedo buildserver -s开关的使用方法)
        "PRGRSMN" Tuxedo资源名(resource manager name)

5. 入口全局makefile的书写(参考样板sample.main.mak)
    这个makefile位于$(MKHOME)/mak目录，用于全局的操作系统平台、中间件、数据库的设置；整个工程的全编译
    makefile列举所有模块的makefile，按照依赖顺序，被依赖的模块在前(先编译)，依赖模块在后(后编译)

6. 平台设置
    在第一次使用编译前，首先需要设置目前的操作系统、数据库、中间件。
    在$(MKHOME)/mak目录下，假设入口全局的makefile文件名是project.mak，键入：
    make -f project.mak <ostype> <dbtype> <tuxtype>
    其中ostype 可以是：aix,aix64,hpux,hpux64,nsux,osf,rhlux,scoos,scouw,sunos,tblux
        dbtype 可以是：db2,infx7,infx9,ora7,ora8.0,ora8.1,ora9,mysql3,mysql4
        tuxtype可以是：notux,tux6,tux8

7. make 命令使用
    如果要编译某个模块可以到模块的对应目录下，假设makefile的文件名是module1.mak：
    make -f module.mak [debug|release] 编译模块module，并且只编译修改过的程序。debug 表示生成调试版本，release生成优化编译版本，不可调试(缺省是 dubug版本编译)
    make -f module.mak clean 清除模块生成的所有的目标*.o文件，库文件和执行程序
    make -f module.mak clean [debug|release] 模块全重编译
    如果要编译整个系统在 $(MKHOME)/mak目录下
    make -f project.mak [clean] [debug|release]

8. 源代码的注意事项：
    8.1 头文件包含
        应用头文件只包含了一个目录即$(MKHOME)/include，若include还有子目录如$(MKHOME)/include/module1源代码应带有相对路径：#include "module/file.h"
    8.2 操作系统相关的宏
        若代码中使用了一些不同操作系统有差异的代码，makefile提供了一些宏，代码中可以使用这些宏进行条件编译：
        操作系统平台宏：AIX_UNIX，HP_UNIX，SUNOS_UNIX，NONSTOP_UNIX，OSF_UNIX，SCO_UNIX，SCOUW_UNIX，TURBO_LINUX，REDHAT_LINUX
        64位编译宏：BIT64
        数据库宏：DB2，INFX7，ORA7，ORA8

9. 关于 IBM AIX动态库
    由于IBM AIX的特殊要求，IBM AIX生成动态库需要有一个输出符号的定义文件(exp文件)，若动态库名为module，生成的动态库为libmodule.so，必须在编译前准备文件$(MKHOME)/mak/exp/libmodule.exp。此文件中列举了库中所有外部可以使用的函数名(一个函数名占恍�)

10. 关于mkstand.mak
    mkstand.mak是主要的make逻辑，对此文件的修改将影响到所有的编译的过程，工程中绻卸员嘁氲奶厥庖罂梢孕薷恼飧鑫募�
