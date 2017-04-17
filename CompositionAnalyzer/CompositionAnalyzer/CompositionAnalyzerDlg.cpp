
// CompositionAnalyzerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CompositionAnalyzer.h"
#include "CompositionAnalyzerDlg.h"
#include "afxdialogex.h"
#include "ExcelReader.h"

#define TIMER_PORGRESS 1
#define TIMER_PROGRESS_ELAPS 200

#define SCORE_LIST_HEADER L"ตรทึ"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const LPWSTR FILEFILTER = _T("Excel(*.xlsx)|*.xlsx|Excel(*.xls)|*.xls|All Files(*.*)|*.*||");

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCompositionAnalyzerDlg dialog



CCompositionAnalyzerDlg::CCompositionAnalyzerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMPOSITIONANALYZER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCompositionAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SOURCE_FILE, m_editSourceFilePath);
	DDX_Control(pDX, IDC_EDIT_DEST_FILE, m_editDestFilePath);
	DDX_Control(pDX, IDC_EDIT2, m_edtiMinScore);
	DDX_Control(pDX, IDC_LIST_3ELEMENT, m_list3Element);
	DDX_Control(pDX, IDC_LIST_4ELEMENT, m_list4Element);
	DDX_Control(pDX, IDC_LIST_5ELEMENT, m_list5Element);
}

BEGIN_MESSAGE_MAP(CCompositionAnalyzerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_SOURCE_FILE, &CCompositionAnalyzerDlg::OnBnClickedBtnOpenSourceFile)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEST_FILE, &CCompositionAnalyzerDlg::OnBnClickedBtnOpenDestFile)
	ON_BN_CLICKED(IDC_BTN_ANALYZE, &CCompositionAnalyzerDlg::OnBnClickedBtnAnalyze)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCompositionAnalyzerDlg message handlers

BOOL CCompositionAnalyzerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCompositionAnalyzerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCompositionAnalyzerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCompositionAnalyzerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCompositionAnalyzerDlg::OnBnClickedBtnOpenSourceFile()
{
	CFileDialog fileDlg(TRUE, _T(".xlsx"), nullptr, 0, FILEFILTER, this);
	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		m_editSourceFilePath.SetWindowText(strFilePath);
	}
}


void CCompositionAnalyzerDlg::OnBnClickedBtnOpenDestFile()
{
	CFileDialog fileDlg(TRUE, _T(".xlsx"), nullptr, 0, FILEFILTER, this);
	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		m_editDestFilePath.SetWindowText(strFilePath);
	}
}


void CCompositionAnalyzerDlg::OnBnClickedBtnAnalyze()
{
	CString strFilePath;
	m_editSourceFilePath.GetWindowText(strFilePath);
	if (strFilePath.IsEmpty())
	{
		return;
	}

	CExcelReader reader;
	HRESULT hr = reader.ReadFile(strFilePath.GetBuffer());
	m_compositionAnalyzer.InitilizeData(reader.GetRawContents());

	InitListControl(m_list3Element, m_compositionAnalyzer.GetElementNameList());
	InitListControl(m_list4Element, m_compositionAnalyzer.GetElementNameList());
	InitListControl(m_list5Element, m_compositionAnalyzer.GetElementNameList());

	m_compositionAnalyzer.StartAnalyze();

	SetTimer(TIMER_PORGRESS, TIMER_PROGRESS_ELAPS, nullptr);
}


void CCompositionAnalyzerDlg::OnTimer(UINT_PTR nIDEvent)
{

	CDialogEx::OnTimer(nIDEvent);
}

void CCompositionAnalyzerDlg::InitListControl(CListCtrl & rListControl, vector<wstring>& rElmentList)
{
	int nElementCount = rElmentList.size();

	CRect rect;
	rListControl.GetWindowRect(rect);

	int nColumnWidth = rect.Width() / (nElementCount + 1);
	int nFirstColumnWidth = rect.Width() - nColumnWidth*nElementCount;

	LV_COLUMN lvcolumn = { 0 };
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = SCORE_LIST_HEADER;
	lvcolumn.iSubItem = 0;
	lvcolumn.iOrder = 0;
	lvcolumn.cx = nFirstColumnWidth;

	rListControl.InsertColumn(0, &lvcolumn);
	for (int i = 0; i < nElementCount; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (LPWSTR)rElmentList[i].c_str();
		lvcolumn.iSubItem = i+1;
		lvcolumn.iOrder = i+1;
		lvcolumn.cx = nColumnWidth;

		rListControl.InsertColumn(i+1, &lvcolumn);
	}
}
