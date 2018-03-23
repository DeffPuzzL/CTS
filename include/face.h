#ifndef      XXX_FACE_DEFINE_XXX
#define      XXX_FACE_DEFINE_XXX
#include    "cts.h"
#include    "version.h"

/*************************************************************************************************
    变量池操作
 *************************************************************************************************/
/*    0下标变量池操作 */
#define IBPputstring(t, v)                   lPutstring(pGetCmie(), PONLY_NRML, strlen(v), t, v, FIELD_CHAR)
#define IBPputsenstr(t,v,a)                  lPutstring(pGetCmie(), PONLY_NRML, strlen(v), t, v, a)
#define IBPputlong(t, v)                     lPutlong(pGetCmie(), PONLY_NRML, t, v)
#define IBPputdouble(t, v)                   lPutdouble(pGetCmie(), PONLY_NRML, t, v, 2)
#define IBPputdoublepcs(t, v, pcs)           lPutdouble(pGetCmie(), PONLY_NRML, t, v, pcs)

/*    自动追加添加变量池 */
#define IBPaddstring(t, v)                   lPutstring(pGetCmie(), PONLY_AUTO, strlen(v), t, v, FIELD_CHAR)
#define IBPaddlong(t, v)                     lPutlong(pGetCmie(), PONLY_AUTO, t, v)
#define IBPadddouble(t, v, pcs)              lPutdouble(pGetCmie(), PONLY_AUTO, t, v, pcs)    
#define IBPputstringofs(idx, t, l, v)        lPutstring(pGetCmie(), idx, l, t, v, FIELD_CHAR) 
#define IBPputstringidx(idx, t, v)           lPutstring(pGetCmie(), idx, strlen(v), t, v, FIELD_CHAR)

#define IBPputsenstridx(idx, t, v, a)        lPutstring(pGetCmie(), idx, strlen(v), t, v, a) 
#define IBPputbinaryidx(idx, t, v, l)        lPutstring(pGetCmie(), idx, l, t, v, FIELD_CHAR)
#define IBPputlongidx(idx, t, v)             lPutlong(pGetCmie(), idx, t, v)
#define IBPputdoubleidx(idx, t, v, pcs)      lPutdouble(pGetCmie(), idx, t, v, pcs)

#define IBPgetstring(t, v, d)                lGetvalues(pGetCmie(), PONLY_AUTO, FIELD_CHAR, t, v, sizeof(v) - 1, d)
#define IBPgetlong(t, v, d)                  lGetvalues(pGetCmie(), PONLY_AUTO, FIELD_LONG, t, v, sizeof(long), d)
#define IBPgetdouble(t, v, d)                lGetvalues(pGetCmie(), PONLY_AUTO, FIELD_DOUBLE, t, v, sizeof(double), d)
#define IBPgetstringidx(idx,t,v,d)           lGetvalues(pGetCmie(), idx, FIELD_CHAR, t, v, sizeof(v) - 1, d)
#define IBPgetlongidx(idx,t, v, d)           lGetvalues(pGetCmie(), idx, FIELD_LONG, t, v, sizeof(long), d)
#define IBPgetdoubleidx(idx, t, v, d)        lGetvalues(pGetCmie(), idx, FIELD_DOUBLE, t, v, sizeof(double), d)
#define IBPgetbinaryidx(idx,t,v,l,d)         lGetBinvalue(pGetCmie(), idx, t, v, l, d)
#define IBPgetpackbinidx(idx,t,v,l,d)        lGetPackBinval(pGetCmie(), idx, t, v, l, d)
#define IBPgetdefstring(t, v, u)             lGetDefstring(pGetCmie(), PONLY_AUTO, t, v, sizeof(v) - 1, u)
#define IBPgetdeflong(t, v, u)               lGetDeflong(pGetCmie(), PONLY_AUTO, t, v, sizeof(long), u)
#define IBPgetdefdouble(t, v, u)             lGetDefdouble(pGetCmie(), PONLY_AUTO, t, v, sizeof(double), u)

