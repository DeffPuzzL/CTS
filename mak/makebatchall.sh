cd $ONLINE_HOME/src/batch/Dbs 
make clean all

cd $ONLINE_HOME/src/batch/Dbs/dbauto 
make clean all

cd $ONLINE_HOME/src/batch/Dbs/dbusr
make clean all

cd $ONLINE_HOME/src/batch/Common 
make clean all

cd $ONLINE_HOME/src/batch/BatMain 
make clean all

cd $ONLINE_HOME/src/batch/GenBrhInfileData 
make clean all

cd $ONLINE_HOME/src/batch/LoadTxn 
make -f LoadHostTxnfile.mak clean all
make -f LoadtblNTxnfile.mak clean all
make -f createTblNTxn.mak clean all
make -f loadcuptxn.mak clean all

cd $ONLINE_HOME/src/batch/RunStep 
make clean all

cd $ONLINE_HOME/src/batch/CreatMem 
make clean all

cd $ONLINE_HOME/src/batch/OldPosTxnProc 
make clean all

cd $ONLINE_HOME/src/batch/TxnLogProc 
make clean all

cd $ONLINE_HOME/src/batch/Stlm/stlm 
make -f stlm.mak clean all

cd $ONLINE_HOME/src/batch/PosReport 
make clean all

cd $ONLINE_HOME/src/batch/Report 
make clean all

cd $ONLINE_HOME/src/batch/AftInFile 
make -f makefile.mak clean all

cd $ONLINE_HOME/src/batch/CalSum 
make -f CalSum.mak clean all
