//#include <windows.h>
//#include <stdio.h>
//#include <WinSock.h>
//#include <iostream>
#include "CSmtp.h"

using namespace std;
//"707720629@qq.com 874440975@qq.com deaconwu@163.com zhoukesong@126.com"
int main()
{
	CSmtp smtp(25,                             /*smtp�˿�*/
		"smtp.qq.com",                 /*smtp��������ַ*/
		"707720629@qq.com",         /*��������ַ*/
		"ullzgjrpjhkjbceh",                   /*��������(��Ȩ��)*/
		"874440975@qq.com zhoukesong@126.com deaconwu@163.com",  /*Ŀ�������ַ,��һ�����ÿո�ָ����Ӷ��������*/
		"vin��",                           /*����*/
		"JTHAAABH3L2000396��"      /*�ʼ�����*/
	);

	string filePath("smtp��.rar");
	smtp.AddAttachment(filePath);
	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "����1: �������粻��ͨ������ʧ��!" << endl;
		if (err == 2)
			cout << "����2: �û�������,��˶�!" << endl;
		if (err == 3)
			cout << "����3: �û����������˶�!" << endl;
		if (err == 4)
			cout << "����4: ���鸽��Ŀ¼�Ƿ���ȷ���Լ��ļ��Ƿ����!" << endl;
	}
	system("pause");


	return 0;
}