#define IBPdroplable(t)                      lDeletePool(pGetCmie(), PONLY_AUTO, t)
#define IBPdroplableidx(idx, t)              lDeletePool(pGetCmie(), idx, t)

#define IBPresetuser()						 vResetUser(pGetCmie())
#define IBPresetpool()                       vResetPool(pGetCmie())
#define IBPprintpool()                       vPrintPool(pGetCmie())

#define IBPsetsyslevel(lv)                   vSetSysLevel(pGetCmie(), lv)
#define IBPlogsetname(...)                   vLogSetName(pGetCmie(), __VA_ARGS__)
#define IBPlogstore(...)                     vLogStore(pGetCmie())
#define IBPlogrestore(...)                   vLogRestore(pGetCmie())

/*************************************************************************************************
	线程安全
 *************************************************************************************************/
/*    0下标变量池操作 */
#define IBTputstring(h, t, v)                lPutstring(h, PONLY_NRML, strlen(v), t, v, FIELD_CHAR)
#define IBTputsenstr(h, t,v,a)               lPutstring(h, PONLY_NRML, strlen(v), t, v, a) 
#define IBTputlong(h, t, v)                  lPutlong(h, PONLY_NRML, t, v)
#define IBTputdouble(h, t, v)                lPutdouble(h, PONLY_NRML, t, v, 2)
#define IBTputdoublepcs(h, t, v, pcs)        lPutdouble(h, PONLY_NRML, t, v, pcs)

/*    自动追加添加变量池 */
#define IBTaddstring(h, t, v)                lPutstring(h, PONLY_AUTO, strlen(v), t, v, FIELD_CHAR)
#define IBTaddlong(h, t, v)                  lPutlong(h, PONLY_AUTO, t, v)
#define IBTadddouble(h, t, v, pcs)           lPutdouble(h, PONLY_AUTO, t, v, pcs)    
#define IBTputstringofs(h, idx, t, l, v)     lPutstring(h, idx, l, t, v, FIELD_CHAR) 
#define IBTputstringidx(h, idx, t, v)        lPutstring(h, idx, strlen(v), t, v, FIELD_CHAR)

#define IBTputsenstridx(h, idx, t, v, a)     lPutstring(h, idx, strlen(v), t, v, a) 
#define IBTputbinaryidx(h, idx, t, v, l)     lPutstring(h, idx, l, t, v, FIELD_CHAR)
#define IBTputlongidx(h, idx, t, v)          lPutlong(h, idx, t, v)
#define IBTputdoubleidx(h, idx, t, v, pcs)   lPutdouble(h, idx, t, v, pcs)

#define IBTgetstring(h, t, v, d)             lGetvalues(h, PONLY_AUTO, FIELD_CHAR, t, v, sizeof(v) - 1, d)
#define IBTgetlong(h, t, v, d)               lGetvalues(h, PONLY_AUTO, FIELD_LONG, t, v, sizeof(long), d)
#define IBTgetdouble(h, t, v, d)             lGetvalues(h, PONLY_AUTO, FIELD_DOUBLE, t, v, sizeof(double), d)
#define IBTgetstringidx(h, idx,t,v,d)        lGetvalues(h, idx, FIELD_CHAR, t, v, sizeof(v) - 1, d)
#define IBTgetlongidx(h, idx,t, v, d)        lGetvalues(h, idx, FIELD_LONG, t, v, sizeof(long), d)
#define IBTgetdoubleidx(h, idx, t, v, d)     lGetvalues(h, idx, FIELD_DOUBLE, t, v, sizeof(double), d)
#define IBTgetbinaryidx(h, idx,t,v,l,d)      lGetBinvalue(h, idx, t, v, l, d)
#define IBTgetpackbinidx(h, idx,t,v,l,d)     lGetPackBinval(h, idx, t, v, l, d)
#define IBTgetdefstring(h, t, v, u)          lGetDefstring(h, PONLY_AUTO, t, v, sizeof(v) - 1, u)
#define IBTgetdeflong(h, t, v, u)            lGetDeflong(h, PONLY_AUTO, t, v, sizeof(long), u)
#define IBTgetdefdouble(h, t, v, u)          lGetDefdouble(h, PONLY_AUTO, t, v, sizeof(double), u)

