#include	"Ibp.h"

/**************************************************************************************************
 *     全局参数定义
 **************************************************************************************************/
RSA    *g_VRsa = NULL, *g_PRsa = NULL;

/**************************************************************************************************
 *  *  Function:  DES_Encrypt		将英文字符串改成大写
 *  *  Description:					改变输入字符串的值
 **************************************************************************************************/
long	DES_Encrypt(const char *pszInp, const long lInpt, const char *pszKey,
			char **ppszOut, long *plOut, int enc)
{
	const_DES_cblock	yCBlock, yIvec;
	DES_key_schedule	ys;
	long				lLen = lInpt % 8 ? (lInpt / 8 + 1) * 8 : lInpt;
	uchar				*p = NULL;

	memset(yCBlock, 0, 8);

	if(strlen(pszKey) <= 8) memcpy(yCBlock, pszKey, strlen(pszKey));
	else					memcpy(yCBlock, pszKey, 8);

	DES_set_key_unchecked(&yCBlock, &ys);

	memset(yIvec, 0, 8);	//  一般为 初始 0

	*plOut = lLen;
	if(NULL == (p = (uchar *)malloc(*plOut)))
		return -1;

	memset(p, 0, *plOut);
	DES_ncbc_encrypt((uchar *)pszInp, (uchar *)p, lInpt, &ys, &yIvec, enc);
	*ppszOut = p;

	return 0;
}

/**************************************************************************************************
 *  *  Function:  base64_encode		转换成base64
 *  *  Description:
 **************************************************************************************************/
long   base64_encode(const char *s, long n, char *d, long *o)
{
    BUF_MEM *bptr = NULL;
    BIO     *bmem = NULL, *b64 = NULL;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    if (NULL == b64 || NULL == bmem || !o)
        return -1;

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    b64 = BIO_push(b64, bmem);
    BIO_write(b64, s, n);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    if((unsigned int)(bptr->length + 1) > *o)
    {
        BIO_free_all(b64);
        return -1;
    }
    memcpy(d, bptr->data, bptr->length);
    d[bptr->length] = 0;
	*o = bptr->length;

    BIO_free_all(b64);

    return 0;
}

/**************************************************************************************************
 *  *  Function:  base64_decode     base64反码
 *  *  Description:
 **************************************************************************************************/
long    base64_decode(char *s, int n, char *d, long o)
{
    BUF_MEM *bptr = NULL;
    BIO     *b64 = NULL, *bio = NULL;
    int     counts,  size = 0;

    if(s == NULL || d == NULL)
        return -1;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    bio = BIO_new_mem_buf(s, n);
    bio = BIO_push(b64, bio);
    size = BIO_read(bio, d, n);
    d[size] = '\0';

    BIO_free_all(bio);
    return size;
}

/**************************************************************************************************
    函 数 名：lInitRsa(const char *pszPrvfile, const char *pszPubfile)
    功能描述：初始化ras公私密钥
    返回说明：
        0                               --成功
        -1                              --失败
 **************************************************************************************************/
long    lInitRsa(const char *pszPrvfile, const char *pszPubfile)
{
    FILE    *fp = NULL;

    if(NULL == (fp = fopen(pszPrvfile, "rb" )))
    {
        fprintf(stderr, "打开私钥文件(%s)错误, %s", pszPrvfile, strerror(errno));
        return -1;
    }

    if(NULL == (g_VRsa = PEM_read_RSAPrivateKey(fp, NULL, 0, 0)))
    {
        fprintf(stderr, "从配置文件中解析私钥失败");
        goto ErrRSAInit;
    }

    fclose(fp);

    if(NULL == (fp = fopen(pszPubfile, "rb" )))
    {
        fprintf(stderr, "打开公钥文件(%s)错误, %s", pszPubfile, strerror(errno));
        return -1;
    }

//  PEM_read_RSAPublicKey 不能用这个函数，你得公钥使用命令行生成，
//  所以用这个函数不对，用PEM_read_RSA_PUBKEY这个
//  if(NULL == (g_PRsa = PEM_read_RSAPublicKey(file,NULL,NULL,NULL))
    if(NULL == (g_PRsa = PEM_read_RSA_PUBKEY(fp, NULL, 0, 0)))
    {
        fprintf(stderr, "从配置文件中解析公钥失败");
        goto ErrRSAInit;
    }

    fclose(fp);
    return 0;

ErrRSAInit:
//  RSA_free(g_VRsa);
    fclose(fp);
    ERR_print_errors_fp(stdout);
    return -1;
}

/**************************************************************************************************
    函 数 名：lPackRSAsign(uchar *text, long len, uchar *out, long size)
    功能描述：对报文text签名
    参数说明:
        text                           --待签名的字符串
        len                            --text报文长度
        out                            --签名base64后的签名串
        size                           --out最大长度
    返回说明：
        0                              --成功
        -1                             --失败
 **************************************************************************************************/
long    lPackRSAsign(uchar *text, long len, uchar *out, long *size)
{
    uchar  uHash[21], uSign[2048];
    uint   siglen = 0;

    memset(uHash, 0, sizeof(uHash));
    memset(uSign, 0, sizeof(uSign));
    if(!g_VRsa || !size)    return -1;

    SHA1(text, len, uHash);
    if(1 != RSA_sign(NID_sha1, uHash, 20, uSign, &siglen, g_VRsa))
        return -1;

    if(-1 == (siglen = base64_encode((char *)uSign, siglen, out, size)))
        return -1;

    return 0;
}

/**************************************************************************************************
    函 数 名：lPackRSAverify(uchar *text, long len, uchar *sig, long len)
    功能描述：对报文text签名
    参数说明:
        text                           --待验证的报文串
        len                            --text报文长度
        out                            --签名base64后的签名串
        size                           --out最大长度
    返回说明：
        0                              --成功
        -1                             --失败
 **************************************************************************************************/
long    lPackRSAverify(const char *text, long n, char *sig, long len)
{
    uint   siglen = 0;
    uchar  uHash[21], uSign[2048];

    memset(uHash, 0, sizeof(uHash));
    memset(uSign, 0, sizeof(uSign));
    if(!g_PRsa)    return -1;

    if(-1 == (siglen = base64_decode(sig, len, uSign, sizeof(uSign))))
        return -1;

    SHA1(text, n, uHash);
    if (1 != RSA_verify(NID_sha1, uHash, 20, uSign, siglen, g_PRsa))
        return -1;

    return 0;
}

/****************************************************************************************
	code end
****************************************************************************************/
