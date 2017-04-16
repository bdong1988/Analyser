#include "stdafx.h"
#include "Analyzer.h"
#include "Utility.h"
#include "Combination.h"
#include "spliter.h"
#include "AnalyzeThread.h"
#include "CSVProcessor.h"

#define SAFE_RELEASEHANDLE(p) if(p){CloseHandle(p); p = nullptr;}

static const wstring CONDITION = L"condition";
static const wstring RANGE = L"range";
static const wstring AVERAGE = L"average";

CAnalyzer::CAnalyzer()
{
	m_groupList.resize(GROUPINFO.size());
	for (unsigned long i = 0; i < GROUPINFO.size(); i++)
	{
		m_groupList[i].SetTotalScore(GROUPINFO[i].lfScore);
	}

	m_hFinishEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}	


CAnalyzer::~CAnalyzer()
{
	SAFE_RELEASEHANDLE(m_hAnalyzeThread);
	SAFE_RELEASEHANDLE(m_hResultLogThread);
	SAFE_RELEASEHANDLE(m_hFinishEvent);
}

void CAnalyzer::InitilizeData(const contentArray & rawDataArray)
{
	unsigned long dwRowCount = rawDataArray.size();
	for (unsigned long i = 0; i < dwRowCount; i++)
	{
		BOOL bAddCondition = FALSE;

		CConditionPtr pCondition = std::make_shared<CCondition>();
		unsigned long dwColCount = rawDataArray[i].size();

		if (0 == i)
		{
			for (unsigned long j = 0; j < dwColCount; j++)
			{
				if (rawDataArray[i][j].empty())
				{
					continue;
				}

				if (rawDataArray[0][j] != CONDITION && rawDataArray[0][j] != RANGE && rawDataArray[0][j] != AVERAGE)
				{
					wstring strName = rawDataArray[i][j];
					m_elementNameList.push_back(strName);
				}
			}
		}
		else
		{
			for (unsigned long j = 0; j < dwColCount; j++)
			{
				if (rawDataArray[i][j].empty())
				{
					continue;
				}
				if (0 != i)
				{
					bAddCondition = TRUE;
				}

				int nElementIndex = 0;
				if (rawDataArray[0][j] == CONDITION)
				{
					pCondition->SetName(rawDataArray[i][j]);
				}
				else if (rawDataArray[0][j] == RANGE)
				{
					unsigned long dwLow = 0, dwHigh = 0;
					ParseRange(rawDataArray[i][j], dwLow, dwHigh);
					pCondition->SetRange(dwLow, dwHigh);
				}
				else if (rawDataArray[0][j] == AVERAGE)
				{
					unsigned long dwAverage = CUtility::GetNumFromString(rawDataArray[i][j]);
					pCondition->SetAverage(dwAverage);
				}
				else if (!rawDataArray[0][j].empty())
				{
					unsigned long dwContent = CUtility::GetNumFromString(rawDataArray[i][j]);
					pCondition->AddContent(nElementIndex, dwContent, false);
					nElementIndex++;
				}
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

	for (auto& group : m_groupList)
	{
		group.SumConditionAverage();
	}

}

void CAnalyzer::ParseRange(const wstring& strRange, unsigned long & ulLow, unsigned long & ulHigh)
{
	wregex pattern(LR"(([0-9.]+)-([0-9.]+))");
	wcmatch matchResult;
	if (regex_match(strRange.c_str(), matchResult, pattern))
	{
		ulLow = CUtility::GetNumFromString(matchResult[1].str());
		ulHigh = CUtility::GetNumFromString(matchResult[2].str());
	}
}

void CAnalyzer::StartAnalyze()
{
	if (m_bRunning)
	{
		return;
	}

	if (!m_hAnalyzeThread)
	{
		SAFE_RELEASEHANDLE(m_hAnalyzeThread);
	}

	m_hAnalyzeThread = CreateThread(NULL, 0, AnalyzeThreadProc, this, 0, nullptr);

	if (!m_hResultLogThread)
	{
		SAFE_RELEASEHANDLE(m_hResultLogThread);
	}

	m_hResultLogThread = CreateThread(NULL, 0, LogThreadProc, this, 0, nullptr);
}

void CAnalyzer::AnalyzeAll()
{
	m_bRunning = true;
	ResetEvent(m_hFinishEvent);

	m_ullDataCount = 0;
	m_lfHighScore = 0;

	CCombination combination;
	for (int i = 3; i <= 5; i++)
	{
		combination.Calculate(m_elementNameList.size(), i);

		combList selectionList = combination.GetResultsList();
		for (const auto& selection : selectionList)
		{
			m_ullDataCount += CSpliter::Calculate(i, 1000, this, selection.data());
		}
	}

	SetEvent(m_hFinishEvent);
	m_bRunning = false;
}

void CAnalyzer::LogResults()
{
	CCSVProcessor csvProcessor;
	wstring strFileName;
	strFileName.resize(1024);
	GetModuleFileName(nullptr, (LPWSTR)strFileName.data(), 1024);
	wstring::size_type pos = strFileName.rfind(L"\\");
	strFileName = strFileName.substr(0, pos + 1);
	strFileName += L"AnalyzeResult.csv";
	if (!csvProcessor.OpenCSV(strFileName.c_str()))
	{
		return;
	}

	csvProcessor.Write(GenerateResultHeader());
	bool bFinished = true;
	bool bContinue = true;
	while (bContinue)
	{
		HANDLE events[2] = {
			m_resultQueue.GetQueueEvent(),
			m_hFinishEvent
		};

		DWORD eventIndex = WaitForMultipleObjects(_countof(events), events, FALSE, INFINITE);
		switch (eventIndex)
		{
		case 0:
			if (!m_resultQueue.IsEmpty())
			{
				csvProcessor.Write(m_resultQueue.PeekResult());
			}
			else
			{
				m_resultQueue.ClearEvent();
			}
			break;
		case 1:
			if (m_resultQueue.IsEmpty())
			{
				csvProcessor.CloseCSV();
				bContinue = false;
			}
			bFinished = true;
			break;
		default:
			break;
		}
	}

	return;
}

double CAnalyzer::GetTotalScore(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList)
{
	double lfTotalScore = 0;
	int nGroupIndex = 0;
	for (auto& group : m_groupList)
	{
		lfTotalScore += group.GetScore(nElemCount, pUlRatioList, pUlContentIndexList);
		if (!IsContinue(nGroupIndex, lfTotalScore))
		{
			break;
		}
		nGroupIndex++;
	}

	return lfTotalScore;
}

void CAnalyzer::Analyze(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList)
{
	double lfTotalScore = GetTotalScore(nElemCount, pUlRatioList, pUlContentIndexList);

	if (lfTotalScore > m_ulMinScore)
	{
		if (lfTotalScore > m_lfHighScore)
		{
			m_lfHighScore = lfTotalScore;
		}

		m_resultQueue.PushResult(lfTotalScore, nElemCount, pUlContentIndexList, pUlRatioList);
	}
}

void CAnalyzer::SetMinScore(unsigned long ulMinScore)
{
	m_ulMinScore = ulMinScore;
}

void CAnalyzer::AddDataCount(unsigned long long ullDataCount)
{
	m_ullDataCount += ullDataCount;
}

bool CAnalyzer::IsContinue(int nCurrentGroup, double lfScore)
{
	double lfTempScore = 0;
	for (int i = GROUPINFO.size()-1; i > nCurrentGroup; i--)
	{
		lfTempScore += GROUPINFO[i].lfScore;
	}
	
	if (lfTempScore + lfScore >= m_ulMinScore)
	{
		return true;
	}

	return false;
}

wstring CAnalyzer::GenerateResultHeader()
{
	wstring strHeader(L"ตรทึ,");
	int nSize = m_elementNameList.size();

	for (int i = 0; i < nSize; i++)
	{
		strHeader += m_elementNameList[i];
		if (i != nSize - 1)
		{
			strHeader += L",";
		}
		else
		{
			strHeader += L"\r\n";
		}
	}

	return strHeader;
}

DWORD WINAPI CAnalyzer::AnalyzeThreadProc(PVOID pParam)
{
	if (pParam)
	{
		((CAnalyzer*)pParam)->AnalyzeAll();
	}
	return 0;
}

DWORD WINAPI CAnalyzer::LogThreadProc(PVOID pParam)
{
	if (pParam)
	{
		((CAnalyzer*)pParam)->LogResults();
	}
	return 0;
}
