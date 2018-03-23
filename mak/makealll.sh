cd $ONLINE_HOME/src/Common 
make   all

cd $ONLINE_HOME/src/Convert 
make   all

cd $ONLINE_HOME/src/CstDbs 
make   all

cd $ONLINE_HOME/src/Dbs 
make   all

cd $ONLINE_HOME/src/Cust 
make   all

cd $ONLINE_HOME/src/Bridge 
make   all

cd $ONLINE_HOME/src/SwSrv
make   all

cd $ONLINE_HOME/src/Manage 
make   all

cd $ONLINE_HOME/src/POSManage 
make   all

cd $ONLINE_HOME/src/ExpSrv
make   all

cd $ONLINE_HOME/src/TrsSrv
make   all

cd $ONLINE_HOME/src/FwdSrv 
make   all

cd $ONLINE_HOME/src/ToCtrl 
make   all

cd $ONLINE_HOME/src/Dumpmsg
make   all

cd $ONLINE_HOME/src/Comm
make -f C001.mak   all
make -f C002.mak   all
make -f C3003.mak   all

cd $ONLINE_HOME/src/Daemon 
make   all
