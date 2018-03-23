#include "face.h"

int main()
{
	int in_len, out_len;
	//char str[10240] = "accBankName=中信银行&accBankNo=302100011000&acctName=谭彬彬&acctNo=6217710305435596&actType=1&certNo=431028198901032813&certType=01&mertNo=015440395110988&note=代付清算请求&notifyUrl=www.baidu.com&orderSerial=2017112800231002&orgDate=20171201&orgOrder=20171201165650&phone=13530456858&product=P3918163&transAmt=1.00";
	//char str[10240] = "mertNo=015440395110988";
	char str[10240] = "mertNo=015440395110988&orderSerial=2017112800231001";
	char signtext[10240];
	char type[5] = "SHA1";

	memset(signtext, 0, sizeof(signtext));
	in_len = strlen(str);
	int retcode1 = swRsaSign(str, in_len, signtext, &out_len, "", type);
	if(retcode1 < 0)
		printf("sign failed\n"); 
	
	
        int retcode2 = swRsaVerify(str, in_len, signtext, out_len, "", type);
        if(retcode2 < 0)
                printf("Verify failed\n");

	long sign_len=10240;
	char signtext2[10240];
	long urllen;
	char urltext[10240];
	
	memset(signtext2, 0, sizeof(signtext2));
	memset(urltext, 0, sizeof(urltext));
	
    if(RC_SUCC != lInitRsa("/home/appl/cts/src/adapter/http_server/test/SignDemo/private.pem", "/home/appl/cts/src/adapter/http_server/test/SignDemo/public.pem"))
    {
        fprintf(stderr, "初始化RSA密钥对失败\n");
        return RC_FAIL;
    }
	
    if(RC_SUCC != lPackRSAsign(str, in_len, signtext2, &sign_len))
    {
         fprintf(stderr, "组织发送报文字符串签名失败\n");
         return -1;
    }
   	printf("sign2(%ld)=:%s\n", sign_len, signtext2);

	strncat(str, "&sign=", 6);
	strncat(str, signtext2, sign_len);
	printf("send content:%s\n", str);
	int content_len=strlen(str);
	urllen = lUrlencode(str, content_len, urltext, sizeof(urltext));

    printf("urltext(%ld)=:%s\n", urllen, urltext);

	/*if(RC_SUCC != lPackRSAverify(str, in_len, signtext2, sign_len))
    {
		fprintf(stderr,"verify sign failed\n");
		return -1;
    }*/
	return 0;
}

