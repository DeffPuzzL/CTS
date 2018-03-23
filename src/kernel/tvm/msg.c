#include    "tvm.h"

/****************************************************************************************
	消息队列处理
 ****************************************************************************************/
/****************************************************************************************
	创建消息队列
 ****************************************************************************************/
long    lCreateQueue(SATvm *pstSavm, BOOL bCreate)
{
    long    lQid;

    if(bCreate)
    {   
        pstSavm->m_yMey = yGetIPCPath(pstSavm, IPC_MSG);
        if(pstSavm->m_yMey <= RC_FAIL)
            return RC_FAIL;
        lQid = msgget(pstSavm->m_yMey, IPC_CREAT|0600);
    }   
    else
        lQid = msgget(IPC_PRIVATE, IPC_CREAT|0600);
    if(RC_FAIL >= lQid)
    {
        switch(errno)
        {
        case   EEXIST:
            pstSavm->m_lErrno = MSG_ERR_EXIST;
            break;
        case   EACCES:
            pstSavm->m_lErrno = MSG_ERR_ACCES;
            break;
        case   ENOMEM:
            pstSavm->m_lErrno = MSG_ERR_NOMEM;
            break;
        default:
            pstSavm->m_lErrno = MSG_ERR_INVAL;
            break;
        }
        return RC_FAIL;
    }

    return lQid;
}

/******************************************************************************
	函 数 名：lGetQueueNum                  
	功能描述：取指定队列上的消息数量
 ******************************************************************************/
long    lGetQueueNum(SATvm *pstSavm, long lQid)
{
    struct  msqid_ds    stQueue;

    if(msgctl(lQid, IPC_STAT, &stQueue) <= -1)
    {
        switch(errno)
        {
        case   EFAULT:
            pstSavm->m_lErrno = MSG_ERR_FAULT;
            break;
        case   EIDRM:
            pstSavm->m_lErrno = MSG_ERR_EIDRM;
            break;
        default:
            pstSavm->m_lErrno = MSG_ERR_INVAL;
            break;
        }

        return RC_FAIL;
    }

    return stQueue.msg_qnum;
}

/******************************************************************************
	函 数 名：lQueueMaxByte
	功能描述：获取队列最大支持byte
 ******************************************************************************/
long    lQueueMaxByte(SATvm *pstSavm, long lQid)
{
    struct  msqid_ds    stQueue;

    if(msgctl(lQid, IPC_STAT, &stQueue) <= -1)
    {
        switch(errno)
        {
        case   EFAULT:
            pstSavm->m_lErrno = MSG_ERR_FAULT;
            break;
        case   EIDRM:
            pstSavm->m_lErrno = MSG_ERR_EIDRM;
            break;
        default:
            pstSavm->m_lErrno = MSG_ERR_INVAL;
            break;
        }

        return RC_FAIL;
    }

    return stQueue.msg_qbytes;
}

/******************************************************************************
	函 数 名：lQueueRcvTime
	功能描述：获取队列中最后处理事件时间
 ******************************************************************************/
long    lQueueRcvTime(SATvm *pstSavm, long lQid)
{
    struct  msqid_ds    stQueue;

    if(msgctl(lQid, IPC_STAT, &stQueue) <= -1)
    {
        switch(errno)
        {
        case   EFAULT:
            pstSavm->m_lErrno = MSG_ERR_FAULT;
            break;
        case   EIDRM:
            pstSavm->m_lErrno = MSG_ERR_EIDRM;
            break;
        default:
            pstSavm->m_lErrno = MSG_ERR_INVAL;
            break;
        }

        return RC_FAIL;
    }

//  return (long)time(&stQueue.msg_rtime);
	return (long)stQueue.msg_rtime > 0 ? (long)stQueue.msg_rtime : (long)stQueue.msg_ctime;
}

/****************************************************************************************
	阻塞读取消息
 ****************************************************************************************/
long    lEventRead(SATvm *pstSavm, long lQid, void *pstVoid, long lSize, long lMType)
{
    if(RC_SUCC > msgrcv(lQid, pstVoid, lSize - sizeof(long), lMType, 0))
    {
        switch(errno)
        {
        case   E2BIG:
            pstSavm->m_lErrno = MSG_ERR_E2BIG;
            break;
        case   EACCES:
            pstSavm->m_lErrno = MSG_ERR_ACCES;
            break;
        case   EFAULT:
			pstSavm->m_lErrno = MSG_ERR_FAULT;
            break;
        case   EIDRM:
			pstSavm->m_lErrno = MSG_ERR_EIDRM;
            break;
        case   EINTR:
			pstSavm->m_lErrno = MSG_ERR_EINTR;
            break;
        default:
            pstSavm->m_lErrno = MSG_ERR_INVAL;
            break;
        }

        return RC_FAIL;
    }

    return RC_SUCC;
}

/****************************************************************************************
 *     非阻塞读取消息
 ****************************************************************************************/
long    lReadNoWait(SATvm *pstSavm, long lQid, void *psvVoid, long lSize, long lMType)
{
	errno = 0;
	if(RC_SUCC > msgrcv(lQid, psvVoid, lSize - sizeof(long), lMType, IPC_NOWAIT))
	{
		switch(errno)
		{
		case   EAGAIN:
		case   ENOMSG:
			return RC_SUCC;
		case   E2BIG:
		    pstSavm->m_lErrno = MSG_ERR_E2BIG;
		    break;
		case   EACCES:
		    pstSavm->m_lErrno = MSG_ERR_ACCES;
		    break;
		case   EFAULT:
			pstSavm->m_lErrno = MSG_ERR_FAULT;
		    break;
		case   EIDRM:
			pstSavm->m_lErrno = MSG_ERR_EIDRM;
		    break;
		case   EINTR:
			pstSavm->m_lErrno = MSG_ERR_EINTR;
			return RC_FAIL;
		default:
		    pstSavm->m_lErrno = MSG_ERR_INVAL;
		    break;
		}

		return RC_FAIL;
	}

    return RC_SUCC;
}

/****************************************************************************************
 *     发送消息
 ****************************************************************************************/
long    lEventWrite(SATvm *pstSavm, long lQid, void *psvData, long lSize)
{
    if(msgsnd(lQid, psvData, lSize - sizeof(long), 0) < RC_SUCC)
    {
        switch(errno)
        {
        case   EACCES:
            pstSavm->m_lErrno = MSG_ERR_ACCES;
            break;
		case   EAGAIN:
            pstSavm->m_lErrno = MSG_ERR_SNDEG;
			break;
        case   EFAULT:
			pstSavm->m_lErrno = MSG_ERR_FAULT;
            break;
        case   EIDRM:
			pstSavm->m_lErrno = MSG_ERR_EIDRM;
            break;
        case   EINTR:
			pstSavm->m_lErrno = MSG_ERR_EINTR;
            break;
        default:
            pstSavm->m_lErrno = MSG_ERR_INVAL;
            break;
        }

        return RC_FAIL;
    }

    return RC_SUCC;
}

/****************************************************************************************
	code end
 ****************************************************************************************/

