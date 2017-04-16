#pragma once
#include "Group.h"
#include "Result.h"
#include "CSVProcessor.h"

using groupList = vector<CGroup>;

class CAnalyzer
{

public:
	CAnalyzer();
	~CAnalyzer();

	void InitilizeData(const contentArray& rawDataArray);

	void ParseRange(const wstring& strRange, unsigned long& ulLow, unsigned long& ulHigh);

	void StartAnalyze();

	void AnalyzeAll();

	void LogResults();

	double GetTotalScore(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList);

	void Analyze(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList);

	void SetMinScore(unsigned long ulMinScore);

	void AddDataCount(unsigned long long ullDataCount);

	unsigned long long GetDataCount() { return m_ullDataCount; }
	double	GetHighScore() { return m_lfHighScore; }

	bool IsContinue(int nCurrentGroup, double lfScore);

	wstring GenerateResultHeader();

	static DWORD WINAPI AnalyzeThreadProc(PVOID pParam);
	static DWORD WINAPI LogThreadProc(PVOID pParam);


private:
	HANDLE m_hAnalyzeThread = nullptr;
	HANDLE m_hResultLogThread = nullptr;
	HANDLE m_hFinishEvent = nullptr;
	groupList m_groupList;
	vector<wstring> m_elementNameList;
	unsigned long m_ulMinScore = 0;
	unsigned long long m_ullDataCount = 0;
	double		m_lfHighScore = 0;
	CResultQueue m_resultQueue;
	bool m_bRunning = false;
};

