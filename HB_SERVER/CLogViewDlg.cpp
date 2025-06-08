// CLogViewDlg.cpp: 구현 파일
//

#include "pch.h"
#include "HB_SERVER.h"
#include "afxdialogex.h"
#include "CLogViewDlg.h"


// CLogViewDlg 대화 상자

IMPLEMENT_DYNAMIC(CLogViewDlg, CDialogEx)

CLogViewDlg::CLogViewDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_LOGVIEW_DIALOG, pParent)
{
}

CLogViewDlg::~CLogViewDlg()
{
}

void CLogViewDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
}

BEGIN_MESSAGE_MAP(CLogViewDlg, CDialogEx)
END_MESSAGE_MAP()

void CLogViewDlg::SetLogText(const CString& log)
{
    CString resultLog;
    CString line;
    int curPos = 0;

    while ((line = log.Tokenize(_T("\n"), curPos)) != _T(""))
    {
        int firstBracketEnd = line.Find(_T("]"));
        if (firstBracketEnd != -1 && firstBracketEnd + 2 < line.GetLength())
        {
            CString trimmed = line.Mid(firstBracketEnd + 2);
            resultLog += trimmed + _T("\r\n");
        }
        else
        {
            resultLog += line + _T("\r\n");
        }
    }

    m_logText = resultLog;

    if (m_editLog.GetSafeHwnd())
    {
        m_editLog.SetWindowText(m_logText);
    }
}

BOOL CLogViewDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_editLog.SetWindowText(m_logText);

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE 반환
}