#define IBTdroplable(h, t)                   lDeletePool(h, PONLY_AUTO, t)
#define IBTdroplableidx(h, idx, t)           lDeletePool(h, t)

#define IBTresetuser(h)						 vResetUser(h)
#define IBTresetpool(h)                      vResetPool(h)
#define IBTprintpool(h)                      vPrintPool(h)

/*************************************************************************************************
    日志相关操作
 *************************************************************************************************/
#define IBPdetail(...)                       vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_DETAIL, __VA_ARGS__)
#define IBPdebug(...)                        vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_DEBUG, __VA_ARGS__)
#define IBPinfo(...)                         vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_INFO, __VA_ARGS__)
#define IBPrace(...)                         vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_TRACE, __VA_ARGS__)
#define IBPerror(...)                        vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_ERROR, __VA_ARGS__)
#define IBPwarn(...)                         vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_WARM,  __VA_ARGS__)
#define IBPuser(...)                         vPrintString(pGetCmie(), __FILE__, __LINE__, -1,lGetSeqNo(), LOG_MOST,  __VA_ARGS__)
#define IBPfocus(...)                        vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_MOST,  __VA_ARGS__)
#define IBPstring(s,l)                       vPrintHexes(pGetCmie(), LOG_MOST, s, l)
#define IBPcem(s,l)                          vWriteString(pGetCmie(), s, l)    //    通讯交换报文
#define IBPbuss(x)                           vLogWriteList(x)

#define IBPcout(F, ...)                      vPrint(F, __FILE__, __LINE__, getpid(), 0, __VA_ARGS__)
#define IBPcts(...)                          vPrint(DEFAULT_LOG, __FILE__, __LINE__, getpid(), 0, __VA_ARGS__)
#define IBPcrit(...)                         vPrint(CRITICAL_LOG, __FILE__, __LINE__, getpid(), 0, __VA_ARGS__)

//    系统日志相关操作
#define IBTpool(h, ...)                      vPrintString(h, __FILE__, __LINE__, -1,lGetSeqNo(), LOG_TRACE,  __VA_ARGS__)
#define IBTsysm(h, ...)                      vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_MOST,  __VA_ARGS__)
#define IBTmsg(h, ...)                       vPrintString(h, __FILE__, __LINE__, -1, lGetSeqNo(), LOG_INFO,  __VA_ARGS__)
#define IBTbin(h, s,l)                       vPrintHexes(h, LOG_INFO, s, l)

#define IBTdetail(h, ...)                    vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_DETAIL, __VA_ARGS__)
#define IBTdebug(h, ...)                     vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_DEBUG, __VA_ARGS__)
#define IBTinfo(h, ...)                      vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_INFO, __VA_ARGS__)
#define IBTrace(h, ...)                      vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_TRACE, __VA_ARGS__)
#define IBTerror(h, ...)                     vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_ERROR, __VA_ARGS__)
#define IBTwarn(h, ...)                      vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_WARM,  __VA_ARGS__)
#define IBTuser(h, ...)                      vPrintString(h, __FILE__, __LINE__, -1,lGetSeqNo(), LOG_MOST,  __VA_ARGS__)
#define IBTfocus(...)                        vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_MOST,  __VA_ARGS__)
#define IBTstring(h, s, l)                   vPrintHexes(h, LOG_MOST, s, l)
#define IBTbin(h, s, l)                      vPrintHexes(h, LOG_INFO, s, l)
#define IBTcem(h, s, l)                      vWriteString(h, s, l)    //    通讯交换报文

