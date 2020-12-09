#pragma once

class CInfoSocket
{
public:
	CInfoSocket(){}

	CInfoSocket(UCHAR ip1, UCHAR ip2, UCHAR ip3, UCHAR ip4, CString portStr, HWND hWnd)
	{
		memset(&m_serAddr, 0, sizeof(m_serAddr));
		m_pSocket = INVALID_SOCKET;
		m_hWnd = NULL;

		m_serAddr.sin_family = AF_INET;
		m_serAddr.sin_addr.S_un.S_un_b.s_b1 = ip1;
		m_serAddr.sin_addr.S_un.S_un_b.s_b2 = ip2;
		m_serAddr.sin_addr.S_un.S_un_b.s_b3 = ip3;
		m_serAddr.sin_addr.S_un.S_un_b.s_b4 = ip4;

		int iPort = _ttoi(portStr);
		m_serAddr.sin_port = htons(iPort);

		m_hWnd = hWnd;
	}

	~CInfoSocket(){}

	SOCKET OnConnect();

	SOCKET OnReConnect();

	VOID OnReset(UCHAR ip1, UCHAR ip2, UCHAR ip3, UCHAR ip4, CString portStr);

	VOID OnClose();

	INT OnAsync();

	INT OnReceive(char recvData[], int recvSize);

	INT OnSend(char sendData[], int sendSize);

private:
	SOCKET m_pSocket;

	sockaddr_in m_serAddr;

	HWND m_hWnd;
};