#pragma once


#include "CritlSec.h"

#define BUFFER_SIZE 1024
#define QUEUE_SIZE 1000

class Element
{
public:
	Element(int nIndex, double lfRatio):
		m_nElemIndex(nIndex),
		m_lfRatio(lfRatio)
	{

	}

	~Element()
	{

	}
	int m_nElemIndex = 0;
	double m_lfRatio = 0;
};

using elemntList = vector<Element>;

class CResult
{
public:
	CResult(double lfScore, int nElementCount, const unsigned long* pElementIndexList, const unsigned long* pElementRatioList):
		m_lfScore(lfScore)
	{
		for (int i = 0; i < nElementCount; i++)
		{
			m_listElements.push_back(Element(pElementIndexList[i], pElementRatioList[i]));
		}
	}

	CResult()
	{

	}
	~CResult()
	{

	}

	bool operator < (const CResult& rfValue)
	{
		return m_lfScore > rfValue.m_lfScore;
	}

	wstring ConvertToRow()
	{
		wstring strRow;
		double lfRatios[8] = { 0 };

		wchar_t str[BUFFER_SIZE] = { 0 };
		for (const auto& element : m_listElements)
		{
			lfRatios[element.m_nElemIndex] = element.m_lfRatio;
		}

		StringCbPrintf(str, BUFFER_SIZE * sizeof(wchar_t), L"%.3lf,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf", 
				m_lfScore, lfRatios[0], lfRatios[1], lfRatios[2], lfRatios[3], lfRatios[4], lfRatios[5], lfRatios[6], lfRatios[7]);

		strRow = str;

		return strRow;
	}



	double m_lfScore = 0;
	elemntList m_listElements;
};

class CResultQueue
{
public:
	CResultQueue()
	{
		m_resultQueue.resize(QUEUE_SIZE);
	}

	~CResultQueue()
	{

	}

	void Resize(unsigned long ulSize)
	{
		m_resultQueue.resize(ulSize);
	}

	void PushResult(double lfScore, int nElementCount, const unsigned long* pElementRatioListconst, const unsigned long* pElementIndexList)
	{

	}

	void Clear()
	{
		m_resultQueue.clear();
	}

private:
	vector<CResult> m_resultQueue;
};