/*************************************************************************************************
    系统运行错误操作表    
 *************************************************************************************************/
#define SYSError(x, ...)                    if(!strcmp(x, RET_DATABASE))    \
                                            {    \
                                                vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_DATABASE, __VA_ARGS__);\
                                                vSetError(pGetGloble(), x, __VA_ARGS__);    \
                                            }    \
                                            else    \
                                            {    \
                                                vPrintString(pGetCmie(), __FILE__, __LINE__, 0, lGetSeqNo(), LOG_SYSTEM, __VA_ARGS__);\
                                                vSetError(pGetGloble(), x, __VA_ARGS__);    \
                                            }

#define SYSTrror(h, x, ...)                 if(!strcmp(x, RET_DATABASE))    \
                                            {    \
                                                vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_DATABASE, __VA_ARGS__);\
                                                vSetError(&h->m_stCts, x, __VA_ARGS__);    \
                                            }    \
                                            else    \
                                            {    \
                                                vPrintString(h, __FILE__, __LINE__, 0, lGetSeqNo(), LOG_SYSTEM, __VA_ARGS__);\
                                                vSetError(&h->m_stCts, x, __VA_ARGS__);    \
                                            }

/*************************************************************************************************
    函数申明    
 *************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
extern  HHCMIE       pMallocCmie();
extern  HHCMIE       pGetCmie();
extern  void*        pGetDoc();
extern  CtsPool**    pGetPool();
extern  CtsEvent*    pGetEvent();
extern  GCts*        pGetGloble();

extern  CtsEvent*    pCmieEvent(HHCMIE cm);
extern  GCts*        pCmieGloble(HHCMIE cm);
extern  CtsPool**    pCmiePool(HHCMIE cm);

/*************************************************************************************************
 *************************************************************************************************/
/*************************************************************************************************
    系统参数函数
 *************************************************************************************************/
extern  long        lGetSysParam(SATvm *pstSavm, CtsParam *pstParam);
extern  long        lSetSysState(SATvm *pstSavm, long eType);
extern  char*       sGetSysActDate();
extern  long        lGetSystem(char *pszTarget);
extern  long        lSetSysCrDate(SATvm *pstSavm, char *pszCrDate);
extern  long        lSetSysTxDate(SATvm *pstSavm, char *pszTxDate);
extern  long        lSetSysSeqNo(SATvm *pstSavm, llSEQ lSeqNo);
extern  long        lDayendParam(SATvm *pstSavm, CtsParam *pstParam);

/*************************************************************************************************
    异步标识
 *************************************************************************************************/
extern  long        lRegisterAsyn(SATvm *pstSavm, CtsAsyn *pstAsyn);
extern  long        lGetAsynIdx(SATvm *pstSavm, CtsAsyn *pstIdx, char *pszIdx);
extern  long        lDeleteAsyn(SATvm *pstSavm, CtsAsyn *pstIdx);
extern  long        lRecycleAsyn(SATvm *pstSavm, char *pszClearDay);
extern  long        lResetAsyn(SATvm *pstSavm, long lSeqNo, char *pszClear);
extern  long        lSaveAsynIdx(SATvm *pstSavm, char *pszIdx, GCts *pstCts, CtsEvent *pstEvent);
extern  long        lCleanupAsyn();
extern  long        lRecoverByAsyn(SATvm *pstSavm, HHCMIE cm, char *pszIdx);

