#include "opsslpub.h"
#include "face.h"

#define iMAXBUFLEN 8192

/*
 *函数名称：OPSSL_get_conf
 *函数功能: 取配置文件参数(openssl.conf)
 *函数参数: 传入：参数1：分区名，参数2：键值对-属性
 *	    传出：参数3：键值对-值
 *函数返回: 成功：SUCCESS，失败：FAILED
 */
int OPSSL_get_conf(char * alFileName,char * alSection,char * alName,char * alValue)
{
	CONF    *conf;
        long    eline;
        long    result;
        int     ret;
        char    *p;
	char	fileconf[256 + 1];
	char	sectionconf[256 + 1];

	conf = NULL;
	eline = 0;
	result = 0;
	ret = 0;
	p = NULL;

	memset(fileconf,0x00,sizeof(fileconf));
	memset(sectionconf,0x00,sizeof(sectionconf));

	p = getenv("SWITCH_CFGDIR");
	if(p == NULL)
	{
		//swVdebug(0,"getenv() error!");
		return FAILED;
	}

	if(alFileName == NULL)
	{
		sprintf(fileconf,"%s/%s",p,OPENSSL_CONF);
	}
	else
	{
		sprintf(fileconf,"%s/%s",p,alFileName);
	}
	
	conf = NCONF_new(NULL);
	if(conf == NULL)
	{
		//swVdebug(0,"NCONF_new() error!");
		return FAILED;
	}

	//swVdebug(1,"config_file=[%s]\n",fileconf);
        ret = NCONF_load(conf,fileconf,&eline);
        if(ret != 1)
        {
                //swVdebug(0,"NCONF_load() error!");
		return FAILED;
        }

	p = NULL;


	p = NCONF_get_string(conf,alSection,alName);
	if(p == NULL)
	{
		//swVdebug(0,"NCONF_get_string() error!");
		return FAILED;
	}		


	strcpy(alValue,p);

	return SUCCESS;
}

/*
 *函数名称：EVP_BASE64_Encode
 *函数功能: BASE64-编码
 *函数参数: 传入：参数2：待编码串，参数3：待编码串长度
 *	    传出：参数1：BASE64编码串
 *函数返回: 成功：BASE64编码串长度，失败：FAILED
 */
int OPSSL_EVP_BASE64_Encode(unsigned char *alOutBase64Buf, unsigned char *alInBuf,int ilInLen)
{
	return EVP_EncodeBlock(alOutBase64Buf,alInBuf,ilInLen);
}

/*
 *函数名称：EVP_BASE64_Decode
 *函数功能: BASE64-解码
 *函数参数: 传入：参数2：待解码串，参数3：待解码串长度
 *	    传出：参数1：解码串
 *函数返回: 成功：BASE64解码串长度，失败：FAILED
 */
int OPSSL_EVP_BASE64_Decode(unsigned char *alOutBuf, unsigned char *alInBase64Buf,int ilInLen)
{
	int		i;
	int		pad;
	unsigned char	*p;

	pad = 0;
	p = alInBase64Buf + ilInLen -1;
	for(i = 0; i < 4; i++)
	{
		if(*p == '=')
		{
			pad++;
		}
		p--;
	}

	return (EVP_DecodeBlock(alOutBuf,alInBase64Buf,ilInLen) - pad);
}

/*
 *函数名称：OPSSL_RSA_Private_Init
 *函数功能: RSA私钥初始化
 *函数参数: 传入：参数1：分区标识(配置文件)
 *	    传出：无
 *函数返回: 成功：RSA*	失败：NULL
 */
