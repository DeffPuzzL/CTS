cd $HSMHOME/src/cli
make -f lib.a.mak 

cd $HSMHOME/src/cli
make -f lib.so.mak 

cd $HSMHOME/src/svr
make -f hsmsvr.mak

cd $ONLINE_HOME/src/Common 
make clean all

cd $ONLINE_HOME/src/Dbs 
make clean all

cd $ONLINE_HOME/src/CstDbs 
make clean all

cd $ONLINE_HOME/src/Convert 
make -f Convert.mak clean all
make -f IPPConvert.mak clean all

cd $ONLINE_HOME/src/Mem
make -f memCreate.mak clean all
make -f memdb.mak clean all
make -f test.mak all

cd $ONLINE_HOME/src/EPOSDbs 
make clean all

cd $ONLINE_HOME/src/Enc
make clean all

cd $ONLINE_HOME/src/Cust 
make clean all

cd $ONLINE_HOME/src/Convert 
make -f IPPConvert.mak clean
make -f IPPConvert.mak 

cd $ONLINE_HOME/src/Convert 
make -f IPPConvertServer.mak clean
make -f IPPConvertServer.mak 

cd $ONLINE_HOME/src/EPOSManage 
make clean all

cd $ONLINE_HOME/src/EPOSSwSrv
make clean all

#cd $ONLINE_HOME/src/Mem
#make -f memCreate.mak clean all
#if [ $? -ne 0 ];then  
  #exit 1
#fi

#make -f memdb.mak clean all
#if [ $? -ne 0 ];then  
#  exit 1
#fi

#make -f test.mak clean all
#if [ $? -ne 0 ];then
#  exit 1
#fi

cd $ONLINE_HOME/src/Bridge 
make clean all

cd $ONLINE_HOME/src/SwSrv
make clean all

cd $ONLINE_HOME/src/Manage 
make clean all

cd $ONLINE_HOME/src/POSManage 
make clean all

cd $ONLINE_HOME/src/ExpSrv
make clean all

cd $ONLINE_HOME/src/TrsSrv
make clean all

cd $ONLINE_HOME/src/FwdSrv 
make clean all

cd $ONLINE_HOME/src/ToCtrl 
make clean all

cd $ONLINE_HOME/src/Dumpmsg
make clean all

cd $ONLINE_HOME/src/Comm/CommCup
make -f C001.mak clean all

cd $ONLINE_HOME/src/Comm/CommEos
make clean all

cd $ONLINE_HOME/src/Comm/CommHost
make clean all

cd $ONLINE_HOME/src/Comm
make -f C001.mak clean all
make -f C002.mak clean all
make -f C3003.mak clean all
make -f C010.mak clean all

cd $ONLINE_HOME/src/Comm/CommCon
make clean all

cd $ONLINE_HOME/src/Comm/CommIPPCli
make clean all

cd $ONLINE_HOME/src/Comm/CommIPPServerSimu
make clean all

cd $ONLINE_HOME/src/Daemon 
make clean all

cd $ONLINE_HOME/src/MorPChange
make -f Change.mak clean all