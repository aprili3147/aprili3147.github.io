#pragma once
#include "afxdialogex.h"


class CLogViewDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CLogViewDlg)

public:
    CLogViewDlg(CWnd* pParent = nullptr);
    virtual ~CLogViewDlg();

    void SetLogText(const CString& log);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_LOGVIEW_DIALOG };
#endif

protected:
    BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

private:
    CString m_logText;
    CEdit m_editLog;
};