extern  long        lGetCmdReturn(char *pszCmd);
extern  long        lGetCmdString(char *pszCmd, char *pszOut, long lOut);
//extern    long    lCheckSystem()
//    进程相关函数
extern  BOOL        bIsExtraBoot(char *pszPGName);
extern  void        vExitAbnormal(int sig);
extern  long        lRaiseProcess(CtsProcess *pstProcess, long lExt);
extern  long        lForkProcess(char *pszPGName, CtsProcess *pstProcess, BOOL eType);
extern  BOOL        bExistProcess(long lPid);
extern  BOOL        bCheckProcess(long lPid);
extern  long        lShrinkProcess(SATvm *pstSavm, CtsProcess *pstProcess, CtsGroup *pstGroup);
extern  long        lStallProcess(SATvm *pstSavm, CtsProcess *pstProcess, CtsGroup *pstGroup);
extern  long        lRegisterProcess(char *pszProcess, long lHost);
extern  long        lUnregisterProcess(CtsProcess *pstProcess);
extern  long        lExecProcess(SATvm *pstSavm, CtsGroup *pstGroup, BOOL bType);
extern  long        lStopProcess(CtsGroup *pstGroup);
extern  long        lRefreshGroup(SATvm *pstSavm, char *pszPGName, CtsGroup *pstGroup);
extern  long        lUpdateGroup(SATvm *pstSavm, CtsGroup *pstGroup);
extern  long        lGetIdleDone(SATvm *pstSavm, char *pszProcess, long lPos, long lNum);
extern  long        lGetIdleProcess(SATvm *pstSavm, char *pszProcess, long lPos, long lNum, long *plPid);
extern  long        lInitRunTest(char *pszPGName);
extern  long        lInitCoreDemo(SATvm *pstSavm, long lPid, CtsProcess *pstProcess);
extern  long        lInitialDemo(SATvm *pstSavm, long lPid, CtsProcess *pstProcess);
extern  long        lInsertProcess(SATvm *pstSavm, CtsProcess *pstProc);
extern  long        lUpdateProcess(SATvm *pstSavm, CtsProcess *pstProcess, long lState);
extern  long        lExitProcess(CtsProcess *pstProcess, BOOL bf);
extern  long        lRefreshProcess(CtsProcess *pstProcess, long lPid);
extern  long        lDumpProcess(long lPid);
extern  long        lDeleteProcess(long lPid);

//    平台事件函数
extern  long        lInPutRequest(SATvm *pstSavm, void *psvData, long lLen, long lRevfd, char *, char *);
extern  long        lAsPutRequest(SATvm *pstSavm, void *psvData, long lLen, long lRevfd, char *, char *);
extern  long        lInGetRespond(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, char *pszOut, long *plLen);
extern  long        lOutGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm);
extern  long        lOutGetRequestEx(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv);
extern  long        lAsyGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm);
extern  long        lAsyGetRequestEx(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv);
extern  long        lOutPutResponse(SATvm *pstSavm, HHCMIE cm);
extern  long        lCorePutResponse(SATvm *pstSavm, HHCMIE cm);
extern  long        lCoreGetRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm);
extern  long        lGetPackRequest(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm);
extern  long        lGetPackRequestEx(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, EXITEXEC pvFunc, void *argv);
extern  long        lPutPackResponse(SATvm *pstSavm, HHCMIE cm);
extern  long        lCallPackExch(SATvm *pstSavm, HHCMIE cm, BOOL bBuild);
extern  long        lAsynDispatch(SATvm *pstSavm, HHCMIE cm, CtsFlow *pstNext);
extern  long        lDispatchMethod(SATvm *pstSavm, HHCMIE cm);
extern  long        lResetEvent(SATvm *pstSavm, CtsProcess *pstProcess, HHCMIE cm, long lPid);

extern  long        lTraceMethod(SATvm *pstSavm, HHCMIE cm, char *pszPGName, char *pszClass, char *pszMethod);
extern  long        lAyCallMethod(SATvm *pstSavm, HHCMIE cm, char *pszPGName, char *pszClass, char *pszMethod);
extern  long        lMonitorEvent(SATvm *pstSavm, CtsProcess *pstProcess);
extern  long        lMonitorEventEx(SATvm *pstSavm, CtsProcess *pstProcess, EXITEXEC pvFunc, void *argv);
extern  long        lResetQueue(SATvm *pstSavm, long lQid);
extern  long        lResetSAinit();
extern  long        lNotifyPack(char *pszPGName, long lHost);

