#pragma once
#include <mysql.h>
#include "opencv2/opencv.hpp"

using namespace cv;

// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
public:
    CMFCApplication1Dlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    MYSQL* ConnectToDatabase();
    void DisplayImage();  // ✅ 이미지 표시 함수 추가
    std::string selectedFilePath;  // 파일 경로를 저장할 멤버 변수
    void LoadImageFromDB(int id); // ✅ DB에서 이미지 불러오는 함수 추가

public:
    CListCtrl m_listHBS;
    afx_msg void OnBnClickedButtonSelect();
    void FetchAndDisplayHBSData();
    afx_msg void OnFileFileopen();
    afx_msg void OnItemClickListHBS(NMHDR* pNMHDR, LRESULT* pResult); // ✅ 리스트 클릭 이벤트 추가

private:
    HICON m_hIcon;          // 아이콘 핸들
    CStatic m_pictureCtrl;  // ✅ Picture Control 변수
    Mat InpImg, OutImg, TempImg;
    unsigned char** GrayImg;
    unsigned char** U, ** V;

public:
    afx_msg void OnBnClickedButton1();
    bool UploadImageToDB(const cv::Mat& img);
};
