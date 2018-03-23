#include "opsslpub.h"

#define iMAXBUFLEN 8192

/*
 *???????ƣ?OPSSL_get_conf
 *????????: ȡ?????ļ?????(openssl.conf)
 *????????: ???룺????1??????????????2????ֵ??-????
 *	    ???����???3????ֵ??-ֵ
 *????????: ?ɹ???SUCCESS??ʧ?ܣ?FAILED
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
 *???????ƣ?EVP_BASE64_Encode
 *????????: BASE64-????
 *????????: ???룺????2???????봮??????3???????봮????
 *	    ???����???1??BASE64???봮
 *????????: ?ɹ???BASE64???봮???ȣ?ʧ?ܣ?FAILED
 */
int OPSSL_EVP_BASE64_Encode(unsigned char *alOutBase64Buf, unsigned char *alInBuf,int ilInLen)
{
	return EVP_EncodeBlock(alOutBase64Buf,alInBuf,ilInLen);
}

/*
 *???????ƣ?EVP_BASE64_Decode
 *????????: BASE64-????
 *????????: ???룺????2???????봮??????3???????봮????
 *	    ???����???1?????봮
 *????????: ?ɹ???BASE64???봮???ȣ?ʧ?ܣ?FAILED
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
 *???????ƣ?OPSSL_RSA_Private_Init
 *????????: RSA˽Կ??ʼ??
 *????????: ???룺????1????????ʶ(?????ļ?)
 *	    ????????
 *????????: ?ɹ???RSA*	ʧ?ܣ?NULL
 */
