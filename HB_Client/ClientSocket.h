#pragma once

using namespace cv;

// ClientSocket 명령 대상
#define WM_CLIENT_IMG_RECV		 (WM_USER + 2)
#define WM_SERVER_CLOSE			 (WM_USER + 3)
#define WM_CLIENT_CONNECT_RESULT (WM_USER + 101)

#define SERVER_FULL 1001

struct ImageData 
{
	Mat* pImg;         // 이미지
	CString optionStr; // 옵션
	int optionIndex;
};

struct SocketStatus
{
	bool isSuccess;   // 성공 여부
	CString strResult;
	CString strIP;
};

class ClientSocket : public CSocket
{
private:
	CString CheckConnectError(int& errCode);
	CString CheckConnectRejection();
	CString CheckSelectedOption(int index);

	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
public:
	ClientSocket();
	virtual ~ClientSocket();
	
	HWND m_hWnd;

	void SetWnd(HWND hWnd); //핸들러 세팅
	SocketStatus CreateSocket(CString ipAddr, int port);

	bool SendImage(Mat& beforeImg, Mat& afterImg);
	void DisplayImage(CStatic& pic, Mat& img);
};


