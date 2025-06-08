// DlgTab1.cpp: 구현 파일
//

#include "pch.h"
#include "HB_SERVER.h"
#include "afxdialogex.h"
#include "DlgTab1.h"
#include "MySQLConn.h"

//#include "opencv2/imgcodecs.hpp"
using namespace cv;

// CDlgTab1 대화 상자

IMPLEMENT_DYNAMIC(CDlgTab1, CDialogEx)

CDlgTab1::CDlgTab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{

}

CDlgTab1::~CDlgTab1()
{
}

void CDlgTab1::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TEMPLATE_PIC, m_templatePic);
    DDX_Control(pDX, IDC_OPTION_COMBO, m_optionCombo);
    DDX_Control(pDX, IDC_BEFOR_PIC, m_beforePic);
    DDX_Control(pDX, IDC_AFTER_PIC, m_afterPic);
    DDX_Control(pDX, IDC_LIST1, m_list_client);
}


BEGIN_MESSAGE_MAP(CDlgTab1, CDialogEx)
	ON_BN_CLICKED(IDC_CONNECT_BTN, &CDlgTab1::OnBnClickedConnectBtn)
    ON_MESSAGE(WM_ACCEPT_SOCKET, &CDlgTab1::OnAcceptSocket)
    ON_MESSAGE(WM_CLIENT_SEND, &CDlgTab1::OnReceiveSocket)
    ON_MESSAGE(WM_CLIENT_CLOSE, &CDlgTab1::OnClientDisconnect)
    ON_BN_CLICKED(IDC_SELECT_BTN, &CDlgTab1::OnBnClickedSelectBtn)
    ON_BN_CLICKED(IDC_SEND_BTN, &CDlgTab1::OnBnClickedSendBtn)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_SAVE_BTN, &CDlgTab1::OnBnClickedSaveBtn)
END_MESSAGE_MAP()


// CDlgTab1 메시지 처리기
BOOL CDlgTab1::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    m_optionCombo.InsertString(0, L"Match_Edge_Simple");
    m_optionCombo.InsertString(1, L"Match_Contours");
    m_optionCombo.InsertString(2, L"Match_YOLO"); 
    m_optionCombo.InsertString(3, L"Match_ORB");
    m_optionCombo.SetCurSel(0);

    SetDlgItemInt(IDC_PORT_EDIT, 123);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgTab1::OnBnClickedConnectBtn()
{
	int port = GetDlgItemInt(IDC_PORT_EDIT);
    if (!port) {
        AfxMessageBox(L"Enter the port!");
        return;
    }

    // 서버 소켓 설정
    m_server.SetWnd(this->m_hWnd);
    ServerSocketStatus status = m_server.CreateServerSocket(port);

    if (status.isSuccess) {
         
        //컨트롤 스타일 변경 함수 추가 필요
        //Connect 버튼
        CButton* pButton = (CButton*)GetDlgItem(IDC_CONNECT_BTN);
        pButton->EnableWindow(FALSE);
        //Port 컨트롤
        CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PORT_EDIT);
        pEdit->SetReadOnly(TRUE);

        SetDlgItemText(IDC_IP_EDIT, status.resultStr);

        CString msg;
        msg.Format(L"Create Server Socket\nIP : %s  Port : %d", status.resultStr, port);
        MessageBox(msg, L"Create Success");
    }
    else {
        AfxMessageBox(status.resultStr);
    }
}

LRESULT CDlgTab1::OnAcceptSocket(WPARAM wParam, LPARAM lParam)
{
    ClientSocket* newClient = (ClientSocket*)lParam;

    CString str;
    str.Format(_T("Client (%d)"), (int)(newClient));
    m_list_client.InsertString(-1, str);
    
    return 0;
}

LRESULT CDlgTab1::OnClientDisconnect(WPARAM wParam, LPARAM lParam)
{
    ClientSocket* disconnectedClient = (ClientSocket*)lParam;

    CString str;
    UINT idx = 0;
    POSITION pos = m_server.m_clientList.Find(disconnectedClient);

    if (pos != NULL)
    {
        //m_list_client에서 해당 Client 삭제
        str.Format(_T("Client (%d)"), (int)disconnectedClient);
        idx = m_list_client.FindStringExact(-1, str);
        m_list_client.DeleteString(idx);
    }

    m_server.RemoveClient(disconnectedClient);

    return 0;
}

