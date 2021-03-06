// smtp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <windows.h>
#include <stdio.h>
#include <WinSock.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

struct Base64Date6
{
	unsigned int d4 : 6;
	unsigned int d3 : 6;
	unsigned int d2 : 6;
	unsigned int d1 : 6;
};

char ConvertToBase64(char c6);
void EncodeBase64(char* dbuf, char* buf128, int len);
void SendMail(char* email, char* body);
int OpenSocket(struct sockaddr* addr);

int main()
{
	char EmailTo[] = "874440975@qq.com";
	char EmailContents[] = "From: 707720629@qq.com\r\n"
		"To: 707720629@qq.com\r\n"
		"Subject: Hello\r\n\r\n"
		"Hello World, Hello Email!"
		"TO: 707720629@qq.com\r\n"
		"FROM: 707720629@qq.com\r\n"
		"SUBJECT: SMTP协议测试\r\n"
		"Date:2020-11-27\r\n"
		"X-Mailer:fice's mailer\r\n"
		"MIMI-Version:1.0\r\n"
		"Content-Type:multipart/mixed;boundary=\"#BOUNDARY#\"\r\n\r\n"
		"Content-Transfer-Encoding:7bit\r\n\r\n"
		"This is a multi-part message in MIME format\r\n\r\n"
		"--#BOUNDARY#\r\n"
		"Content-Type:text/plain;charset=gb2312\r\n"
		"Content-Transfer-Encoding:printable\r\n\r\n"
		"SMTP协议测试：发送附件/n----by fice 2015.04.20\r\n"
		"--#BOUNDARY#\r\n"
		"Content-Type:text/plain;name=test.txt\r\n"
		"Content-Transfer-Encoding:base64\r\n"
		"Content-Disposition:attachment;filename=\"test.txt\"\r\n\r\n";

	SendMail(EmailTo, EmailContents);

	system("pause");
}

char ConvertToBase64(char uc)
{
	if (uc < 26)
	{
		return 'A' + uc;
	}
	if (uc < 52)
	{
		return 'a' + (uc - 26);
	}
	if (uc < 62)
	{
		return '0' + (uc - 52);
	}
	if (uc == 62)
	{
		return '+';
	}

	return '/';
}

void EncodeBase64(char* dbuf, char* buf128, int len)
{
	struct Base64Date6 *ddd = NULL;
	int i = 0;
	char buf[256] = {};
	char *tmp = NULL;
	char cc = '\0';

	memset(buf, 0, 256);
	strcpy_s(buf, 256, buf128);
	for (i = 1; i <= len / 3; i++)
	{
		tmp = buf + (i - 1) * 3;
		cc = tmp[2];
		tmp[2] = tmp[0];
		tmp[0] = cc;
		ddd = (struct Base64Date6*)tmp;
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
		dbuf[(i - 1) * 4 + 3] = ConvertToBase64((unsigned int)ddd->d4);
	}
	if (len % 3 == 1)
	{
		tmp = buf + (i - 1) * 3;
		cc = tmp[2];
		tmp[2] = tmp[0];
		tmp[0] = cc;
		ddd = (struct Base64Date6*)tmp;
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		dbuf[(i - 1) * 4 + 2] = '=';
		dbuf[(i - 1) * 4 + 3] = '=';
	}
	if (len % 3 == 2)
	{
		tmp = buf + (i - 1) * 3;
		cc = tmp[2];
		tmp[2] = tmp[0];
		tmp[0] = cc;
		ddd = (struct Base64Date6*)tmp;
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
		dbuf[(i - 1) * 4 + 3] = '=';
	}
	return;
}

