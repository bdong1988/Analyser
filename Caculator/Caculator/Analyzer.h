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

	void Analyze();

	double GetTotalScore(int nElemCount, unsigned long* pUlRatioList, unsigned long* pUlContentList);

private:
	groupList m_groupList;
};

