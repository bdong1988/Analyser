#include "stdafx.h"
#include "Analyzer.h"
#include "Utility.h"

static const wstring CONDITION = L"condition";
static const wstring RANGE = L"range";
static const wstring AVERAGE = L"average";
static const wstring FIXEDSOURCE = L"fixed";

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
		BOOL bAddCondition = FALSE;

		for (DWORD j = 0; j < dwColCount; j++)
		{	
			if (rawDataArray[i][j].empty())
			{
				continue;
			}

			
			if (rawDataArray[0][j] == CONDITION)
			{
				bAddCondition = TRUE;
				pCondition->SetName(rawDataArray[i][j]);
			}
			else if (rawDataArray[0][j] == RANGE)
			{
				bAddCondition = TRUE;
				DWORD dwLow = 0, dwHigh = 0;
				ParseRange(rawDataArray[i][j], dwLow, dwHigh);
				pCondition->SetRange(dwLow, dwHigh);
			}
			else if (rawDataArray[0][j] == AVERAGE)
			{
				bAddCondition = TRUE;
				DWORD dwAverage = CUtility::GetNumFromString(rawDataArray[i][j]);
				pCondition->SetAverage(dwAverage);
			}
			else if (rawDataArray[0][j] == FIXEDSOURCE)
			{
				bAddCondition = TRUE;
				DWORD dwContent = CUtility::GetNumFromString(rawDataArray[i][j]);
				pCondition->SetFixedContent(dwContent);
			}
			else if (!rawDataArray[0][j].empty())
			{
				bAddCondition = TRUE;
				wstring strName = rawDataArray[0][j];
				DWORD dwContent = CUtility::GetNumFromString(rawDataArray[i][j]);
				pCondition->AddContent(strName, dwContent);
			}
		}
		if (bAddCondition)
		{
			if (i <= 18)
			{
				m_groupList[0].AddCondition(pCondition);
			}
			else if (i > 18 && i <= 31)
			{
				m_groupList[1].AddCondition(pCondition);
			}
			else if (i > 31 && i <= 34)
			{
				m_groupList[2].AddCondition(pCondition);
			}
			else
			{
				m_groupList[3].AddCondition(pCondition);
			}
		}

	}
}

void CAnalyzer::ParseRange(const wstring& strRange, DWORD & dwLow, DWORD & dwHigh)
{
	wregex pattern(LR"(([0-9.]+)-([0-9.]+))");
	wcmatch matchResult;
	if (regex_match(strRange.c_str(), matchResult, pattern))
	{
		dwLow = CUtility::GetNumFromString(matchResult[1].str());
		dwHigh = CUtility::GetNumFromString(matchResult[2].str());
	}
}

void CAnalyzer::Analyze()
{

}
