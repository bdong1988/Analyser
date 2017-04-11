
// CaculatorDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CCaculatorDlg dialog
class CCaculatorDlg : public CDialogEx
{
// Construction
public:
	CCaculatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CACULATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpenFile();
	afx_msg void OnBnClickedBtnCaculate();
	CEdit m_editFilePath;
};
