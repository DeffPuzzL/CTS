#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2004 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : database.ora9.mak                                          |
#|----------------------------------------------------------------------------|

#DBINCP = $(ORACLE_HOME)/rdbms/demo -I$(ORACLE_HOME)/rdbms/public
DBINCP = $(ORACLE_HOME)/precomp/public
DBLIBP = -L$(ORACLE_HOME)/lib
#DBLIBS = -lclntsh -lclient9 `cat $(ORACLE_HOME)/lib/sysliblist`
#DBLIBS = -lclntsh -lclient10 `cat $(ORACLE_HOME)/lib/sysliblist`
DBLIBS = `cat $(ORACLE_HOME)/lib/sysliblist`
DBD    = _DB_ORA
DBDEF  = -DORA10 -D$(DBD)
DBFIX  = .pc
DBFIXCPP  = .pcpp
DBTYPE = ora
