//#include <windows.h>
//#include <stdio.h>
//#include <WinSock.h>
//#include <iostream>
#include "CSmtp.h"

using namespace std;
//"707720629@qq.com 874440975@qq.com deaconwu@163.com zhoukesong@126.com"
int main()
{
	CSmtp smtp(25,                             /*smtp端口*/
		"smtp.qq.com",                 /*smtp服务器地址*/
		"707720629@qq.com",         /*你的邮箱地址*/
		"ullzgjrpjhkjbceh",                   /*邮箱密码(授权码)*/
		"874440975@qq.com zhoukesong@126.com deaconwu@163.com",  /*目的邮箱地址,这一部分用空格分割可添加多个收信人*/
		"vin码",                           /*主题*/
		"JTHAAABH3L2000396码"      /*邮件正文*/
	);

	string filePath("smtp码.rar");
	smtp.AddAttachment(filePath);
	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "错误1: 由于网络不畅通，发送失败!" << endl;
		if (err == 2)
			cout << "错误2: 用户名错误,请核对!" << endl;
		if (err == 3)
			cout << "错误3: 用户密码错误，请核对!" << endl;
		if (err == 4)
			cout << "错误4: 请检查附件目录是否正确，以及文件是否存在!" << endl;
	}
	system("pause");


	return 0;
}