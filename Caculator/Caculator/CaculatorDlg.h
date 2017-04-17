
// CaculatorDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Analyzer.h"
#include "afxcmn.h"

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
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnAnalyze();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CString GetTime();
private:
	CAnalyzer m_analyzer;
	CEdit m_editMinScore;
	CEdit m_editFilePath;
	CProgressCtrl m_progress;	
	CStatic m_staticTimeStart;
	CStatic m_staticTimeEnd;
	CStatic m_staticDataCount;
	CStatic m_staticHighScore;
	CStatic m_staticLowScore;
	int	m_nProgress = 0;
	bool m_bAnalyzeInfoSet = false;
};
