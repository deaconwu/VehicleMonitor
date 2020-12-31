// CLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "NewEnergyVehicle.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include "UserMessage.h"
#include <tinyxml.h>
#include <string>
#include "Md5.h"
#include "HexAscii.h"

using namespace std;

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CLoginDlg::OnInitDialog()
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile("account.xml"))
	{
		MD5_CTX md5;
		unsigned char chPwd[100];
		unsigned char encrypt[16];

		xmlDocument.LinkEndChild(new TiXmlDeclaration("1.0", "UTF-8", "yes"));
		TiXmlElement * xmlRoot = new TiXmlElement("root");
		xmlDocument.LinkEndChild(xmlRoot);

		TiXmlElement* xmlChild1 = new TiXmlElement("user");
		xmlRoot->LinkEndChild(xmlChild1);

		TiXmlElement* xmlChild1_0 = new TiXmlElement("nick");
		xmlChild1->LinkEndChild(xmlChild1_0);
		xmlChild1_0->LinkEndChild(new TiXmlText("abc"));

		memset(encrypt, 0, sizeof(encrypt));
		memset(chPwd, 0, sizeof(chPwd));
		memcpy(chPwd, "123456", strlen("123456"));
		MD5Init(&md5);
		MD5Update(&md5, chPwd, (int)strlen((char *)chPwd));
		MD5Final(&md5, encrypt);

		char chAscii[32] = {};
		Hex2Ascii(encrypt, chAscii);

		TiXmlElement* xmlChild1_1 = new TiXmlElement("pwd");
		xmlChild1->LinkEndChild(xmlChild1_1);
		xmlChild1_1->LinkEndChild(new TiXmlText(chAscii));

		TiXmlElement* xmlChild2 = new TiXmlElement("user");
		xmlRoot->LinkEndChild(xmlChild2);

		TiXmlElement* xmlChild2_0 = new TiXmlElement("nick");
		xmlChild2->LinkEndChild(xmlChild2_0);
		xmlChild2_0->LinkEndChild(new TiXmlText("xyz"));

		memset(encrypt, 0, sizeof(encrypt));
		memset(chPwd, 0, sizeof(chPwd));
		memcpy(chPwd, "456789", strlen("456789"));
		MD5Init(&md5);
		MD5Update(&md5, chPwd, (int)strlen((char *)chPwd));
		MD5Final(&md5, encrypt);

		memset(chAscii, 0, sizeof(chAscii));
		Hex2Ascii(encrypt, chAscii);

		TiXmlElement* xmlChild2_1 = new TiXmlElement("pwd");
		xmlChild2->LinkEndChild(xmlChild2_1);
		xmlChild2_1->LinkEndChild(new TiXmlText(chAscii));

		TiXmlElement* xmlChild3 = new TiXmlElement("user");
		xmlRoot->LinkEndChild(xmlChild3);

		TiXmlElement* xmlChild3_0 = new TiXmlElement("nick");
		xmlChild3->LinkEndChild(xmlChild3_0);
		xmlChild3_0->LinkEndChild(new TiXmlText("wuzj"));

		memset(encrypt, 0, sizeof(encrypt));
		memset(chPwd, 0, sizeof(chPwd));
		memcpy(chPwd, "dwmx12#53435@", strlen("dwmx12#53435@"));
		MD5Init(&md5);
		MD5Update(&md5, chPwd, (int)strlen((char *)chPwd));
		MD5Final(&md5, encrypt);

		memset(chAscii, 0, sizeof(chAscii));
		Hex2Ascii(encrypt, chAscii);

		TiXmlElement* xmlChild3_1 = new TiXmlElement("pwd");
		xmlChild3->LinkEndChild(xmlChild3_1);
		xmlChild3_1->LinkEndChild(new TiXmlText(chAscii));

		xmlDocument.SaveFile("account.xml");
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CLoginDlg::OnBnClickedBtnLogin)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedBtnLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strNick = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT_USERNAME))->GetWindowText(strNick);
	if (strNick.IsEmpty())
	{
		MessageBox(_T("请输入用户名"), _T("提示"), MB_OK);
		return;
	}

	CString strPwd = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->GetWindowText(strPwd);
	if (strPwd.IsEmpty())
	{
		MessageBox(_T("请输入密码"), _T("提示"), MB_OK);
		return;
	}

	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile("account.xml"))
	{
		return;
	}

	TiXmlElement* pRoot = xmlDocument.RootElement();
	if (NULL == pRoot)
	{
		return;
	}

	TiXmlElement* pElem = NULL;
	for (pElem = pRoot->FirstChildElement("user"); pElem != NULL; pElem = pElem->NextSiblingElement())
	{
		TiXmlElement* pNodeNick = pElem->FirstChildElement("nick");
		const char* nodeNick = pNodeNick->GetText();

 		char chNick[255] = {};
		memcpy(chNick, strNick, strNick.GetLength());

		if (strcmp(chNick, nodeNick))
		{
			continue;
		}

		TiXmlElement* pNodePwd = pElem->FirstChildElement("pwd");
		const char* nodePwd = pNodePwd->GetText();
		char chNodePwd[32] = {};
		memcpy(chNodePwd, nodePwd, 32);

		char chPwd[255] = {};
		memcpy(chPwd, strPwd, strPwd.GetLength());

		MD5_CTX md5;
		unsigned char encrypt[16] = {};
		MD5Init(&md5);
		MD5Update(&md5, (unsigned char*)chPwd, (int)strlen(chPwd));
		MD5Final(&md5, encrypt);

		char chAscii[32] = {};
		Hex2Ascii(encrypt, chAscii);

		if (!memcmp(chAscii, chNodePwd, 32))
		{
			break;
		}
	}

	if (NULL == pElem)
	{
		MessageBox(_T("用户名或密码错误"), _T("提示"), MB_OK);
		return;
	}

	::SendMessage(m_hWnd, UM_LOGININ, NULL, NULL);
}
