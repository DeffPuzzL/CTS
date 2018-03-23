#include    "tvm.h"

/****************************************************************************************
	信号量操作union结构
 ****************************************************************************************/
union semun {
	int					val;  
	struct	semid_ds	*buf;  
	struct	seminfo		*__buf;
};

/****************************************************************************************
	插入链表数据
 ****************************************************************************************/
long	lCreateSems(SATvm *pstSavm, RunTime *pstRun, long lSems, long lValue)
{
	long    i = 0;
	union	semun	uSem;
	
	if(lSems <= 0)
	{
		pstSavm->m_lErrno = SEM_CDIT_NULL;
		return RC_FAIL;	
	}

	if(RC_FAIL == (pstRun->m_semID = semget(pstSavm->m_ySey, lSems, IPC_CREAT|0600)))
	{
        switch(errno)
        {
        case   EEXIST:
            pstSavm->m_lErrno = SEM_ERR_EXIST;
            break;
        case   EIDRM:
            pstSavm->m_lErrno = SEM_ERR_EIDRM;
            break;
        case   EACCES:
            pstSavm->m_lErrno = SEM_ERR_ACCES;
            break;
        case   ENOMEM:
            pstSavm->m_lErrno = SEM_ERR_NOMEM;
            break;
        default:
            pstSavm->m_lErrno = SEM_ERR_INVAL;
            break;
        }

		return RC_FAIL;
	}

	for(i = 0, uSem.val = lValue; i < lSems; i ++)
        semctl(pstRun->m_semID, i, SETVAL, uSem);

  	return RC_SUCC;
}

/****************************************************************************************
	信号量的操作
 ****************************************************************************************/
long	lOperateSems(SATvm *pstSavm, long semID, long lSems, Benum evp)
{
	struct	sembuf	se; 

	//	第一个信号量
	se.sem_num = lSems;
	se.sem_op  = evp;
	se.sem_flg = SEM_UNDO;

	if(RC_SUCC != semop(semID, &se, 1))
	{
        switch(errno)
        {
		case   EINTR:
			return RC_SUCC;
        case   EEXIST:
            pstSavm->m_lErrno = SEM_ERR_EXIST;
            break;
        case   EIDRM:
            pstSavm->m_lErrno = SEM_ERR_EIDRM;
            break;
        case   EACCES:
            pstSavm->m_lErrno = SEM_ERR_ACCES;
            break;
        case   ENOMEM:
            pstSavm->m_lErrno = SEM_ERR_NOMEM;
            break;
		case   E2BIG:
		case   ERANGE:
            pstSavm->m_lErrno = SEM_ERR_INVAL;
            break;
        default:
            pstSavm->m_lErrno = SEM_ERR_INVAL;
            break;
        }

		return RC_FAIL;
	}

	return RC_SUCC;
}


/****************************************************************************************
	code end
 ****************************************************************************************/

