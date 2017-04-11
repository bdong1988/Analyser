#include "stdafx.h"
#include "Analyzer.h"

static const wstring CONDITION = L"condition";
static const wstring RANGE = L"range";
static const wstring AVERAGE = L"average";

static const set<wstring> OILList = {
	L"╨клрсм",
	L"вьИ╣см",
	L"ягбИвясм ",
	L"╢С╤╧см",
	L"╡квясм",
	L"©Ш╩╗вясм",
	L"сЯцвсм"
};

CAnalyzer::CAnalyzer()
{
	m_groupList.resize(GROUPINFO.size());
}


CAnalyzer::~CAnalyzer()
{
}

void CAnalyzer::InitilizeData(const contentArray & rawDataArray)
{
	DWORD dwRowCount = rawDataArray.size();
	for (DWORD i = 0; i < dwRowCount; i++)
	{
		if (0 == i)
		{
			continue;
		}

		CConditionPtr pCondition = std::make_shared<CCondition>();

		DWORD dwColCount = rawDataArray[i].size();
		for (DWORD j = 0; j < dwColCount; j++)
		{
			if (rawDataArray[i][j].empty())
			{
				continue;
			}

			if (rawDataArray[0][j] == CONDITION)
			{
				pCondition->SetName(rawDataArray[i][j]);
			}
			else if (rawDataArray[0][j] == RANGE)
			{
				DWORD dwLow =0, dwHigh = 0;
				ParseRange(rawDataArray[i][j], dwLow, dwHigh);
			}
		}
	}
}

void CAnalyzer::ParseRange(const wstring& strRange, DWORD & dwLow, DWORD & dwHigh)
{
	wregex pattern(LR"((.+)-(.+))");
	wcmatch matchResult;
	if (regex_match(strRange.c_str(), matchResult, pattern))
	{
		wstring strLow = matchResult[1].str();
		wstring strHigh = matchResult[2].str();
	}
}
