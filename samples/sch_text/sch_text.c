/*
* �ı����弼���ܽ��ı����ݽ������������
*/

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <errno.h>

#include "msp_cmn.h"
#include "msp_errors.h"

#ifdef _WIN64
#pragma comment(lib,"../../libs/msc_x64.lib") //x64
#else
#pragma comment(lib,"../../libs/msc.lib") //x86
#endif

#define SOURCETEXT  "source.txt" //�����ı���Դ
#define RESULTTEXT  "result.txt" //�������ı�

int main(int argc, char* argv[])
{
	const char*  login_params	 =	"appid=589c478e"; // ��¼������appid��msc���,��������Ķ�
	const char*  rec_text        =   NULL;
	unsigned int str_len         =   0;
	int	         ret		     =	 MSP_SUCCESS;
	FILE*        fw              =   NULL;
	FILE*        fr              =   NULL;
	long         txtSize         =   0;
	long		 read_size	     =	 0;
	char*        text            =   NULL;
	/* �û���¼ */
	ret = MSPLogin(NULL, NULL, login_params); //��һ���������û������ڶ������������룬����NULL���ɣ������������ǵ�¼����	
	if (MSP_SUCCESS != ret)
	{
		printf("MSPLogin failed , Error code %d.\n",ret);
		goto exit; //��¼ʧ�ܣ��˳���¼
	}
	fr=fopen(SOURCETEXT,"rb");
	if(NULL == fr)
	{
		printf("\nopen [%s] failed! \n",SOURCETEXT);
		goto exit;
	}

	fseek(fr, 0, SEEK_END);
	txtSize = ftell(fr);
	fseek(fr, 0, SEEK_SET);

	text = (char *)malloc(txtSize+1);
	if (NULL == text)
	{
		printf("\nout of memory! \n");
		goto exit;
	}

	read_size = fread((void *)text,1, txtSize, fr);
	if (read_size != txtSize)
	{
		printf("\nread [%s] error!\n", SOURCETEXT);
		goto exit;
	}
	text[txtSize]='\0';
	str_len = strlen(text);

	printf("\n��ʼ�������...\n");
	rec_text = MSPSearch("nlp_version =2.0",text,&str_len,&ret);
	if(MSP_SUCCESS !=ret)
	{
		printf("MSPSearch failed ,error code is:%d\n",ret);
		goto exit;
	}
	printf("\n����������!\n");
	fw=fopen(RESULTTEXT,"wb");
	if(NULL == fw)
	{
		printf("\nopen [%s] failed! \n",RESULTTEXT);
		goto exit;
	}

	read_size = fwrite(rec_text,1,str_len,fw);
	if(read_size != str_len)
	{
		printf("\nwrite [%s] error!\n", RESULTTEXT);
		goto exit;
	}
	printf("\n������������д��%s�ļ�\n",RESULTTEXT);
exit:
	if (NULL != fr)
	{
		fclose(fr);
		fr = NULL;
	}
	if (NULL != fw)
	{
		fclose(fw);
		fw = NULL;
	}
	if (NULL != text)
	{
		free(text);
		text = NULL;
	}
	printf("\n��������˳� ...\n");
	_getch();
	MSPLogout(); //�˳���¼

	return 0;
}