LRESULT CDlgTab1::OnReceiveSocket(WPARAM wParam, LPARAM lParam)
{
    ClientRecvData* pRecvData = (ClientRecvData*)lParam;

    Mat* pBeforeImg = pRecvData->pBeforeImg;
    Mat* pAfterImg = pRecvData->pAfterImg;

    m_beforeMat = pBeforeImg->clone();
    m_afterMat = pAfterImg->clone();

    AfxMessageBox(L"Receive Client Data!");

    m_client.DisplayImage(m_beforePic, *pBeforeImg);
    m_client.DisplayImage(m_afterPic, *pAfterImg);

    delete pBeforeImg;
    delete pAfterImg;
    return LRESULT();
}


BOOL CDlgTab1::PreTranslateMessage(MSG* pMsg)
{
    //엔터 종료 막기
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)
        {
            // ESC 키 이벤트에 대한 처리 추가
            return TRUE;
        }
        else if (pMsg->wParam == VK_RETURN)
        {
            // Enter 키 이벤트에 대한 처리 추가
            return TRUE;
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgTab1::OnBnClickedSelectBtn()
{
    CFileDialog fileDlg(TRUE, _T("*.jpg"), nullptr, OFN_FILEMUSTEXIST, _T("Image Files (*.jpg;*.png)|*.jpg;*.png||"));

    if (fileDlg.DoModal() == IDOK) {
        CString path = fileDlg.GetPathName();

        CT2CA pszString(path);
        std::string strPath(pszString);

        // 이미지를 읽기
        m_templateMat = imread(strPath, IMREAD_COLOR);

        // 공통 클래스 사용하여 이미지 표시
        m_client.DisplayImage(m_templatePic, m_templateMat);
    }
}


void CDlgTab1::OnBnClickedSendBtn()
{
    int selectedIndex = m_optionCombo.GetCurSel();

    if (m_server.m_clientList.GetCount() < 1) {
        AfxMessageBox(L"There is no client!");
        return;
    }

    if (m_templateMat.empty()) {
        AfxMessageBox(L"No images to send!");
        return;
    }

    POSITION pos = m_server.m_clientList.GetHeadPosition();
    while (pos != NULL)
    {
        ClientSocket* client = (ClientSocket*)m_server.m_clientList.GetNext(pos);
        client->SendImage(m_templateMat, selectedIndex);
        AfxMessageBox(L"Send Data Successful!");
    }
}

void CDlgTab1::OnDestroy()
{
    CDialogEx::OnDestroy();

    POSITION pos = m_server.m_clientList.GetHeadPosition();
    while (pos != NULL)
    {
        ClientSocket* client = (ClientSocket*)m_server.m_clientList.GetNext(pos);
        if (client) {
            client->Close();
            delete client;
        }
    }
    m_server.m_clientList.RemoveAll();

}

void CDlgTab1::OnBnClickedSaveBtn()
{
    CMySQLConn db;
    HBSData data;

    CString errorMsg;
    if (m_templateMat.empty()) errorMsg += L"Target Image\n";
    if (m_beforeMat.empty()) errorMsg += L"Before Image\n";
    if (m_afterMat.empty()) errorMsg += L"After Image\n";

    if (!errorMsg.IsEmpty()) {
        AfxMessageBox(L"The following image is blank\n" + errorMsg);
        return;
    }

    data.beforeImage = m_beforeMat.clone();
    data.targetImage = m_templateMat.clone();
    data.afterImage = m_afterMat.clone();

    // 데이터 삽입
    if (db.InsertData(data)) {
        CLogger::GetInstance().Log(L"DB Save Success!", LOG_LEVEL_INFO);
        AfxMessageBox(L"DB Save Success!");
        m_beforeMat.release();
        m_afterMat.release();
    }
    else {
        CLogger::GetInstance().Log(L"Failed to save data!", LOG_LEVEL_ERROR);
        AfxMessageBox(L"Failed to save data!");
    }
}

