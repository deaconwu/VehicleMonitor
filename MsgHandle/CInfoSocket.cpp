#include "pch.h"
#include "CInfoSocket.h"
#include "UserMessage.h"

#pragma comment(lib,"ws2_32.lib")

SOCKET CInfoSocket::OnConnect()
{
	if (m_pSocket != INVALID_SOCKET)
		OnClose();

	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return INVALID_SOCKET;
	}

	//创建套接字
	m_pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_pSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return INVALID_SOCKET;
	}

	if (connect(m_pSocket, (sockaddr *)&m_serAddr, sizeof(m_serAddr)) == SOCKET_ERROR)
	{
		DWORD dwErr = GetLastError();
		closesocket(m_pSocket);
		WSACleanup();
		m_pSocket = INVALID_SOCKET;
		return INVALID_SOCKET;
	}

	return m_pSocket;
}

SOCKET CInfoSocket::OnReConnect()
{
	OnClose();

	SOCKET sock = OnConnect();

	return sock;
}

VOID CInfoSocket::OnReset(UCHAR ip1, UCHAR ip2, UCHAR ip3, UCHAR ip4, CString portStr)
{
	m_serAddr.sin_family = AF_INET;
	m_serAddr.sin_addr.S_un.S_un_b.s_b1 = ip1;
	m_serAddr.sin_addr.S_un.S_un_b.s_b2 = ip2;
	m_serAddr.sin_addr.S_un.S_un_b.s_b3 = ip3;
	m_serAddr.sin_addr.S_un.S_un_b.s_b4 = ip4;

	int iPort = _ttoi(portStr);
	m_serAddr.sin_port = htons(iPort);
}

VOID CInfoSocket::OnClose()
{
	closesocket(m_pSocket);
	WSACleanup();

	m_pSocket = INVALID_SOCKET;
}

INT CInfoSocket::OnAsync()
{
	INT ret = WSAAsyncSelect(m_pSocket, m_hWnd, NETWORK_EVENT, FD_READ | FD_CLOSE);

	return ret;
}

INT CInfoSocket::OnReceive(char recvData[], int recvSize)
{
	if (INVALID_SOCKET == m_pSocket)
	{
		return -1;
	}

// 	INT optVal = 0;
// 	INT optLen = sizeof(optVal);
// 	getsockopt(m_pSocket, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, &optLen);

	INT iSize = recv(m_pSocket, recvData, recvSize, 0);
	DWORD dwErr = GetLastError();

	return iSize;
}

INT CInfoSocket::OnSend(char sendData[], int sendSize)
{
	if (INVALID_SOCKET == m_pSocket)
	{
		return -1;
	}

	INT iSize = send(m_pSocket, sendData, sendSize, 0);
	DWORD dwErr = GetLastError();

	return iSize;
}