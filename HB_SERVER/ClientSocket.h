#pragma once
using namespace cv;

// ClientSocket 명령 대상

#define WM_CLIENT_SEND WM_USER+2
#define WM_CLIENT_CLOSE WM_USER+3

struct ClientRecvData
{
	Mat* pBeforeImg;
	Mat* pAfterImg;
	CString logStr;
};

class ClientSocket : public CSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();
	void SetWnd(HWND hWnd);
	void SendImage(Mat& img, int selOption);
	HWND m_hWnd;
	virtual void OnReceive(int nErrorCode);
	void DisplayImage(CStatic& pic, Mat& img);
	virtual void OnClose(int nErrorCode);
};


