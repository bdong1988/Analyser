#include "stdafx.h"
#include "Analyzer.h"
#include "Utility.h"
#include "Combination.h"
#include "spliter.h"
#include "CSVProcessor.h"
#include "CompositionAnalyzerDlg.h"

#define SAFE_RELEASEHANDLE(p) if(p){CloseHandle(p); p = nullptr;}

static const wstring CONDITION = L"condition";
static const wstring RANGE = L"range";
static const wstring AVERAGE = L"average";

#define RESULT_QUEUE_SIZE 5
enum
{
	ELEMNT_COUNT3 = 3,
	ELEMNT_COUNT4 = 4,
	ELEMNT_COUNT5 = 5
};

CAnalyzer::CAnalyzer()
{
	m_groupList.resize(GROUPINFO.size());
	for (unsigned long i = 0; i < GROUPINFO.size(); i++)
	{
		m_groupList[i].SetTotalScore(GROUPINFO[i].lfScore);
	}

	m_displayResultQueue.Resize(RESULT_QUEUE_SIZE);
	m_totoalResultQueue.Resize(1000);

	m_hFinishEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}	


CAnalyzer::~CAnalyzer()
{
	SAFE_RELEASEHANDLE(m_hAnalyzeThread);
	SAFE_RELEASEHANDLE(m_hFinishEvent);
}

void CAnalyzer::SetMainDlg(CCompositionAnalyzerDlg* pDlg)
{
	m_pMainDlg = pDlg;
}

void CAnalyzer::Clear()
{
	SAFE_RELEASEHANDLE(m_hAnalyzeThread);
	m_elementNameList.clear();

	m_ullDataCount = 0;
	m_lfHighScore = 0;
	m_lfMinScore = 0;
	m_bRunning = false;
	m_bDataInitialized = false;
	m_bContiue = true;
	ResetEvent(m_hFinishEvent);
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
					bool bFixed = false;
					if (j == ELEMENTCOUNT -1)
					{
						bFixed = true;
					}
					pCondition->AddContent(nElementIndex, dwContent, bFixed);
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

	m_bRunning = true;

	if (!m_hAnalyzeThread)
	{
		SAFE_RELEASEHANDLE(m_hAnalyzeThread);
	}

	m_hAnalyzeThread = CreateThread(NULL, 0, AnalyzeThreadProc, this, 0, nullptr);

}

void CAnalyzer::AnalyzeAll()
{
	ResetEvent(m_hFinishEvent);

	m_ullDataCount = 0;
	m_lfHighScore = 0;

	CCombination combination;
	for (int i = 3; i <= 5; i++)
	{
		m_displayResultQueue.Clear();
		m_displayResultQueue.Resize(RESULT_QUEUE_SIZE);
		combination.Calculate(m_elementNameList.size(), i);

		combList selectionList = combination.GetResultsList();
		for (auto& selection : selectionList)
		{
			//selection.push_back(ELEMENTCOUNT - 1);
			m_ullDataCount += CSpliter::Calculate(i, 1000, this, selection.data());
			if (!m_bContiue)
			{
				break;
			}
		}

		if (!m_bContiue)
		{
			break;
		}

		m_pMainDlg->DisplayResult(i, m_displayResultQueue);
	}
	
	if (m_bContiue)
	{
		LogTotalResults();
		m_pMainDlg->AnalyzeDone();
	}
	
	SetEvent(m_hFinishEvent);
	m_bRunning = false;
}

void CAnalyzer::LogTotalResults()
{

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

	if (lfTotalScore > m_lfMinScore)
	{
		m_displayResultQueue.PushResult(lfTotalScore, nElemCount, pUlRatioList, pUlContentIndexList);
		if (lfTotalScore > m_lfHighScore)
		{
			m_lfHighScore = lfTotalScore;		
		}
	}
}

void CAnalyzer::SetMinScore(double lfMinScore)
{
	m_lfMinScore = lfMinScore;
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
	
	if (lfTempScore + lfScore >= m_lfMinScore)
	{
		return true;
	}

	return false;
}

DWORD WINAPI CAnalyzer::AnalyzeThreadProc(PVOID pParam)
{
	if (pParam)
	{
		((CAnalyzer*)pParam)->AnalyzeAll();
	}
	return 0;
}