//    配置查询函数
extern  long        lGetNextFlow(SATvm *pstSavm, HHCMIE cm, CtsFlow *pstFlow, long *plRet);
extern  long        lGetTrCode(SATvm *pstSavm, CtsTrCode *pstCode, char *pszInCode, char *pszInSub);
extern  long        lGetClass(SATvm *pstSavm, CtsClass *pstClass, char *pszClass, char *pszMethod);
extern  long        lGetProcess(SATvm *pstSavm, CtsProcess *pstProcess, long lPid);
extern  long        lGetGroup(SATvm *pstSavm, CtsGroup *pstGroup, char *pszProcess, long lHost);
extern  long        lGetHost(SATvm *pstSavm, CtsHost *pstHost, long lHost);
extern  long        lGetHostByApp(SATvm *pstSavm, CtsHost *pstHost, char *pszPGName);
extern  long        lGetService(SATvm *pstSavm, CtsService *pstService, char *pszTrCode);
extern  long        lGetTradeFlow(SATvm *pstSavm, CtsFlow *pstFlow, long lSvrId, long lFlowSeq);

extern  long        lGetHostService(SATvm *pstSavm, long lHost, CtsService **ppstService, long *plOut);
extern  long        lGetHostClass(SATvm *pstSavm, long lHost, CtsClass **ppstClass, size_t *plOut);
extern  long        lDeleteFlow(SATvm *pstSavm, long lSvrID);
extern  long        lRemoveService(SATvm *pstSavm, CtsService *pstService);

//    交易相关函数
extern  long        lGenSeqNo(SATvm *pstSavm);
extern  long        lDisableService(SATvm *pstSavm, char *pszTrCode);
extern  long        lEnableService(SATvm *pstSavm, char *pszTrCode);
extern  BOOL        bIsEventValid(SATvm *pstSavm, HHCMIE cm);
extern  BOOL        bIsMonDeposit(SATvm *pstSavm, HHCMIE cm);
extern  long        lSetDrivePack(SATvm *pstSavm, HHCMIE cm, char *pszData, long lLen);
extern  long        lGetDrivePack(SATvm *pstSavm, HHCMIE cm, char *pszData, long *plLen);
extern  long        lSetGlobleCts(SATvm *pstSavm, HHCMIE cm, BOOL bType);
extern  long        lGetGlobleCts(SATvm *pstSavm, HHCMIE cm, BOOL bType);

extern  long        lInitTrans(SATvm *pstSavm, GCts *pstCts, size_t *plOffset, void *psvData, long lLen);
extern  long        lInitCall(SATvm *pstSavm, HHCMIE cm, GCts *pstNts, long *plOffset);
extern  long        lFreeTrans(SATvm *pstSavm, llSEQ lSeqNo, size_t lOffset);
extern  long        lResetTrade(SATvm *pstSavm, HHCMIE cm);
extern  long        lResetTrans(SATvm *pstSavm, HHCMIE cm);
extern  long        lCall(SATvm *pstSavm, char *pszTrCode);          //  非平台进程使用
extern  long        lStartTrade(SATvm *pstSavm, HHCMIE cm, char *pszTrCode);	//	非业务模块调用
extern  long        lCallTrade(SATvm *pstSavm, HHCMIE cm, char *pszTrCode);     //  业务模块异步调用
extern  long        lNewTrade(SATvm *pstSavm, HHCMIE cm, char *pszTrCode, BOOL bPool); // 业务模块同步调用

extern  BOOL        bIsFinish(SATvm *pstSavm, llSEQ lSeqNo, size_t lOffset);
extern  long        lCountTrade();
extern  BOOL        bIsExistTrade();
extern  long        lCallSmsModule(const char* fmt, ...);
extern  void        vHttpPostSms(char *pszUrl, char *pszUtf8);
extern  long        lPackReadFile(char *pszFile);
extern  long        lPackRead(SATvm *pstSavm, long lMid, void *psvData, long *plLen);
extern  long        lPackWrite(SATvm *pstSavm, long lMid, void *psvOut, long *plLen);
extern  long        lPackWriteAt(SATvm *pstSavm, HHCMIE cm, long lMid, void *psvOut, long *plLen);
extern  long        lPackReadAt(SATvm *pstSavm, HHCMIE cm, long lMid, void *psvData, long *plLen);

