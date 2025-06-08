// ClientSocket.cpp: 구현 파일
//

#include "pch.h"
#include "HB_Client.h"
#include "ClientSocket.h"


// ClientSocket
ClientSocket client;

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}


// ClientSocket 멤버 함수
void ClientSocket::SetWnd(HWND hWnd)
{
    client.m_hWnd = hWnd;
}

SocketStatus ClientSocket::CreateSocket(CString ipAddr, int port)
{
    SocketStatus status;
    status.isSuccess = false;
    status.strIP = ipAddr;

    if (!client.Create()) {
        status.strResult = L"Create TCP Socket Failed!";
        CLogger::GetInstance().Log(status.strResult, LOG_LEVEL_ERROR);
        return status;
    }

    //소켓 연결 시도
    client.Connect(status.strIP, port);

    // 연결이 완료되었는지 확인
    int errCode = GetLastError();
    if (errCode != 0) {
        CString errorMsg = CheckConnectError(errCode);
        status.strResult = errorMsg;
        CLogger::GetInstance().Log(status.strResult, LOG_LEVEL_WARN);
        client.Close();
        return status;
    }

    //서버 인원 초과 여부
    CString checkConnectStr = CheckConnectRejection();
    if (checkConnectStr != "") {
        status.strResult = checkConnectStr;
        CLogger::GetInstance().Log(status.strResult, LOG_LEVEL_WARN);
        client.Close();
        return status;
    }

    CString logStr;
    logStr.Format(L"Connect Server. IP : %s, PORT : %d", ipAddr, port);
    CLogger::GetInstance().Log(logStr, LOG_LEVEL_INFO);

    status.strResult = _T("Connect Server");
    status.isSuccess = true;

    return status;
}

CString ClientSocket::CheckConnectError(int& errCode) {

    CString errorMsg;

    switch (errCode) {
    case WSAETIMEDOUT: // 연결 타임아웃
        errorMsg = L"Connection timed out: The attempt to connect timed out.";
        break;
    case WSAECONNREFUSED: // 서버가 꺼져 있거나 포트를 열지 않음
        errorMsg = L"Connection refused: The server is not running or the port is closed.";
        break;
    default:
        errorMsg.Format(L"Unknown connection error: Code %d", errCode);
        break;
    }
    return errorMsg;
}


CString ClientSocket::CheckConnectRejection() {
    
    CString errMsg;
    int errorCode = 0;
    int nBytes = client.Receive(&errorCode, sizeof(errorCode));

    if (nBytes == SOCKET_ERROR || nBytes < sizeof(errorCode)) {
        int err = GetLastError();
        
        errMsg.Format(L"Connect Status receive failed! Error code: %d", err);
        CLogger::GetInstance().Log(errMsg, LOG_LEVEL_ERROR);
    }

    if (errorCode == SERVER_FULL) {
        errMsg.Format(L"Server Full!");
    }

    return errMsg;
}

bool ClientSocket::SendImage(Mat& beforeImg, Mat& afterImg)
{
    // before 이미지
    std::vector<uchar> beforeBuffer;
    imencode(".png", beforeImg, beforeBuffer);

    // after 이미지
    std::vector<uchar> afterBuffer;
    imencode(".png", afterImg, afterBuffer);

    // 로그
    CString strLog = CLogger::GetInstance().GetLogFileContent();
    if (strLog.IsEmpty()) {
        CLogger::GetInstance().Log(L"The log content is empty and cannot be sent.", LOG_LEVEL_WARN);
        return false;
    }

    // 로그 byte 배열 변환
    std::string logStr(CT2A(strLog.GetString()));
    std::vector<uchar> logBuffer(logStr.begin(), logStr.end());

    // before 이미지 전송
    int beforeSize = beforeBuffer.size();
    client.Send(&beforeSize, sizeof(beforeSize));   // 크기
    client.Send(beforeBuffer.data(), beforeSize);   // 이미지 데이터

    // after 이미지 전송
    int afterSize = afterBuffer.size();
    client.Send(&afterSize, sizeof(afterSize));     // 크기
    client.Send(afterBuffer.data(), afterSize);     // 이미지 데이터

    // 로그 전송
    int logSize = logBuffer.size();
    client.Send(&logSize, sizeof(logSize)); // 크기 
    client.Send(logBuffer.data(), logSize); // 로그 데이터

    CLogger::GetInstance().Log(L"Send data to server successful");
    return true;
}

void ClientSocket::OnReceive(int nErrorCode)
{
    if (nErrorCode != 0) return;

    CString sizeLogStr;

    // 이미지 크기
    int imgSize = 0;
    client.Receive(&imgSize, sizeof(imgSize));

    sizeLogStr.Format(L"Receive Target size : %d", imgSize);
    CLogger::GetInstance().Log(sizeLogStr);

    // 이미지 데이터
    std::vector<uchar> imgBuffer(imgSize);
    client.Receive(imgBuffer.data(), imgSize);

    int selOption;

    Receive(&selOption, sizeof(selOption));

    CString strOption = CheckSelectedOption(selOption);
    sizeLogStr.Format(L"Receive Option : %s", strOption);
    CLogger::GetInstance().Log(sizeLogStr);

    Mat* pImg = new Mat(cv::imdecode(imgBuffer, IMREAD_COLOR));  // Mat 변환

    ImageData imgData;
    imgData.pImg = pImg;
    imgData.optionStr = strOption;
    imgData.optionIndex = selOption;

    CLogger::GetInstance().Log(L"Receive Server Data!");
    AfxMessageBox(L"Receive Server Data!");

    SendMessage(client.m_hWnd, WM_CLIENT_IMG_RECV, 0, (LPARAM)&imgData);

    CSocket::OnReceive(nErrorCode);
}

CString ClientSocket::CheckSelectedOption(int index)
{
    switch (index) 
    {
    case 0: return L"Match_Edge_Simple";
    case 1: return L"Match_Contours";
    case 2: return L"Match_YOLO";
    case 3: return L"Match_YOLO_Target";
    case 4: return L"Match_ORB";
    default: return L"";
    }
}

void ClientSocket::DisplayImage(CStatic& pic, cv::Mat& img)
{
    // 픽처 박스 크기
    CRect rect;
    pic.GetClientRect(&rect);

    CImage image;
    image.Create(img.cols, img.rows, 24);  // CImage 생성

    //Mat 데이터 비트를 CImage 메모리로 복사
    uchar* pDst = (uchar*)image.GetBits();
    int step = image.GetPitch();
    for (int y = 0; y < img.rows; y++) {
        memcpy(pDst + y * step, img.ptr(y), img.cols * 3);
    }

    CClientDC dc(&pic);
    pic.Invalidate();
    image.StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
}

void ClientSocket::OnClose(int nErrorCode)
{
    CLogger::GetInstance().Log(L"Server Disconnect!");

    SendMessage(m_hWnd, WM_SERVER_CLOSE, 0, (LPARAM)this);

    CSocket::OnClose(nErrorCode);
}
