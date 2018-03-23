#ifndef __OPSSL_PUB_H__
#define __OPSSL_PUB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/conf.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/des.h>

#define  SUCCESS 0
#define  FAILED -1 
#define  OPENSSL_CONF     "openssl.conf"
//#define  MAXVARLEN 1024
#define  MAXVARLEN 2048
//#define  MAXMSGLEN 8192
#define  MAXMSGLEN 16384

int OPSSL_get_conf(char * alFileName,char * alSection,char * alName,char * alValue);
int OPSSL_EVP_BASE64_Encode(unsigned char *alOutBase64Buf, unsigned char *alInBuf,int ilInLen);
int OPSSL_EVP_BASE64_Decode(unsigned char *alOutBuf, unsigned char *alInBase64Buf,int ilInLen);
RSA* OPSSL_RSA_Private_Init(char *alSection);
int OPSSL_RSA_Private_Decrypt(RSA *rsa,unsigned char *from,int flen,unsigned char *to);
RSA* OPSSL_RSA_Public_Init(char *alSection);
int OPSSL_RSA_Public_Encrypt(RSA *rsa,unsigned char *from,int flen,unsigned char *to);
int OPSSL_AES_ECB_Encrypt(char *in, int len, char *key, char *out,int * outlen);
int OPSSL_AES_ECB_Decrypt(char *in, int len, char *key, char *out,int * outlen);
int OPSSL_RSA_SHA1_Sign(RSA *rsa,unsigned char *in,int ilen,unsigned char *out,int *len);
int OPSSL_RSA_SHA1_Verify(RSA * rsa,unsigned char *in,int ilen,unsigned char *sign_value, int sign_len);

int OPSSL_DESede_Encode(char * key,char *in,char *out);

extern int swVdebug(short iDebug,char *frm, ...);

#endif
