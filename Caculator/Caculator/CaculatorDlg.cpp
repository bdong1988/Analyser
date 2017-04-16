
// CaculatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Caculator.h"
#include "CaculatorDlg.h"
#include "afxdialogex.h"

#include "ExcelReader.h"

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


// CCaculatorDlg dialog



CCaculatorDlg::CCaculatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CACULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	DDX_Control(pDX, IDC_EDIT_MINSCORE, m_editMinScore);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_STATIC_TIME_START, m_staticTimeStart);
	DDX_Control(pDX, IDC_STATIC_TIME_END, m_staticTimeEnd);
	DDX_Control(pDX, IDC_DATA_COUNT, m_staticDataCount);
	DDX_Control(pDX, IDC_HIGH_SCORE, m_staticHighScore);
	DDX_Control(pDX, IDC_LOW_SCORE, m_staticLowScore);
}

BEGIN_MESSAGE_MAP(CCaculatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CCaculatorDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CCaculatorDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_ANALYZE, &CCaculatorDlg::OnBnClickedBtnAnalyze)
END_MESSAGE_MAP()


// CCaculatorDlg message handlers

BOOL CCaculatorDlg::OnInitDialog()
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
	m_editMinScore.SetWindowText(L"83");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCaculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCaculatorDlg::OnPaint()
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
HCURSOR CCaculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCaculatorDlg::OnBnClickedBtnOpenFile()
{
	CFileDialog fileDlg(TRUE, _T(".xlsx"), nullptr, 0, FILEFILTER, this);
	CString strFilePath;
	
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		m_editFilePath.SetWindowText(strFilePath);

	}
}

void CCaculatorDlg::OnBnClickedBtnOpen()
{
	// TODO: Add your control notification handler code here
	CString strFilePath;
	m_editFilePath.GetWindowText(strFilePath);
	if (strFilePath.IsEmpty())
	{
		return;
	}

	CExcelReader reader;
	HRESULT hr = reader.ReadFile(strFilePath.GetBuffer());
	m_analyzer.InitilizeData(reader.GetRawContents());
}


void CCaculatorDlg::OnBnClickedBtnAnalyze()
{
	CString strMinScore;
	m_editMinScore.GetWindowText(strMinScore);
	m_analyzer.SetMinScore(_wtol(strMinScore));
	CString strStartTime(L"Start Time: ");
	strStartTime += GetTime();
	m_staticTimeStart.SetWindowText(strStartTime);
	m_analyzer.StartAnalyze();
	CString strEndTime(L"End Time: ");
	strEndTime += GetTime();
	m_staticTimeEnd.SetWindowText(strEndTime);

	CString strCount;
	strCount.Format(L"DataCount: %llu", m_analyzer.GetDataCount());
	m_staticDataCount.SetWindowText(strCount);

	CString strHighScore;
	strHighScore.Format(L"Highest Score: %lf", m_analyzer.GetHighScore());
	m_staticHighScore.SetWindowText(strHighScore);
}

CString CCaculatorDlg::GetTime()
{
	CString strTime;
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);

	return strTime;
}
