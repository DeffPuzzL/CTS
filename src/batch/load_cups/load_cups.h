/**************************************************************************************************
    文 件 名：settle_algo.h
    代码作者：DeffPuzzL、liujie
    编写版本：
    创建日期：2016-07-14
    功能描述：demo头文件
 **************************************************************************************************/
#ifndef     __HHH__CLASS_ALGO_HHHH_XXXX
#define     __HHH__CLASS_ALGO_HHHH_XXXX

#include    "face.h"
#include    "cups.h"
#include    "algo.h"
#include    "dbsapi.h"
#include    "tbl_cups_succ.h"

/*************************************************************************************************
    实现秒到清分。查询业务
 *************************************************************************************************/
class   CCups
{
public:
    CCups();
    ~CCups();

public:
    inline  char   *sGetVersion() { return g_szVersion; }
    long    lRunObject(char *, char *);

    CMList*     pGetCfgList();
    char*       sGetLoadCups();
    void        vSetLoadCups(char *p);
    char*       sGetLoadDate();
    char*       sGetLoadContent();
    void        vSetLoadDate(char *p);
    char*       sGetLoadFileName();
    void        vSetLoadFileName(char *p);
    long        lGetLoadLine();
    long        lGetLoadValid();
    void        vSetLoadLine(long lLine);
    BOOL        bIsIgnore();
    void        vSetIgnore(BOOL bf);
    BOOL        bIsRevocate();
    void        vSetRevocate(BOOL bf);
    BOOL        bIsDigit(char *s);
    char*       sGetValueType(long lType);
    char*       sTransCharset(char *s);
    char*       sGetTruckValue(char *s, char *o, char *d);
    long        lSetValueType(char *pszValue, long *plType);
    void        vDebugConfig();
    void        vPrintReList(Revocate *pstRevc);
    long        lInitailConfig();
    long        lInitRevocate(CupsCfg *pstCfg);
    CupsCfg*    pGetCupsCfg(char *pszCupsNo);
    void        vPrintValues(FiledDef  *pstFiled);
    void        vDebugCupTxn(TCupsSucc *pstCups);
    void        vDestroyConfig(CMList **ppstRoot);
    void        vDeleteFiled(FiledDef *pstFiled);
    long        lGetFiledNameIdx(FiledDef *pstFiled, char *pszName);
    FiledDef*   pInsertFiled(FiledDef *pstRoot, FiledDef *pstData, long lSize);
    ReList*     pInsertReList(ReList *pstRoot, ReList *pstData, long lSize);
    void        vDeleteReList(Benum eCfg, Revocate *pstRevoct);
    char*       sGetNameValue(char *pszCupsNo, char *pszFiled);
    char*       sGetCombValues(FiledDef *pstFiled, ComFiled *pstComb);
    char**      ppGetFmtParam(char *s, long lSize, long *plOut);
    char*       sGetFiledValue(FiledDef *pstFiled, long lIdx, long *plType);
    void        vPrintFiledDef(FiledDef *pstFiled);
    long        lInitSysVarList(CupsCfg *pstCfg, long *plId);
    long        lInsertCupTxn(TCupsSucc *pstCups);
    long        lParseFiledValue(FiledDef *pstRoot);
    long        lRouteCheck(Route *pstRoute, char *pszLine);
    BOOL        bIsLoadLine(char *pszLoadFunc, char *pszLine);
    long        lReserveTxn(CupsCfg *pstCfg, char *pszLine);
    long        lFiledDefine(CupsCfg *pstCfg, char *pszLine, long lIdx);
    long        lCallUserCheck(char *pszFunc, void *pstVoid, void *pstCom);
    long        lParseExpress(FiledDef *pstDef, FiledDef *pstRoot);
    long        lTransferValue(char *pszCut, char *pszDest, long lSize, CupsCfg *pstCfg);
    long        lInitFiledValue(CupsCfg *pstCfg, TCupsSucc *pstCups, char *pszLine);
    long        lAnalyseExpress(char *pszExpress, FiledDef *pstFiled, CupsCfg *pstCfg);
    long        lCheckExpress(char *pszFiled, CUPExpress *pstExp, FiledDef *pstFiled);
    long        lCheckCombina(char *pszComb, ComFiled *pstComb, FiledDef *pstFiled);
    long        lCallUserFunc(char *pszFunc, char *pszCupsNo, char *pszDate, char *pszValue, long *plLen);
    long        lExecuteFunc(char *pszFunc, char *pszCupsNo, char *pszDate, char *pszValue, long *plLen);

public:
    long        lMapCupsValue(TCupsSucc *pstCups, char *pszName, char *pszValue);
    long        lAbolishCupsTxn(Benum eMode, ReList *pstReList);
    long        lAlpyAbolishCupsTxn(ReList *pstReList);
    long        lCpnr_AbolishTrade(void *pstVoid, void *pstCom);

public:
    void        vLoadChnCom();

private:
    typedef struct
    {
        char    m_szName[64];
        void (CCups::*m_pEvent)();
    } stEvent;
    static stEvent  m_stEvent[];
};

#endif  //  __HHH__CLASS_ALGO_HHHH_XXXX
/**************************************************************************************************
    code end
 **************************************************************************************************/

