#pragma once


#include "CritlSec.h"

#define BUFFER_SIZE 1024
#define QUEUE_SIZE 10240000

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
		m_resultQueue.resize(m_ulQueueSize);
		m_hQueueEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}

	~CResultQueue()
	{

	}

	void PushResult(double lfScore, int nElementCount, const unsigned long* pElementIndexList, const unsigned long* pElementRatioList)
	{
		m_resultQueue[m_nQueueHead] = CResult(lfScore, nElementCount, pElementIndexList, pElementRatioList);
		m_nQueueHead++;

		if (m_nQueueHead >= m_ulQueueSize)
		{
			m_nQueueHead = m_nQueueHead % m_ulQueueSize;
			if (m_nQueueHead == m_nQueueTail)
			{
				m_ulQueueSize = m_ulQueueSize * 2;
				m_resultQueue.resize(m_ulQueueSize);

			}
		}

		SetEvent(m_hQueueEvent);
	}

	void ClearEvent()
	{
		ResetEvent(m_hQueueEvent);
	}

	wstring PeekResult()
	{
		wstring strResult = m_resultQueue[m_nQueueTail].ConvertToRow();
		m_nQueueTail++;
		if (m_nQueueTail >= m_ulQueueSize)
		{
			m_nQueueTail = m_nQueueTail % m_ulQueueSize;
		}

		return strResult;
	}

	void Clear()
	{
		m_resultQueue.clear();
	}

	HANDLE GetQueueEvent()
	{
		return m_hQueueEvent;
	}

	bool IsEmpty()
	{
		return m_nQueueHead == m_nQueueTail;
	}

private:
	vector<CResult> m_resultQueue;
	CCritSec m_csRsultQueueLock;
	HANDLE m_hQueueEvent = nullptr;
	unsigned long m_ulQueueSize = QUEUE_SIZE;
	int m_nQueueHead = 0;
	int m_nQueueTail = 0;
};