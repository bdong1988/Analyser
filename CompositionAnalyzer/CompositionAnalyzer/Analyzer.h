#pragma once
#include "Group.h"
#include "Result.h"
#include "CSVProcessor.h"

#define ELEMENTCOUNT 8

using groupList = vector<CGroup>;

class CAnalyzer
{

public:
	CAnalyzer();
	~CAnalyzer();

	void Clear();

	void InitilizeData(const contentArray& rawDataArray);

	void ParseRange(const wstring& strRange, unsigned long& ulLow, unsigned long& ulHigh);

	void StartAnalyze();

	void AnalyzeAll();

	void LogResults();

	double GetTotalScore(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList);

	void Analyze(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList);

	void SetMinScore(double lfMinScore);

	void AddDataCount(unsigned long long ullDataCount);

	unsigned long long GetDataCount() { return m_ullDataCount; }
	double	GetHighScore() { return m_lfHighScore; }

	bool IsContinue(int nCurrentGroup, double lfScore);

	HANDLE GetFinishedEvent() { return m_hFinishEvent; }
	bool IsRunning() { return m_bRunning; }

	vector<wstring>& GetElementNameList() { return m_elementNameList; }

	static DWORD WINAPI AnalyzeThreadProc(PVOID pParam);
private:
	HANDLE m_hAnalyzeThread = nullptr;
	HANDLE m_hFinishEvent = nullptr;
	groupList m_groupList;

	vector<wstring> m_elementNameList;
	CResultQueue m_totoalResultQueue;
	CResultQueue m_3ElemResultQueue;
	CResultQueue m_4ElemResultQueue;
	CResultQueue m_5ElemResultQueue;

	unsigned long long m_ullDataCount = 0;
	double		m_lfHighScore = 0;
	double		m_lfMinScore = 0;
	bool m_bRunning = false;
	bool m_bDataInitialized = false;
};

