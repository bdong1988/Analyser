
// CompositionAnalyzerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Analyzer.h"

// CCompositionAnalyzerDlg dialog
class CCompositionAnalyzerDlg : public CDialogEx
{
// Construction
public:
	CCompositionAnalyzerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPOSITIONANALYZER_DIALOG };
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
	afx_msg void OnBnClickedBtnOpenSourceFile();
	afx_msg void OnBnClickedBtnOpenDestFile();
	afx_msg void OnBnClickedBtnAnalyze();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void DisplayResult(int nElemCount, CResultQueue& rResultQueue);
	void AnalyzeDone();
	void EnableControl(BOOL bEnable);
	void SetCurrentCount(unsigned long long ullCount);

	CEdit m_editSourceFilePath;
	CEdit m_editDestFilePath;
	CEdit m_edtiMinScore;
	CListCtrl m_list3Element;
	CListCtrl m_list4Element;
	CListCtrl m_list5Element;
	CProgressCtrl m_progressAnalyze;
	CButton m_btnAnalyze;
	CButton m_btnChooseSourceFile;
	CButton m_btnChosseDestFile;

	int m_nProgressPos = 0;

	CAnalyzer m_compositionAnalyzer;

private:
	void InitListControl(CListCtrl& rListControl, vector<wstring>& rElmentList);
	
public:

	afx_msg void OnClose();
	CStatic m_staticCount;
};