void SendMail(char* email, char* body)
{
	setlocale(LC_ALL, "Chinese-simplified");
	int sockfd = { 0 };
	char buf[1500] = { 0 };
	char rbuf[1500] = { 0 };
	char login[128] = { 0 };
	char pass[128] = { 0 };
	WSADATA WSAData;
	struct sockaddr_in their_addr = { 0 };
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	memset(&their_addr, 0, sizeof(their_addr));

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(25);
	hostent *hptr = gethostbyname("smtp.qq.com");
	memcpy(&their_addr.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);
	printf("IP of smpt.163.com is : %d:%d:%d:%d\n",
		their_addr.sin_addr.S_un.S_un_b.s_b1,
		their_addr.sin_addr.S_un.S_un_b.s_b2,
		their_addr.sin_addr.S_un.S_un_b.s_b3,
		their_addr.sin_addr.S_un.S_un_b.s_b4);

	// 连接邮件服务器，如果连接后没有响应，则2 秒后重新连接
	sockfd = OpenSocket((struct sockaddr*)&their_addr);
	memset(rbuf, 0, 1500);
	while (recv(sockfd, rbuf, 1500, 0) == 0)
	{
		cout << "reconnect..." << endl;
		Sleep(2);
		sockfd = OpenSocket((struct sockaddr*)&their_addr);
		memset(rbuf, 0, 1500);
	}

	printf("Connect Receive: %s\n", rbuf);

	// EHLO
	char pcname[128] = { 0 };
	DWORD size = 128;
	GetComputerNameA(pcname, &size); // 获取计算机名
	memset(buf, 0, 1500);
	//sprintf_s(buf, 1500, "EHLO %s\r\n", pcname);
	sprintf_s(buf, 1500, "EHLO localhost\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("EHLO localhost Receive: %s\n", rbuf);

	// AUTH LOGIN
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "AUTH LOGIN\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("AUTH LOGIN Receive: %s\n", rbuf);

	// USER
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "707720629@qq.com");//你的邮箱账号
	memset(login, 0, 128);
	EncodeBase64(login, buf, strlen(buf));
	sprintf_s(buf, 1500, "%s\r\n", login);
	send(sockfd, buf, strlen(buf), 0);
	printf("Base64 UserName: %s", buf);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("User Login Receive: %s\n", rbuf);

	// PASSWORD
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "ullzgjrpjhkjbceh");//你的邮箱密码
	memset(pass, 0, 128);
	EncodeBase64(pass, buf, strlen(buf));
	sprintf_s(buf, 1500, "%s\r\n", pass);
	send(sockfd, buf, strlen(buf), 0);

	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("Send Password Receive: %s\n", rbuf);

	// MAIL FROM
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "MAIL FROM: <707720629@qq.com>\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("MAIL FROM Receive: %s\n", rbuf);

	// RCPT TO 第一个收件人
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "RCPT TO: <707720629@qq.com>\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("RCPT TO Receive: %s\n", rbuf);


	// DATA 准备开始发送邮件内容
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "DATA\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("Send Mail DATA Receive: %s\n", rbuf);

	// 发送邮件内容，\r\n.\r\n内容结束标记
// 	memset(buf, 0, 1500);
// 	sprintf_s(buf, 1500, "%s\r\n.\r\n", "L1K1D8EE1H0000068");
// 	send(sockfd, buf, strlen(buf), 0);//发送邮件，不带附件的内容
// 	memset(rbuf, 0, 1500);
// 	recv(sockfd, rbuf, 1500, 0);
// 	printf("Send Content Receive: %s\n", rbuf);

	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "Content-Transfer-Encoding:base64\r\nContent-Disposition:attachment;\r\nContent-Type:application/txt\r\nfilename=\"test.txt\"\r\n\r\n");
	send(sockfd, buf, strlen(buf), 0);//发送邮件，附件名
//  	memset(rbuf, 0, 1500);
//  	recv(sockfd, rbuf, 1500, 0);
//  	printf("Send FileName Receive: %s\n", rbuf);

	//用二进制方式读取附件文件内容并转为base64格式  
	FILE *pf = fopen("test.txt", "rb");
	fseek(pf, 0, SEEK_END);
	int filelen = ftell(pf);
	char *filebuf = (char*)malloc(filelen);
	memset(filebuf, 0, filelen);
	char *filebase64 = (char*)malloc(filelen * 2);
	memset(filebase64, 0, filelen * 2);
	fseek(pf, 0, SEEK_SET);
	fread(filebuf, 1, filelen, pf);
	EncodeBase64(filebase64, filebuf, filelen);

	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "%s\r\n", filebase64);

	send(sockfd, buf, strlen(buf), 0);//发送邮件的附件
	memset(rbuf, 0, 1500);
// 	recv(sockfd, rbuf, 1500, 0);
// 	printf("Send Attachment Receive: %s\n", rbuf);

	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "\r\n.\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("END Receive: %s\n", rbuf);

	// QUIT
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "QUIT\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	printf("QUIT Receive: %s\n", rbuf);

	//清理工作
	closesocket(sockfd);
	WSACleanup();
	return;
}

int OpenSocket(struct sockaddr* addr)
{
	int sockfd = 0;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		cout << "Open sockfd(TCP) error!" << endl;
		exit(-1);
	}
	if (connect(sockfd, addr, sizeof(struct sockaddr)) < 0)
	{
		cout << "Connect sockfd(TCP) error!" << endl;
		exit(-1);
	}
	return sockfd;
}