#pragma once
#include "Group.h"

static const DWORD MinRatio = 1;
static const DWORD MaxRatio = 1000;

using groupList = vector<CGroup>;

class CAnalyzer
{

public:
	CAnalyzer();
	~CAnalyzer();

	void InitilizeData(const contentArray& rawDataArray);

	void ParseRange(const wstring& strRange, DWORD& dwLow, DWORD& dwHigh);

private:
	groupList m_groupList;
};