extern  long        lSaveProcess(CMList **ppstRoot, long lPid);
extern  long        lWaitProcess(CMList *pstRoot);
/*************************************************************************************************
    变量池
 *************************************************************************************************/
extern  void        vCombinePool(HHCMIE cm, CtsPool **node);
extern  long        lDeletePool(HHCMIE cm, long lIdx, const char *t);
extern  long        lPutdouble(HHCMIE cm, long lIdx, const char *t, double dValue, long lPercs);
extern  long        lPutlong(HHCMIE cm, long lIdx, const char *t, long lValue);
extern  long        lPutstring(HHCMIE cm, long lIdx, long lLen, const char *t, char *pv, long atr);
extern  long        lGetvalues(HHCMIE cm, long lIdx, long atr, const char *t, void *pv, long lOut, const char *ps);
extern  long        lGetBinvalue(HHCMIE cm, long lIdx, const char *t, void *pv, long *pl, const char *ps);
extern  long        lGetPackBinval(HHCMIE cm, long lIdx, const char *t, void *pv, long *pl, const char *ps);
extern  long        lGetDefstring(HHCMIE cm, long lIdx, const char *t, void *pv, long lOut, char *pszDef);
extern  long        lGetDefdouble(HHCMIE cm, long lIdx, const char *t, void *pv, long lOut, double dDef);
extern  long        lGetDeflong(HHCMIE cm, long lIdx, const char *t, void *pv, long lOut, long lDef);
extern  long        lGetCountPool(HHCMIE cm, const char *t);
extern  long        lParsePool(HHCMIE cm, void *pvData, long lSize, long lCount);
extern  long        lBuildPool(HHCMIE cm, void *pvData, long lSize, long *plCount);
extern  void        vResetZero(HHCMIE cm);
extern  void        vResetUser(HHCMIE cm);
extern  void        vResetPool(HHCMIE cm);
extern  void        vPrintPool(HHCMIE cm);
extern  void        vSetTradePool(HHCMIE cm);
/*************************************************************************************************
    交易结构
 *************************************************************************************************/
extern  void        vInitialGloble();
extern  void        vCtsStore();
extern  void        vCtsRestore();
extern  void        vSetError(GCts *pstCts, const char *pszRet, const char *f, ...);
extern  void        vSetTrError(const char *pszRet, const char *f, ...);
extern  long        lGetSeqNo();
extern  void        vSetSeqNo(long lSeqNo);
extern  void        vSetInHost(long lHost);
extern  long        lGetInHost();
extern  void        vSetLocalHost(long lHost);
extern  long        lGetLocalHost();
extern  char*       sGetTrCode();
extern  void        vSetTrCode(const char* pszCode);
extern  char*       sGetInCode(void);
extern  void        vSetInCode(const char* pszInCode);
extern  char*       sGetInSubCode(void);
extern  void        vSetInSubCode(const char* pszSubCode);
extern  long        lGetStatus();
extern  long        lGmtStatus(HHCMIE me);
extern  void        vSetStatus(long lState);
extern  char*       sGetRetCode(void);
extern  void        vSetRetCode(const char* pszRetCode);
extern  char*       sGetRetMsg(void);
extern  void        vSetRetMsg(const char* szFmt, ...);
extern  long        lGetLastStatus(void);
extern  void        vSetLastStatus(const long lStatus);
extern  char*       sGetLastRet(void);
extern  void        vSetLastRet(const char* pszRetCode);
extern  void        vSetLastMsg(const char* pszFmt, ...);
extern  char*       sGetLastMsg(void);
extern  char*       sGetLogName();
extern  char*       sGetActDate();
extern  char*       sGetTxDate();
extern  void        vSetLogName(const char* pszFmt, ...);
extern  long        lGetUseTime(void);
extern  long        lGetTimeout(void);
extern  void        vSetRetSucc(GCts *pstCts);
extern  void        vSetSuccess(GCts *pstCts);

