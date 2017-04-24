#include "stdafx.h"
#include "Analyzer.h"
#include "Utility.h"
#include "Combination.h"
#include "spliter.h"
#include "CSVProcessor.h"
#include "CompositionAnalyzerDlg.h"
#include "ExcelProcessor.h"
#include "AnalyzerException.h"
#include <locale>
#include <codecvt>

#define SAFE_RELEASEHANDLE(p) if(p){CloseHandle(p); p = nullptr;}

static const wstring CONDITION = L"condition";
static const wstring RANGE = L"range";
static const wstring AVERAGE = L"average";

#define RESULT_QUEUE_SIZE 5
#define RESULT_LOG_COUNT 1000
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
	m_totoalResultQueue.Resize(RESULT_LOG_COUNT);

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
	for (unsigned long i = 0; i < GROUPINFO.size(); i++)
	{
		m_groupList[i].ClearData();
	}
	m_totoalResultQueue.Clear();
	m_totoalResultQueue.Resize(RESULT_LOG_COUNT);
	m_ullDataCount = 0;
	m_lfHighScore = 0;
	m_lfMinScore = 0;
	m_bRunning = false;
	m_bDataInitialized = false;
	m_bContiue = true;
	ResetEvent(m_hFinishEvent);
}

BOOL CAnalyzer::InitilizeData(const contentArray & rawDataArray)
{
	try
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

				if (m_elementNameList.size() != ELEMENTCOUNT && m_elementNameList.size() != ELEMENTCOUNT - 1)
				{
					return FALSE;
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
						if (j == ELEMENTCOUNT - 1)
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
	catch (const std::exception& ex)
	{
		return FALSE;
	}

	return TRUE;

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
	else
	{
		throw CAnalyzeException("Element Range has the incorrect format");
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
	int nCount = m_elementNameList.size();
	int nMin = 0;
	int nMax = 0;
	for (int i = 3; i <= 5; i++)
	{
		m_displayResultQueue.Clear();
		m_displayResultQueue.Resize(RESULT_QUEUE_SIZE);
		if (nCount == ELEMENTCOUNT)
		{
			combination.Calculate(m_elementNameList.size() - 1, i - 1);
		}
		else
		{
			combination.Calculate(m_elementNameList.size(), i);
		}
		

		combList selectionList = combination.GetResultsList();
		for (auto& selection : selectionList)
		{
			if (nCount == ELEMENTCOUNT)
			{
				selection.push_back(ELEMENTCOUNT - 1);
			}		
			CSpliter::Calculate(i, 1000, this, selection.data());
			if (!m_bContiue)
			{
				break;
			}
		}

		if (!m_bContiue)
		{
			break;
		}

		if (i == 5)
		{
			for (int j = 0; j < 5; j++)
			{
				unsigned long indexList[5] = { 0 };
				unsigned long ratioList[5] = { 0 };
				auto elemntList = m_displayResultQueue.m_resultQueue[j].m_listElements;
				for (int k = 0; k < elemntList.size(); k++)
				{
					indexList[k] = elemntList[k].m_nElemIndex;
					ratioList[k] = elemntList[k].m_ulRatio;
				}
				CSpliter::CalcuateHighPresicion5(1000, this, indexList, ratioList);
			}

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
	CCSVProcessor csvProcessor;
	if (!csvProcessor.OpenCSV(m_strLogFileName.c_str()))
	{
		AfxMessageBox(L"保存结果失败， 请检查保存结果文件名是否合法。");
		return;
	}
	csvProcessor.Write(GenerateLogHeader());
	
	int nLogCount = m_totoalResultQueue.GetCurrent();
	BOOL b8Elem = TRUE;
	if (m_elementNameList.size() == 8)
	{
		b8Elem = TRUE;
	}
	else
	{
		b8Elem = FALSE;
	}

	wstring strRow;
	for (int i = 0; i < nLogCount; i++)
	{
		if (b8Elem)
		{
			strRow = m_totoalResultQueue.m_resultQueue[i].CovertToLogRow8Elem();
		}
		else
		{
			strRow = m_totoalResultQueue.m_resultQueue[i].CovertToLogRow7Elem();
		}
		csvProcessor.Write(strRow);
	}
	
	csvProcessor.CloseCSV();

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
	m_ullDataCount++;
	if (lfTotalScore > m_lfMinScore)
	{
		m_displayResultQueue.PushResult(lfTotalScore, nElemCount, pUlRatioList, pUlContentIndexList);
		if (lfTotalScore > m_lfHighScore)
		{
			m_lfHighScore = lfTotalScore;		
		}
		m_totoalResultQueue.PushResult(lfTotalScore, nElemCount, pUlRatioList, pUlContentIndexList);
	}
}

void CAnalyzer::SetMinScore(double lfMinScore)
{
	m_lfMinScore = lfMinScore;
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

void CAnalyzer::SetLogFileName(const wstring& strFileName)
{
	m_strLogFileName = strFileName;
}

wstring CAnalyzer::GenerateLogHeader()
{
	wstring strLogHeader(L"得分,");
	for (const auto& elem : m_elementNameList)
	{
		strLogHeader += elem;
		strLogHeader += L',';
	}

	return strLogHeader;
}

DWORD WINAPI CAnalyzer::AnalyzeThreadProc(PVOID pParam)
{
	if (pParam)
	{
		((CAnalyzer*)pParam)->AnalyzeAll();
	}
	return 0;
}
