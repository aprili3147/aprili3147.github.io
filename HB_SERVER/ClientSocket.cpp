// ClientSocket.cpp: 구현 파일
//

#include "pch.h"
#include "HB_SERVER.h"
#include "ClientSocket.h"

using namespace cv;

// ClientSocket

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}


// ClientSocket 멤버 함수
void ClientSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

void ClientSocket::SendImage(Mat& img, int selOption) {

	// 이미지 압축
	std::vector<uchar> buffer;
	imencode(".png", img, buffer);

	// 옵션 byte 배열 변환
	//std::string optionStr(CT2A(strOption.GetString()));
	//std::vector<uchar> optionBuffer(optionStr.begin(), optionStr.end());

	// 이미지 전송
	int imgSize = buffer.size();
	Send(&imgSize, sizeof(imgSize));   // 크기
	Send(buffer.data(), imgSize);       // 이미지 데이터

	// 옵션 전송
	//int optionSize = optionBuffer.size();
	//Send(&optionSize, sizeof(optionSize)); // 크기 
	//Send(optionBuffer.data(), optionSize); // 옵션 데이터

	Send(&selOption, sizeof(selOption));

	CLogger::GetInstance().Log(L"Send Data Successful!");
}

void ClientSocket::OnReceive(int nErrorCode)
{
	if (nErrorCode != 0) return;

	CString sizeLogStr;

	// before 이미지 크기
	int beforeSize = 0;
	Receive(&beforeSize, sizeof(beforeSize));

	sizeLogStr.Format(L"Receive Before size : %d", beforeSize);
	CLogger::GetInstance().Log(sizeLogStr);

	// before 이미지 데이터
	std::vector<uchar> beforeBuffer(beforeSize);
	Receive(beforeBuffer.data(), beforeSize);

	// after 이미지 크기
	int afterSize = 0;
	Receive(&afterSize, sizeof(afterSize));

	sizeLogStr.Format(L"Receive After size : %d", afterSize);
	CLogger::GetInstance().Log(sizeLogStr);

	// after 이미지 데이터
	std::vector<uchar> afterBuffer(afterSize);
	Receive(afterBuffer.data(), afterSize);

	// 로그 크기
	int logSize = 0;
	Receive(&logSize, sizeof(logSize));

	sizeLogStr.Format(L"Receive Log size : %d", logSize);
	CLogger::GetInstance().Log(sizeLogStr);

	// 로그 데이터
	std::vector<uchar> logBuffer(logSize);
	Receive(logBuffer.data(), logSize);
	std::string logStr(logBuffer.begin(), logBuffer.end());
	CString strLog(logStr.c_str());

	Mat* pBeforeImg = new Mat(cv::imdecode(beforeBuffer, IMREAD_COLOR));
	Mat* pAfterImg = new Mat(cv::imdecode(afterBuffer, IMREAD_COLOR));

	ClientRecvData recvData;
	recvData.pBeforeImg = pBeforeImg;
	recvData.pAfterImg = pAfterImg;
	recvData.logStr = strLog;

	CLogger::GetInstance().Log(L"Receive Client Data!");
	
	CLogger::GetInstance().Log(L"\n***** Client Log *****", LOG_LEVEL_CLIENT);
	CLogger::GetInstance().Log(recvData.logStr, LOG_LEVEL_CLIENT);
	CLogger::GetInstance().Log(L"**********************\n", LOG_LEVEL_CLIENT);

	SendMessage(m_hWnd, WM_CLIENT_SEND, 0, (LPARAM)&recvData);

	CSocket::OnReceive(nErrorCode);
}

void ClientSocket::DisplayImage(CStatic& pic, Mat& img)
{
	// 픽처 박스 크기
	CRect rect;
	pic.GetClientRect(&rect);

	CImage image;
	image.Create(img.cols, img.rows, 24);

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
	SendMessage(m_hWnd, WM_CLIENT_CLOSE, 0, (LPARAM)this);
	CSocket::OnClose(nErrorCode);
}