RSA* OPSSL_RSA_Private_Init(char *alSection)
{
	BIO             *key = NULL;
	RSA             *rsa = NULL;
	char		privateKey[MAXVARLEN + 1] = "/home/appl/cts/src/adapter/http_server/test/SignDemo/rsa_private_key.pem";

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
 *???????ƣ?OPSSL_RSA_Private_Decrypt
 *????????: RSA˽Կ????
 *????????: ???룺????1??RSA*(OPSSL_RSA_Private_Init????)??????2???????ܴ???????3???????봮????
 *	    ???����???4?????ܴ?
 *????????: ?ɹ??????ܴ????ȣ?ʧ?ܣ?FAILED
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
 *???????ƣ?OPSSL_RSA_Public_Init
 *????????: RSA??Կ??ʼ??
 *????????: ???룺????1????????ʶ(?????ļ?)
 *	    ????????
 *????????: ?ɹ???RSA*	ʧ?ܣ?NULL
 */
RSA* OPSSL_RSA_Public_Init(char *alSection)
{
	BIO             *key = NULL;
	RSA             *rsa = NULL;
	char		publicKey[MAXVARLEN + 1] = "/home/appl/cts/src/adapter/http_server/test/SignDemo/rsa_public_key.pem";

	//memset(publicKey,0x00,sizeof(publicKey));
	//if(OPSSL_get_conf(NULL,alSection,"publicKey",publicKey) != SUCCESS)
	//{
		//swVdebug(0,"OPSSL_get_conf() error!");
	//	return NULL;
	//}

	//swVdebug(1,"[%s][%s]\n",alSection,publicKey);

	key = BIO_new(BIO_s_file());
	if(key == NULL)
	{
		//swVdebug(0,"BIO_new_file() error!");
		return NULL;
	}

	BIO_read_filename(key,publicKey);
	rsa = PEM_read_bio_RSA_PUBKEY(key,NULL,NULL,NULL);
	if(rsa == NULL)
	{
		//swVdebug(0,"PEM_read_bio_RSA_PUBKEY() error!");
		BIO_free(key);
		return NULL;
	}
	BIO_free(key);
	return rsa;
}

/*
 *???????ƣ?OPSSL_RSA_Public_Encrypt
 *????????: RSA??Կ????
 *????????: ???룺????1??RSA*(OPSSL_RSA_Public_Init????)??????2???????ܴ???????3???????ܴ?????
 *	    ???����???4?????ܴ?
 *????????: ?ɹ??????ܴ????ȣ?ʧ?ܣ?FAILED
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
 *???????ƣ?OPSSL_AES_ECB_Encrypt
 *????????: AES_ECB-128????
 *????????: ???룺????1???????ܴ???????2???????ܴ? ???ȣ?????3????????Կ
 *	    ???����???4?????ܴ?
 *????????: ?ɹ???SUCCESS??ʧ?ܣ?FAILED
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
		memset(buff, '\r', sizeof(buff));	//ΪʲôҪ????\r
		memcpy(buff, in + (len / AES_BLOCK_SIZE ) * AES_BLOCK_SIZE , len % AES_BLOCK_SIZE);
		AES_ecb_encrypt((unsigned char *)buff, (unsigned char *)out + i * AES_BLOCK_SIZE, &aes, AES_ENCRYPT);
	}

	*outlen =  ((len) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;

	return SUCCESS;
}

/*
 *???????ƣ?OPSSL_AES_ECB_Decrypt
 *????????: AES_ECB-128????
 *????????: ???룺????1???????ܴ???????2???????ܴ? ???ȣ?????3????????Կ
 *	    ???����???4?????ܴ?
 *????????: ?ɹ???SUCCESS??ʧ?ܣ?FAILED
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
 *???????ƣ?OPSSL_RSA_SHA1_Sign
 *????????: RSA_SHA1ǩ??
 *????????: ???룺????1??RSA*(??Կ֤????ʼ??????)??????2????ǩ?�����????3????ǩ?�����??
 *	    ???����???4??ǩ????
 *????????: ?ɹ???SUCCESS??ʧ?ܣ?FAILED
 */
int OPSSL_RSA_SHA1_Sign(RSA *rsa,unsigned char *in,int ilen,unsigned char *out,int *len)
{
	EVP_MD_CTX mdctx;
	EVP_PKEY *evpKey=NULL; //EVPKEY ?ṹ????�� 

	evpKey=EVP_PKEY_new();//?½?һ??EVP_PKEY ??��
	if (evpKey==NULL)
	{
		//swVdebug(0,"EVP_PKEY_new() error!");
		RSA_free(rsa);
		return FAILED;
	}

	if (EVP_PKEY_set1_RSA(evpKey,rsa) !=1) //????RSA?ṹ?嵽EVP_PKEY ?ṹ??
	{
		//swVdebug(0,"EVP_PKEY_set1_RSA() error!");
		RSA_free(rsa);
		EVP_PKEY_free(evpKey);
		return FAILED;
	}

	EVP_MD_CTX_init(&mdctx); //??ʼ??ժҪ?????? 
	if (!EVP_SignInit_ex(&mdctx,EVP_sha1(),NULL))
	{
		//swVdebug(0,"EVP_SignInit_ex() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if (!EVP_SignUpdate(&mdctx,in,ilen)) //????ǩ????ժҪ??Update
	{
		//swVdebug(0,"EVP_SignUpdate() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_SignFinal(&mdctx,out,(unsigned int *)len,evpKey))//ǩ??????
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
 *???????ƣ?OPSSL_RSA_SHA1_Sign
 *????????: RSA_SHA1ǩ??
 *????????: ???룺????1??RSA*(??Կ֤????ʼ??????)??????2????ǩ?�����????3????ǩ?�����??
 *	    ???����???4??ǩ????
 *????????: ?ɹ???SUCCESS??ʧ?ܣ?FAILED
 */
int OPSSL_RSA_MD5_Sign(RSA *rsa,unsigned char *in,int ilen,unsigned char *out,int *len)
{
	EVP_MD_CTX mdctx;
	EVP_PKEY *evpKey=NULL; //EVPKEY ?ṹ????�� 

	evpKey=EVP_PKEY_new();//?½?һ??EVP_PKEY ??��
	if (evpKey==NULL)
	{
		//swVdebug(0,"EVP_PKEY_new() error!");
		RSA_free(rsa);
		return FAILED;
	}

	if (EVP_PKEY_set1_RSA(evpKey,rsa) !=1) //????RSA?ṹ?嵽EVP_PKEY ?ṹ??
	{
		//swVdebug(0,"EVP_PKEY_set1_RSA() error!");
		RSA_free(rsa);
		EVP_PKEY_free(evpKey);
		return FAILED;
	}

	EVP_MD_CTX_init(&mdctx); //??ʼ??ժҪ?????? 
	if (!EVP_SignInit_ex(&mdctx,EVP_md5(),NULL))
	{
		//swVdebug(0,"EVP_SignInit_ex() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if (!EVP_SignUpdate(&mdctx,in,ilen)) //????ǩ????ժҪ??Update
	{
		//swVdebug(0,"EVP_SignUpdate() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_SignFinal(&mdctx,out,(unsigned int *)len,evpKey))//ǩ??????
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
 *???????ƣ?OPSSL_RSA_SHA1_Verify
 *????????: RSA_SHA1??ǩ
 *????????: ???룺????1??RSA*(˽Կ֤????ʼ??????)??????2??????ǩ????????3??ǩ????Ϣ
 *	    ????????
 *????????: ?ɹ???SUCCESS??ʧ?ܣ?FAILED
 */
int OPSSL_RSA_SHA1_Verify(RSA * rsa,unsigned char *in,int ilen,unsigned char *sign_value, int sign_len)
{
	EVP_MD_CTX mdctx;
	EVP_PKEY *evpKey=NULL; //EVPKEY ?ṹ????�� 
		

	evpKey=EVP_PKEY_new();//?½?һ??EVP_PKEY ??��
	if (evpKey == NULL)
	{
		//swVdebug(0,"EVP_PKEY_new() error!");
		RSA_free(rsa);
		return FAILED;
	}

	if (EVP_PKEY_set1_RSA(evpKey,rsa) != 1) //????RSA?ṹ?嵽EVP_PKEY ?ṹ??
	{
		//swVdebug(0,"EVP_PKEY_set1_RSA() error!");
		RSA_free(rsa);
		EVP_PKEY_free(evpKey);
		return FAILED;
	}

	EVP_MD_CTX_init(&mdctx); //??ʼ??ժҪ?????? 
	if(!EVP_VerifyInit_ex(&mdctx,EVP_sha1(),NULL))
	{
		//swVdebug(0,"EVP_VerifyInit_ex() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_VerifyUpdate(&mdctx,in,ilen)) //????ǩ????ժҪ??Update
	{
		//swVdebug(0,"EVP_VerifyUpdate() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_VerifyFinal(&mdctx,sign_value,sign_len,evpKey))//ǩ??????
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

int OPSSL_RSA_MD5_Verify(RSA * rsa,unsigned char *in,int ilen,unsigned char *sign_value, int sign_len)
{
	EVP_MD_CTX mdctx;
	EVP_PKEY *evpKey=NULL; //EVPKEY ?ṹ????�� 
		

	evpKey=EVP_PKEY_new();//?½?һ??EVP_PKEY ??��
	if (evpKey == NULL)
	{
		//swVdebug(0,"EVP_PKEY_new() error!");
		RSA_free(rsa);
		return FAILED;
	}

	if (EVP_PKEY_set1_RSA(evpKey,rsa) != 1) //????RSA?ṹ?嵽EVP_PKEY ?ṹ??
	{
		//swVdebug(0,"EVP_PKEY_set1_RSA() error!");
		RSA_free(rsa);
		EVP_PKEY_free(evpKey);
		return FAILED;
	}

	EVP_MD_CTX_init(&mdctx); //??ʼ??ժҪ?????? 
	if(!EVP_VerifyInit_ex(&mdctx,EVP_md5(),NULL))
	{
		//swVdebug(0,"EVP_VerifyInit_ex() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_VerifyUpdate(&mdctx,in,ilen)) //????ǩ????ժҪ??Update
	{
		//swVdebug(0,"EVP_VerifyUpdate() error!");
		EVP_PKEY_free(evpKey);
		RSA_free(rsa);
		return FAILED;
	}

	if(!EVP_VerifyFinal(&mdctx,sign_value,sign_len,evpKey))//ǩ??????
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

	printf("sign=:%s\n", out);
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
		return -1;
	}

	memset(aTmp,0x00,sizeof(aTmp));
	iTmp = OPSSL_EVP_BASE64_Decode((unsigned char *)aTmp,(unsigned char *)sign,slen);
	if(iTmp < 0)
	{
		//swVdebug(0,"OPSSL_EVP_BASE64_Decode() error!");
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
		return -1;
	}
	//swVdebug(1,"OPSSL_RSA_%s_Verify success...",type);

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
	
	/* ?????????? */
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
	
	/* ?????????????????ռ估???????????? */
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
		
		/* ???????ܣ?ÿ8?ֽ?һ?? */
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

