
// HttpPostMfcDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "HttpPostMfc.h"
#include "HttpPostMfcDlg.h"
#include "afxdialogex.h"
#include "TimeStamp.h"
#include "http_post.h"
#include "TimeStamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHttpPostMfcDlg 对话框



CHttpPostMfcDlg::CHttpPostMfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HTTPPOSTMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_chUrl, 0, sizeof(m_chUrl));
	memset(m_chPathPost, 0, sizeof(m_chPathPost));
	memset(m_chPathRecord, 0, sizeof(m_chPathRecord));
	memset(&m_bi, 0, sizeof(m_bi));
}

void CHttpPostMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHttpPostMfcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LAUNCH, &CHttpPostMfcDlg::OnBnClickedBtnLaunch)
	ON_BN_CLICKED(IDC_BTN_STOP, &CHttpPostMfcDlg::OnBnClickedBtnStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PATHUPLOAD, &CHttpPostMfcDlg::OnBnClickedBtnPathupload)
	ON_BN_CLICKED(IDC_BTN_PATHRECORD, &CHttpPostMfcDlg::OnBnClickedBtnPathrecord)
END_MESSAGE_MAP()


// CHttpPostMfcDlg 消息处理程序

BOOL CHttpPostMfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(_T("数据上传"));

	((CEdit*)GetDlgItem(IDC_EDIT_PERIOD))->SetWindowText(_T("1天"));
	((CButton*)GetDlgItem(IDC_BTN_STOP))->EnableWindow(false);

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER))->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHttpPostMfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHttpPostMfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHttpPostMfcDlg::OnBnClickedBtnLaunch()
{
	// TODO: 在此添加控件通知处理程序代码

	TCHAR tchUrl[256] = {};
	((CEdit*)GetDlgItem(IDC_EDIT_URL))->GetWindowText(tchUrl, 256);
	if (strlen(tchUrl) == 0)
	{
		MessageBox(_T("请输入URL"), _T("提示"), MB_OK);
		return;
	}
	memset(m_chUrl, 0, sizeof(m_chUrl));
	sprintf(m_chUrl, "%s", tchUrl);

	TCHAR tchPathPost[256] = {};
	((CEdit*)GetDlgItem(IDC_EDIT_PATHUPLOAD))->GetWindowText(tchPathPost, 256);
	if (strlen(tchPathPost) == 0)
	{
		MessageBox(_T("请选择上传文件路径"), _T("提示"), MB_OK);
		return;
	}
	memset(m_chPathPost, 0, sizeof(m_chPathPost));
	sprintf(m_chPathPost, "%s", tchPathPost);

	TCHAR tchPathRecord[256] = {};
	((CEdit*)GetDlgItem(IDC_EDIT_PATHRECORD))->GetWindowText(tchPathRecord, 256);
	if (strlen(tchPathRecord) == 0)
	{
		MessageBox(_T("请选择历史文件路径"), _T("提示"), MB_OK);
		return;
	}
	memset(m_chPathRecord, 0, sizeof(m_chPathRecord));
	sprintf(m_chPathRecord, "%s", tchPathRecord);

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER))->GetTime(&m_dateTimePost);

	SYSTEMTIME stCur;
	GetLocalTime(&stCur);

	time_t tSel = ConvertSecondsTime(m_dateTimePost);
	time_t tCur = ConvertSecondsTime(stCur);

	if (tSel <= tCur)
	{
		MessageBox(_T("上传执行时间不能早于当前时间"), _T("提示"), MB_OK);
		return;
	}

	((CButton*)GetDlgItem(IDC_BTN_STOP))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_LAUNCH))->EnableWindow(false);

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT_URL))->EnableWindow(false);

	UINT iElapse = tSel - tCur;
	SetTimer(TIMER_ID_POST, iElapse * 1000, NULL);	//最后一个填NULL，默认回调OnTimer
}


void CHttpPostMfcDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(TIMER_ID_POST);
	((CButton*)GetDlgItem(IDC_BTN_STOP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_LAUNCH))->EnableWindow(true);

	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER))->EnableWindow(true);
	((CEdit*)GetDlgItem(IDC_EDIT_URL))->EnableWindow(true);
}


void CHttpPostMfcDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);

	KillTimer(nIDEvent);

	if (nIDEvent == TIMER_ID_POST)
	{
		bool ret = OnPost(m_chPathPost, m_chUrl);

		if (ret)
		{
			//上传完成，移动文件
			SHFILEOPSTRUCT FileOp;
			ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCT));

			int nLengthSrc = strlen(m_chPathPost);
			char *pNewPathSrc = new char[nLengthSrc + 2];
			strcpy(pNewPathSrc, m_chPathPost);
			strcat(pNewPathSrc, "\\*.txt\0");

			int nLengthDst = strlen(m_chPathRecord);
			char *pNewPathDst = new char[nLengthDst + 2];
			strcpy(pNewPathDst, m_chPathRecord);

			FileOp.fFlags = FOF_NOCONFIRMATION | FOF_FILESONLY;
			FileOp.hNameMappings = NULL;
			FileOp.hwnd = NULL;
			FileOp.lpszProgressTitle = NULL;
			FileOp.pFrom = pNewPathSrc;
			FileOp.pTo = pNewPathDst;
			FileOp.wFunc = FO_MOVE;
			SHFileOperation(&FileOp);

			free(pNewPathSrc);
			pNewPathSrc = NULL;

			free(pNewPathDst);
			pNewPathDst = NULL;
		}

		//间隔下一周期触发上传
		CalcNextDateTime(m_dateTimePost);
		time_t tNext = ConvertSecondsTime(m_dateTimePost);

		SYSTEMTIME st;
		GetLocalTime(&st);
		time_t tCur = ConvertSecondsTime(st);

		UINT iElapse = tNext - tCur;
		SetTimer(TIMER_ID_POST, iElapse * 1000, NULL);
	}
}


void CHttpPostMfcDlg::OnBnClickedBtnPathupload()
{
	// TODO: 在此添加控件通知处理程序代码

	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_PATHUPLOAD))->SetWindowText(tchPath);
}


void CHttpPostMfcDlg::OnBnClickedBtnPathrecord()
{
	// TODO: 在此添加控件通知处理程序代码

	LPITEMIDLIST lp = SHBrowseForFolder(&m_bi);
	TCHAR tchPath[512] = {};
	SHGetPathFromIDList(lp, tchPath);

	((CEdit*)GetDlgItem(IDC_EDIT_PATHRECORD))->SetWindowText(tchPath);
}