RSA* OPSSL_RSA_Private_Init(char *alSection)
{
	BIO             *key = NULL;
	RSA             *rsa = NULL;
	char		privateKey[MAXVARLEN + 1] = "/home/appl/cts/src/adapter/http_server/test/private_rsa.pem";

	//memset(privateKey,0x00,sizeof(privateKey));
	//if(OPSSL_get_conf(NULL,alSection,"privateKey",privateKey) != SUCCESS)
	//{
		//swVdebug(0,"OPSSL_get_conf() error!");
	//	return NULL;
	//}
	
	//swVdebug(1,"[%s][%s]\n",alSection,privateKey);

	key = BIO_new(BIO_s_file());
	if(key == NULL)
	{
		//swVdebug(0,"BIO_new_file() error!");
		return NULL;
	}

	BIO_read_filename(key,privateKey);
	rsa = PEM_read_bio_RSAPrivateKey(key,NULL,NULL,NULL);
	if(rsa == NULL)
	{
		//swVdebug(0,"PEM_read_bio_RSAPrivateKey() error!");
		BIO_free(key);
		return NULL;
	}
	BIO_free(key);
	return rsa;
}

/*
 *函数名称：OPSSL_RSA_Private_Decrypt
 *函数功能: RSA私钥解密
 *函数参数: 传入：参数1：RSA*(OPSSL_RSA_Private_Init返回)，参数2：待解密串，参数3：待解码串长度
 *	    传出：参数4：解密串
 *函数返回: 成功：解密串长度，失败：FAILED
 */
int OPSSL_RSA_Private_Decrypt(RSA *rsa,unsigned char *from,int flen,unsigned char *to)
{
	int	tlen = 0;
	tlen = RSA_private_decrypt(flen,from,to,rsa,RSA_PKCS1_PADDING);
	if(tlen == -1)
	{
		return FAILED;
	}
	return tlen;
}

/*
 *函数名称：OPSSL_RSA_Public_Init
 *函数功能: RSA公钥初始化
 *函数参数: 传入：参数1：分区标识(配置文件)
 *	    传出：无
 *函数返回: 成功：RSA*	失败：NULL
 */
RSA* OPSSL_RSA_Public_Init(char *alSection)
{
	BIO             *key = NULL;
	RSA             *rsa = NULL;
	char		publicKey[MAXVARLEN + 1] = "/home/appl/cts/src/adapter/http_server/test/public_rsa.pem";

	//memset(publicKey,0x00,sizeof(publicKey));
	//if(OPSSL_get_conf(NULL,alSection,"publicKey",publicKey) != SUCCESS)
	//{
		//swVdebug(0,"OPSSL_get_conf() error!");
	//	return NULL;
	//}

	//swVdebug(1,"[%s][%s]\n",alSection,publicKey);
	IBPrace("[%s][%s]\n",alSection,publicKey);
	key = BIO_new(BIO_s_file());
	if(key == NULL)
	{
		//swVdebug(0,"BIO_new_file() error!");
		IBPrace("BIO_new_file() error!");
		return NULL;
	}

	BIO_read_filename(key,publicKey);
	rsa = PEM_read_bio_RSA_PUBKEY(key,NULL,NULL,NULL);
	if(rsa == NULL)
	{
		//swVdebug(0,"PEM_read_bio_RSA_PUBKEY() error!");
		IBPrace("PEM_read_bio_RSA_PUBKEY() error!");
	  ERR_load_crypto_strings();
    char errBuf[512];
    ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
	  IBPrace("errBuf :%s \n", errBuf);
		BIO_free(key);
		return NULL;
	}
	BIO_free(key);
	return rsa;
}

/*
 *函数名称：OPSSL_RSA_Public_Encrypt
 *函数功能: RSA公钥加密
 *函数参数: 传入：参数1：RSA*(OPSSL_RSA_Public_Init返回)，参数2：待加密串，参数3：待加密串长度
 *	    传出：参数4：加密串
 *函数返回: 成功：加密串长度，失败：FAILED
 */
int OPSSL_RSA_Public_Encrypt(RSA *rsa,unsigned char *from,int flen,unsigned char *to)
{
	int	tlen = 0;
	tlen = RSA_public_encrypt(flen,from,to,rsa,RSA_PKCS1_PADDING);
	if(tlen == -1)
	{
		return FAILED;
	}
	return tlen;
}

