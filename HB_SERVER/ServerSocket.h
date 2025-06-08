#pragma once
#include "ClientSocket.h"

#define WM_ACCEPT_SOCKET WM_USER+1
#define MAX_CLIENT 1
#define SERVER_FULL 1001

struct ServerSocketStatus
{
	bool isSuccess;   // 성공 여부
	CString resultStr;
};

class ServerSocket : public CSocket
{
private:
public:
	ServerSocket();
	virtual ~ServerSocket();
	HWND m_hWnd;
	CPtrList m_clientList;
	void SetWnd(HWND hWnd);
	virtual void OnAccept(int nErrorCode);
	ServerSocketStatus CreateServerSocket(int port);
	void RemoveClient(ClientSocket* client);
};

