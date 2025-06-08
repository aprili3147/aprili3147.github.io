// ServerSocket.cpp: 구현 파일
//

#include "pch.h"
#include "HB_SERVER.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

//IP 관련 헤더 파일
#include <winsock2.h>
#include <ws2tcpip.h>


// ServerSocket

ServerSocket::ServerSocket()
{
}

ServerSocket::~ServerSocket()
{
}


// ServerSocket 멤버 함수
void ServerSocket::SetWnd(HWND hWnd)
{
    m_hWnd = hWnd;
}

void ServerSocket::OnAccept(int nErrorCode)
{
    if (nErrorCode == 0)
    {
        ClientSocket* newClient = new ClientSocket;

        //클라이언트 연결
        if (Accept(*newClient))
        {
            int errorCode = -1;

            //클라이언트 소켓 최대
            if (m_clientList.GetCount() >= MAX_CLIENT)
            {
                errorCode = SERVER_FULL;
                newClient->Send(&errorCode, sizeof(errorCode));

                newClient->Close();
                
                CString logMsg;
                logMsg.Format(L"Client %d attempted to connect, but the server is full. Connection closed.", newClient);
                CLogger::GetInstance().Log(logMsg, LOG_LEVEL_WARN);

                delete newClient;
                return;
            }

            newClient->Send(&errorCode, sizeof(errorCode));

            m_clientList.AddTail(newClient);
            newClient->SetWnd(m_hWnd);
            SendMessage(m_hWnd, WM_ACCEPT_SOCKET, 0, (LPARAM)newClient);

            CString logStr;
            logStr.Format(L"Connect Client : %d", newClient);
            CLogger::GetInstance().Log(logStr, LOG_LEVEL_INFO);
        }
        else
        {
            delete newClient;
        }
    }

	CSocket::OnAccept(nErrorCode);
}

ServerSocketStatus ServerSocket::CreateServerSocket(int port) {
    ServerSocketStatus status;
    status.isSuccess = false;

    // 서버의 IP 주소 가져오기
    CString strIP = L"Unknown IP";
    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        struct addrinfo hints = { 0 }, * res = nullptr;
        hints.ai_family = AF_INET;  // IPv4 주소만 가져오기

        if (getaddrinfo(hostname, nullptr, &hints, &res) == 0) {
            struct sockaddr_in* addr = (struct sockaddr_in*)res->ai_addr;
            WCHAR ipStr[INET_ADDRSTRLEN];

            InetNtop(AF_INET, &addr->sin_addr, ipStr, INET_ADDRSTRLEN);
            strIP = ipStr;

            freeaddrinfo(res);
        }
    }

    if (!Create(port)) {
        CLogger::GetInstance().Log(L"Create Server Socket Failed!", LOG_LEVEL_ERROR);

        status.resultStr = L"Create Server Socket Failed!";
        return status;
    }
    if (!Listen()) {
        CLogger::GetInstance().Log(L"Listening Server Failed!", LOG_LEVEL_ERROR);

        status.resultStr = L"Listening Server Failed!";
        return status;
    }

    CString logStr;
    logStr.Format(L"Listening Server. IP : %s, PORT : %d", strIP, port);
    CLogger::GetInstance().Log(logStr, LOG_LEVEL_INFO);


    status.isSuccess = true;
    status.resultStr = strIP;
    return status;
}

void ServerSocket::RemoveClient(ClientSocket* client)
{
    POSITION pos = m_clientList.Find(client);
    if (pos != NULL) {
        ClientSocket* pRemove = (ClientSocket*)m_clientList.GetAt(pos);
        delete pRemove;
        m_clientList.RemoveAt(pos);

        CString logStr;
        logStr.Format(L"Disconnect Client : %d", pos);
        CLogger::GetInstance().Log(logStr, LOG_LEVEL_INFO);
    }
}


