// DlgTab2.cpp: 구현 파일
//

#include "pch.h"
#include "HB_SERVER.h"
#include "afxdialogex.h"
#include "DlgTab2.h"
#include "MySQLConn.h"


// CDlgTab2 대화 상자

IMPLEMENT_DYNAMIC(CDlgTab2, CDialogEx)

CDlgTab2::CDlgTab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DB_DIALOG, pParent)
{

}

CDlgTab2::~CDlgTab2()
{
}

void CDlgTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DB_LIST, m_db_List);
	DDX_Control(pDX, IDC_BEFORE_PIC, m_beforePic);
	DDX_Control(pDX, IDC_AFTER_PIC, m_afterPic);
	DDX_Control(pDX, IDC_LOG_EDIT, m_logText);
}


BEGIN_MESSAGE_MAP(CDlgTab2, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DB_LIST, &CDlgTab2::OnLvnItemchangedDBList)
	ON_MESSAGE(WM_UPDATE_DB, &CDlgTab2::OnUpdateDb)
	ON_BN_CLICKED(IDC_DELETE_BTN, &CDlgTab2::OnBnClickedDeleteBtn)
END_MESSAGE_MAP()


// CDlgTab2 메시지 처리기

BOOL CDlgTab2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	db.ConnectMySQL();
	db.SetWnd(this->m_hWnd);

	// 컬럼
	m_db_List.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 35);
	m_db_List.InsertColumn(1, _T("Date"), LVCFMT_LEFT, 200);

	// 스타일
	m_db_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_db_List.ModifyStyle(0, LVS_REPORT);
	m_db_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	DrawList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDlgTab2::PreTranslateMessage(MSG* pMsg)
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

void CDlgTab2::DrawList() 
{
	//리스트 컨트롤
	// 초기화
	m_db_List.DeleteAllItems();

	std::vector<HBSData> dataList = db.FindAll();

	// ID 내림차순 정렬
	std::sort(dataList.begin(), dataList.end(), [](const HBSData& a, const HBSData& b) {
		return a.id > b.id;
	});

	for (int i = 0; i < dataList.size(); ++i) {
		const HBSData& data = dataList[i];

		// 첫 번째 열 (ID)
		CString idStr;
		idStr.Format(_T("%d"), data.id);
		int index = m_db_List.InsertItem(i, idStr);

		// 두 번째 열 (생성일)
		m_db_List.SetItemText(index, 1, data.createdAt);
	}
}


void CDlgTab2::OnLvnItemchangedDBList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int selectedRow = pNMLV->iItem;
	if (selectedRow < 0) return;

	CString strID = m_db_List.GetItemText(selectedRow, 0);
	int selectedID = _ttoi(strID);

	HBSData data = db.FindOne(selectedID);

	DisplayData(data);
}

void CDlgTab2::DisplayData(HBSData& data)
{
	DisplayImage(m_beforePic, data.beforeImage);
	DisplayImage(m_afterPic, data.afterImage);
	
	DisplayLog(data.log);
}

void CDlgTab2::DisplayImage(CStatic& pic, cv::Mat& img)
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

void CDlgTab2::DisplayLog(const CString& logText)
{
	CString modifiedLog = logText;
	int pos = 0;

	while ((pos = modifiedLog.Find('\n', pos)) != -1) {
		modifiedLog.Insert(pos, _T("\r"));
		pos += 2;
	}

	SetDlgItemText(IDC_LOG_EDIT, modifiedLog);
}

void CDlgTab2::OnBnClickedDeleteBtn()
{
	int index = m_db_List.GetNextItem(-1, LVNI_SELECTED);

	if (index == -1) {
		AfxMessageBox(L"Select the items you want to delete.");
		return;
	}

	CString strID = m_db_List.GetItemText(index, 0);
	int id = _ttoi(strID);
	db.DeleteData(id);
}

LRESULT CDlgTab2::OnUpdateDb(WPARAM wParam, LPARAM lParam)
{
	DrawList();
	return 0;
}