/*
 *函数名称：OPSSL_AES_ECB_Encrypt
 *函数功能: AES_ECB-128加密
 *函数参数: 传入：参数1：待加密串，参数2：待加密串 长度，参数3：加密密钥
 *	    传出：参数4：加密串
 *函数返回: 成功：SUCCESS，失败：FAILED
 */
int OPSSL_AES_ECB_Encrypt(char *in, int len, char *key, char *out,int *outlen)
{
	int		i;
	int		rlt;
	char		buff[AES_BLOCK_SIZE];
	AES_KEY		aes;



	if((rlt = AES_set_encrypt_key((unsigned char *)key, AES_BLOCK_SIZE * 8, &aes)) != 0 )
	{
		//swVdebug(0,"AES_set_encrypt_key() error!(%d)",rlt);
		return FAILED;
	}
					
	for (i = 0; i < len / AES_BLOCK_SIZE; i++)
	{
		AES_ecb_encrypt((unsigned char *)in + i * AES_BLOCK_SIZE, (unsigned char *)out + i * AES_BLOCK_SIZE, &aes, AES_ENCRYPT);
	}
		
	if (len % AES_BLOCK_SIZE != 0)
	{
		memset(buff, '\r', sizeof(buff));	//为什么要填充\r
		memcpy(buff, in + (len / AES_BLOCK_SIZE ) * AES_BLOCK_SIZE , len % AES_BLOCK_SIZE);
		AES_ecb_encrypt((unsigned char *)buff, (unsigned char *)out + i * AES_BLOCK_SIZE, &aes, AES_ENCRYPT);
	}

	*outlen =  ((len) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;

	return SUCCESS;
}

/*
 *函数名称：OPSSL_AES_ECB_Decrypt
 *函数功能: AES_ECB-128解密
 *函数参数: 传入：参数1：待解密串，参数2：待解密串 长度，参数3：解密密钥
 *	    传出：参数4：解密串
 *函数返回: 成功：SUCCESS，失败：FAILED
 */
int OPSSL_AES_ECB_Decrypt(char *in, int len, char *key, char *out,int *outlen)
{
	int		i;
	AES_KEY		aes;
		
	if(AES_set_decrypt_key((unsigned char *)key, AES_BLOCK_SIZE * 8, &aes) <0 )
	{
		//swVdebug(0,"AES_set_encrypt_key() error!");
		return FAILED;
	}
		
	for (i = 0; i < len / 16; i++)
	{
		AES_ecb_encrypt((unsigned char *)in + i * AES_BLOCK_SIZE, (unsigned char *)out + i * AES_BLOCK_SIZE, &aes, AES_DECRYPT);
	}

	for (i = len - 1; i >= 0; i--)
  	{
      		if (out[i] != '\r')
		{
          		break; 
		}
  	}

  	*outlen = i + 1;

	return SUCCESS;
}

/*
 *函数名称：OPSSL_RSA_SHA1_Sign
 *函数功能: RSA_SHA1签名
 *函数参数: 传入：参数1：RSA*(公钥证书初始化返回)，参数2：待签名串，参数3：待签名串长度
 *	    传出：参数4：签名串
 *函数返回: 成功：SUCCESS，失败：FAILED
 */
int OPSSL_RSA_SHA1_Sign(RSA *rsa,unsigned char *in,int ilen,unsigned char *out,int *len)
{
	EVP_MD_CTX mdctx;
	EVP_PKEY *evpKey=NULL; //EVPKEY 结构体变量 

	evpKey=EVP_PKEY_new();//新建一个EVP_PKEY 变量
	if (evpKey==NULL)
	{
		//swVdebug(0,"EVP_PKEY_new() error!");
		RSA_free(rsa);
		return FAILED;
	}

	if (EVP_PKEY_set1_RSA(evpKey,rsa) !=1) //保存RSA结构体到EVP_PKEY 结构体
	{
		//swVdebug(0,"EVP_PKEY_set1_RSA() error!");
		RSA_free(rsa);
		EVP_PKEY_free(evpKey);
		return FAILED;
	}

	EVP_MD_CTX_init(&mdctx); //初始化摘要上下文 
	if (!EVP_SignInit_ex(&mdctx,EVP_sha1(),NULL))
	{
		//swVdebug(0,"EVP_SignInit_ex() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if (!EVP_SignUpdate(&mdctx,in,ilen)) //计算签名（摘要）Update
	{
		//swVdebug(0,"EVP_SignUpdate() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_SignFinal(&mdctx,out,(unsigned int *)len,evpKey))//签名输出
	{
		//swVdebug(0,"EVP_SignFinal() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	EVP_PKEY_free(evpKey);
	RSA_free(rsa);
	EVP_MD_CTX_cleanup(&mdctx);

	return SUCCESS;
}

/*
 *函数名称：OPSSL_RSA_SHA1_Sign
 *函数功能: RSA_SHA1签名
 *函数参数: 传入：参数1：RSA*(公钥证书初始化返回)，参数2：待签名串，参数3：待签名串长度
 *	    传出：参数4：签名串
 *函数返回: 成功：SUCCESS，失败：FAILED
 */
int OPSSL_RSA_MD5_Sign(RSA *rsa,unsigned char *in,int ilen,unsigned char *out,int *len)
{
	EVP_MD_CTX mdctx;
	EVP_PKEY *evpKey=NULL; //EVPKEY 结构体变量 

	evpKey=EVP_PKEY_new();//新建一个EVP_PKEY 变量
	if (evpKey==NULL)
	{
		//swVdebug(0,"EVP_PKEY_new() error!");
		RSA_free(rsa);
		return FAILED;
	}

	if (EVP_PKEY_set1_RSA(evpKey,rsa) !=1) //保存RSA结构体到EVP_PKEY 结构体
	{
		//swVdebug(0,"EVP_PKEY_set1_RSA() error!");
		RSA_free(rsa);
		EVP_PKEY_free(evpKey);
		return FAILED;
	}

	EVP_MD_CTX_init(&mdctx); //初始化摘要上下文 
	if (!EVP_SignInit_ex(&mdctx,EVP_md5(),NULL))
	{
		//swVdebug(0,"EVP_SignInit_ex() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if (!EVP_SignUpdate(&mdctx,in,ilen)) //计算签名（摘要）Update
	{
		//swVdebug(0,"EVP_SignUpdate() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_SignFinal(&mdctx,out,(unsigned int *)len,evpKey))//签名输出
	{
		//swVdebug(0,"EVP_SignFinal() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	EVP_PKEY_free(evpKey);
	RSA_free(rsa);
	EVP_MD_CTX_cleanup(&mdctx);

	return SUCCESS;
}

/*
 *函数名称：OPSSL_RSA_SHA1_Verify
 *函数功能: RSA_SHA1验签
 *函数参数: 传入：参数1：RSA*(私钥证书初始化返回)，参数2：待验签串，参数3：签名信息
 *	    传出：无
 *函数返回: 成功：SUCCESS，失败：FAILED
 */
int OPSSL_RSA_SHA1_Verify(RSA * rsa,unsigned char *in,int ilen,unsigned char *sign_value, int sign_len)
{
	EVP_MD_CTX mdctx;
	EVP_PKEY *evpKey=NULL; //EVPKEY 结构体变量 
		

	evpKey=EVP_PKEY_new();//新建一个EVP_PKEY 变量
	if (evpKey == NULL)
	{
		//swVdebug(0,"EVP_PKEY_new() error!");
		IBPerror("EVP_PKEY_new() error!");
		RSA_free(rsa);
		return FAILED;
	}

	if (EVP_PKEY_set1_RSA(evpKey,rsa) != 1) //保存RSA结构体到EVP_PKEY 结构体
	{
		//swVdebug(0,"EVP_PKEY_set1_RSA() error!");
		IBPerror("EVP_PKEY_set1_RSA() error!");
		RSA_free(rsa);
		EVP_PKEY_free(evpKey);
		return FAILED;
	}

	EVP_MD_CTX_init(&mdctx); //初始化摘要上下文 
	if(!EVP_VerifyInit_ex(&mdctx,EVP_sha1(),NULL))
	{
		//swVdebug(0,"EVP_VerifyInit_ex() error!");
		IBPerror("EVP_VerifyInit_ex() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_VerifyUpdate(&mdctx,in,ilen)) //计算签名（摘要）Update
	{
		//swVdebug(0,"EVP_VerifyUpdate() error!");
		IBPerror("EVP_VerifyUpdate() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_VerifyFinal(&mdctx,sign_value,sign_len,evpKey))//签名输出
	{
		//swVdebug(0,"EVP_VerifyFinal() error!");
		IBPerror("EVP_VerifyFinal() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	EVP_MD_CTX_cleanup(&mdctx);
	EVP_PKEY_free(evpKey);
	RSA_free(rsa);

	return SUCCESS;
}

int OPSSL_RSA_MD5_Verify(RSA * rsa,unsigned char *in,int ilen,unsigned char *sign_value, int sign_len)
{
	EVP_MD_CTX mdctx;
	EVP_PKEY *evpKey=NULL; //EVPKEY 结构体变量 
		

	evpKey=EVP_PKEY_new();//新建一个EVP_PKEY 变量
	if (evpKey == NULL)
	{
		//swVdebug(0,"EVP_PKEY_new() error!");
		RSA_free(rsa);
		return FAILED;
	}

	if (EVP_PKEY_set1_RSA(evpKey,rsa) != 1) //保存RSA结构体到EVP_PKEY 结构体
	{
		//swVdebug(0,"EVP_PKEY_set1_RSA() error!");
		RSA_free(rsa);
		EVP_PKEY_free(evpKey);
		return FAILED;
	}

	EVP_MD_CTX_init(&mdctx); //初始化摘要上下文 
	if(!EVP_VerifyInit_ex(&mdctx,EVP_md5(),NULL))
	{
		//swVdebug(0,"EVP_VerifyInit_ex() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_VerifyUpdate(&mdctx,in,ilen)) //计算签名（摘要）Update
	{
		//swVdebug(0,"EVP_VerifyUpdate() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_VerifyFinal(&mdctx,sign_value,sign_len,evpKey))//签名输出
	{
		//swVdebug(0,"EVP_VerifyFinal() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	EVP_MD_CTX_cleanup(&mdctx);
	EVP_PKEY_free(evpKey);
	RSA_free(rsa);

	return SUCCESS;
}

int swRsaDecrypt(char * in,int ilen,char * out,int *olen,char * alSection)
{
	RSA		*rsa = NULL;
	int		iLen;
	unsigned char aTmp[iMAXBUFLEN + 1];
	unsigned char aIn[iMAXBUFLEN + 1];
	unsigned char aOut[iMAXBUFLEN + 1];
	int iTmpIn;
	int iTmp,iKeylen;
	int iOffset,oOffset;	
	
	memset(aTmp,0x00,sizeof(aTmp));
	iTmp = OPSSL_EVP_BASE64_Decode((unsigned char *)aTmp,(unsigned char *)in,ilen);
	if(iTmp < 0)
	{
		//swVdebug(0,"OPSSL_EVP_BASE64_Decode() error!");
		return -1;
	}
	
	rsa = OPSSL_RSA_Private_Init(alSection);
	if(rsa == NULL)
	{
		//swVdebug(0,"OPSSL_RSA_Private_Init() error!");
		return -1;
	}

	iKeylen = RSA_size(rsa);
	iOffset = 0;
	oOffset = 0;
	while(iTmp > 0)
	{
		if(iTmp > iKeylen)
			iTmpIn = iKeylen;
		else
			iTmpIn = iTmp;
		memset(aIn,0x00,sizeof(aIn));
		memcpy(aIn,aTmp+iOffset,iTmpIn);
		memset(aOut,0x00,sizeof(aOut));
		iLen = OPSSL_RSA_Private_Decrypt(rsa,aIn,iTmpIn,aOut);
		if(iLen == -1)	
		{
			//swVdebug(0,"OPSSL_RSA_Private_Decrypt() error!");
			RSA_free(rsa);
			return -1;
		}
		memcpy(out+oOffset,aOut,iLen);
		oOffset += iLen;
		iOffset += iTmpIn;
		iTmp -= iTmpIn;
	}
	iLen = oOffset;
	RSA_free(rsa);
	//swVdebug(1,"swRsaDecrypt success...");
	*olen = iLen;
	return 0;
}

int swRsaEncrypt(char * in,int ilen,char * out,int *olen,char * alSection)
{
	RSA		*rsa = NULL;
	int		iLen;
	unsigned char aTmp[iMAXBUFLEN + 1];
	unsigned char aIn[iMAXBUFLEN + 1];
	unsigned char aOut[iMAXBUFLEN + 1];
	int iTmpIn;
	int iTmp,iKeylen;
	int iOffset,oOffset;
	
	rsa = OPSSL_RSA_Public_Init(alSection);
	if(rsa == NULL)
	{
		//swVdebug(0,"OPSSL_RSA_Public_Init() error!");
		return -1;
	}
	iKeylen = RSA_size(rsa);
	
	iTmp = ilen;
	iOffset = 0;
	oOffset = 0;
	memset(aTmp,0x00,sizeof(aTmp));
	while(iTmp > 0)
	{
		if(iTmp > iKeylen - 11)
			iTmpIn = iKeylen - 11;
		else
			iTmpIn = iTmp;
		
		memset(aIn,0x00,sizeof(aIn));
		memcpy(aIn,in+iOffset,iTmpIn);
		
		memset(aOut,0x00,sizeof(aOut));
		iLen = OPSSL_RSA_Public_Encrypt(rsa,aIn,iTmpIn,aOut);
		if(iLen == -1)	
		{
			//swVdebug(0,"OPSSL_RSA_Public_Encrypt() error!");
			RSA_free(rsa);
			return -1;
		}
		memcpy(aTmp+oOffset,aOut,iLen);
		oOffset += iLen;
		iOffset += iTmpIn;
		iTmp -= iTmpIn;
	}
	RSA_free(rsa);
	iLen = oOffset;

	iTmp = OPSSL_EVP_BASE64_Encode((unsigned char *)out,(unsigned char *)aTmp,iLen);
	if(iTmp < 0)
	{
		//swVdebug(0,"OPSSL_EVP_BASE64_Encode() error!");
		return -1;
	}	
	*olen = iTmp;
	return 0;
}

int swRsaSign(char * in,int ilen,char * out,int *olen,char * alSection,char *type)
{
	int rlt;
	RSA		*rsa = NULL;
	char aTmp[iMAXBUFLEN + 1];
	int iTmp;	
	
	rsa = OPSSL_RSA_Private_Init(alSection);
	if(rsa == NULL)
	{
		//swVdebug(0,"OPSSL_RSA_Private_Init() error!");
		return -1;
	}

	memset(aTmp,0x00,sizeof(aTmp));
	if(memcmp(type,"SHA1",4) == 0)
		rlt = OPSSL_RSA_SHA1_Sign(rsa,(unsigned char *)in,ilen,(unsigned char *)aTmp,&iTmp);
	else if(memcmp(type,"MD5",3) == 0)
		rlt = OPSSL_RSA_MD5_Sign(rsa,(unsigned char *)in,ilen,(unsigned char *)aTmp,&iTmp);
	else{
		//swVdebug(0,"sign type(%s) error!",type);
		return -1;		
	}
	if(rlt != 0)	
	{
		//swVdebug(0,"OPSSL_RSA_%s_Sign() error!",type);
		return -1;
	}	
	//swVdebug(1,"swRsaSign success...");
	
	rlt = OPSSL_EVP_BASE64_Encode((unsigned char *)out,(unsigned char *)aTmp,iTmp);
	if(rlt < 0)
	{
		//swVdebug(0,"OPSSL_EVP_BASE64_Encode() error!");
		return -1;
	}
	*olen = rlt;
	return 0;
}

int swRsaVerify(char * in,int ilen,char * sign,int slen,char * alSection,char *type)
{
	int rlt;
	RSA		*rsa = NULL;
	char aTmp[iMAXBUFLEN + 1];
	int iTmp;	
		
	rsa = OPSSL_RSA_Public_Init(alSection);
	if(rsa == NULL)
	{
		//swVdebug(0,"OPSSL_RSA_Public_Init() error!");
		IBPrace("OPSSL_RSA_Public_Init() error!");
		return -1;
	}

	memset(aTmp,0x00,sizeof(aTmp));
	iTmp = OPSSL_EVP_BASE64_Decode((unsigned char *)aTmp,(unsigned char *)sign,slen);
	if(iTmp < 0)
	{
		//swVdebug(0,"OPSSL_EVP_BASE64_Decode() error!");
		IBPrace("OPSSL_EVP_BASE64_Decode() error!");
		return -1;
	}
	
	if(memcmp(type,"SHA1",4) == 0)
		rlt = OPSSL_RSA_SHA1_Verify(rsa,(unsigned char *)in,ilen,(unsigned char *)aTmp,iTmp);
	else if(memcmp(type,"MD5",3) == 0)
		rlt = OPSSL_RSA_MD5_Verify(rsa,(unsigned char *)in,ilen,(unsigned char *)aTmp,iTmp);
	else{
		//swVdebug(0,"sign type(%s) error!",type);
		return -1;		
	}
	
	if(rlt != 0)	
	{
		//swVdebug(0,"OPSSL_RSA_%s_Verify() error!",type);
		IBPrace("OPSSL_RSA_%s_Verify() error!",type);
		return -1;
	}
	//swVdebug(1,"OPSSL_RSA_%s_Verify success...",type);
	IBPrace("OPSSL_RSA_%s_Verify success...",type);	

	return 0;
}

int OPSSL_DESede_Encode(char *key,char *in,char *out)
{ 
	int i,j;
	int iType;
	int kLen,iLen;
	char aKey[128+1];
	char aData[4096+1];
	char aTmp[4096+1];
	DES_key_schedule ks;
	unsigned char block[8] = {0};
	
	/* 设置密码表 */
	memset(aKey,0x00,sizeof(aKey));
	memcpy(aKey,key,24);
	//strcpy(aKey,key);
	kLen = strlen(aKey);
	
	if(kLen%8 != 0)
		memset(aKey + kLen, 8 - kLen % 8, 8 - kLen % 8);
	kLen = strlen(aKey);

	if(kLen /8 == 2)
	{
		memcpy(aKey+16,aKey,8);
		aKey[24] = 0;
		kLen = 24;
	}
	
	/* 分析补齐明文所需空间及补齐填充数据 */
	memset(aData,0x00,sizeof(aData));
	strcpy(aData,in); 
	iLen = strlen(aData);
	
	if(iLen%8 == 0)
		memset(aData+iLen,8,8);
	else
		memset(aData + iLen, 8 - iLen % 8, 8 - iLen % 8);
	
	iLen = strlen(aData);
	
	for(i=0;i*8<kLen;i++)
	{
		memset(aTmp,0x00,sizeof(aTmp));
		memcpy(aTmp,aKey+i*8,8);
		DES_set_key_unchecked((const_DES_cblock*)aTmp, &ks);
		
		iType = (i%2) ? DES_DECRYPT : DES_ENCRYPT;
		
		/* 分组加密，每8字节一组 */
		for (j = 0; j*8 < iLen; j++) 
		{
			memset(aTmp,0x00,sizeof(aTmp));
			memcpy(aTmp, aData + 8 * j, 8);
			DES_ecb_encrypt((const_DES_cblock*)aTmp, (DES_cblock*)block, &ks, iType);
			memcpy(aData + 8 * j, block, 8);
		}
	}
	memset(aTmp,0x00,sizeof(aTmp));
	OPSSL_EVP_BASE64_Encode(aTmp,aData,strlen(aData));
	strcpy(out,aTmp);
	return 0;
}

