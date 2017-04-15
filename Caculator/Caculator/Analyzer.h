#pragma once
#include "Group.h"

static const unsigned long MinRatio = 1;
static const unsigned long MaxRatio = 1000;

using groupList = vector<CGroup>;

class CAnalyzer
{

public:
	CAnalyzer();
	~CAnalyzer();

	void InitilizeData(const contentArray& rawDataArray);

	void ParseRange(const wstring& strRange, unsigned long& ulLow, unsigned long& ulHigh);

	void AnalyzeAll();

	double GetTotalScore(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList);

	void Analyze(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList);

	void SetMinScore(unsigned long ulMinScore);

	void AddDataCount(unsigned long long ullDataCount);

	unsigned long long GetDataCount() { return m_ullDataCount; }

	bool IsContinue(int nCurrentGroup, double lfScore);
private:
	groupList m_groupList;
	vector<wstring> m_elementNameList;
	unsigned long m_ulResultCount = 0;
	unsigned long m_ulMinScore = 0;
	unsigned long long m_ullDataCount = 0;
};