/*************************************************************************************************
    二级缓存应用
 *************************************************************************************************/
extern  long        lInitAllSatvm(SATvm *pstSavm);
extern  long        lGetLogSize();
extern  void        vSetProgram(char *s);
extern  char*       sGetProgram();
extern  void        vSetLogSize(long lSize);
extern  long        lGetInQueue();
extern  void        vSetInQueue(long lQid);
extern  void        vSetMaxQueue(long lMax);
extern  long        lGetMaxQueue();
extern  void        vSetMaxPid(long lMax);
extern  long        lGetMaxPid();
extern  void        vSetToPos(long lPos);
extern  long        lGetToPos();
extern  void        vSetToNum(long lNum);
extern  long        lGetToNum();
extern  long        lGetToQueue();
extern  void        vSetToQueue(long lQid);
extern  void        vSetTrQueue(long lQid);
extern  long        lGetTrQueue();
extern  void        vSetUserSize(long lSize);
extern  long        lGetUserSize();

extern  void        vSetSocket(BSock sk);
extern  BSock       skGetSocket();
extern  long        lGetLogLevel();
extern  void        vSetLogLevel(long lLevl);
extern  void        vInitFileFd(HHCMIE cm);
extern  void        vCheckFileFd(HHCMIE cm);
extern  char*       pGetBuffer();
extern  void*       pGetVTData();
extern  void*       pvSetVTData(void *pv);

/*************************************************************************************************
    日志函数
 *************************************************************************************************/
extern  void        vLogStore(HHCMIE cm);
extern  void        vLogRestore(HHCMIE cm);
extern  char*       sLogGetPath();
extern  void        vLogSetPath();
extern  void        vLogSetName(HHCMIE cm, const char* fmt, ...);
extern  char*       sLogGetName(HHCMIE cm);
extern  void        vLogSetSize(long lSize);
extern  long        lLogGetSize();
extern  long        lGetSysMaster();
extern  void        vSetSysMaster(long lNode);
extern  long        lGetSysLevel(HHCMIE cm);
extern  void        vSetSysLevel(HHCMIE cm, long lLev);
extern  void        vLogInitial(HHCMIE cm, GCts *pstCts);
extern  void        vLogWriteList(GCts *pstCts);
extern  void        vWriteString(HHCMIE cm, char *s, long l);
extern  void        vPrintHexes(HHCMIE cm, long lv, char *s, int k);
extern  void        vPrintString(HHCMIE cm, const char *, long , long , long , long , const char *fmt, ...);
extern  void        vPrint(char *, char *, long , long , long , char *fmt, ...);

/*************************************************************************************************
    内存三级缓存
 *************************************************************************************************/
extern  CtsPack*    pGetCache(long lMid);


//    底层操作表类
/*************************************************************************************************
    数据库内存表操作，提供以下API操作内存表, 游标、更新、查询操作
 *************************************************************************************************/
extern    char*       sGetUpdTime();
extern    long        lCreateQueue(SATvm *pstSavm, BOOL bCreate);
extern    void        vDeleteQueue(SATvm *pstSavm);

/*************************************************************************************************
    应用函数类    提供以下API，查询内存表
 *************************************************************************************************/
extern    long        lConvertCharset(const char *fCharset, const char *dCharset, char *pszOrg, long lOLen, 
                        char *pszOut, size_t *plOut);
extern    int         nCallSort(const void *pvFrom, const void *pvNext);

#ifdef __cplusplus
}
#endif

#endif        //    XXX_FACE_DEFINE_XXX
