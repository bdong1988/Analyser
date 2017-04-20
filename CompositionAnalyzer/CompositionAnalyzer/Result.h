#pragma once

#define BUFFER_SIZE 1024
#define QUEUE_SIZE 1000

class Element
{
public:
	Element(int nIndex, unsigned long ulRatio):
		m_nElemIndex(nIndex),
		m_ulRatio(ulRatio)
	{

	}
	Element()
	{

	}

	~Element()
	{

	}
	int m_nElemIndex = 0;
	unsigned long m_ulRatio = 0;
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

	CResult(const CResult& rhs)
	{
		m_lfScore = rhs.m_lfScore;
		m_listElements = rhs.m_listElements;
	}
	~CResult()
	{

	}

	CResult& operator = (CResult&& rhs)
	{
		m_lfScore = rhs.m_lfScore;
		m_listElements = std::move(rhs.m_listElements);

		return *this;
	}



	void SetElementCount(int nCount)
	{
		m_listElements.resize(nCount);
	}

	void SetData(double lfScore, int nElementCount, const unsigned long* pElementIndexList, const unsigned long* pElementRatioList)
	{
		m_lfScore = lfScore;
		for (int i = 0; i < nElementCount; i++)
		{
			m_listElements[i].m_nElemIndex = pElementIndexList[i];
			m_listElements[i].m_ulRatio = pElementRatioList[i];
		}
	}

	bool operator < (const CResult& rfValue)
	{
		return m_lfScore > rfValue.m_lfScore;
	}

	double m_lfScore = 0;
	elemntList m_listElements;
};

class CResultQueue
{
public:
	CResultQueue()
	{
		m_resultQueue.resize(QUEUE_SIZE + 1);
		m_nMaxIndex = QUEUE_SIZE;
	}

	~CResultQueue()
	{

	}

	void Resize(unsigned long ulSize)
	{
		m_resultQueue.resize(ulSize + 1);
		m_nMaxIndex = ulSize;
	}

	void PushResult(double lfScore, int nElementCount, const unsigned long* pElementRatioListconst, const unsigned long* pElementIndexList)
	{
		if (m_nCurrentIndex < m_nMaxIndex)
		{
			m_resultQueue[m_nCurrentIndex++] = CResult(lfScore, nElementCount, pElementIndexList,pElementRatioListconst);
		}
		else if (m_nCurrentIndex == m_nMaxIndex)
		{
			m_resultQueue[m_nCurrentIndex] = CResult(lfScore, nElementCount, pElementIndexList, pElementRatioListconst);
		}

		std::sort(m_resultQueue.begin(), m_resultQueue.end());
	}
	void Clear()
	{
		m_resultQueue.clear();
	}

	vector<CResult>& GetResults()
	{
		return m_resultQueue;
	}

	unsigned long GetCurrent()
	{
		return m_nCurrentIndex;
	}

	vector<CResult> m_resultQueue;
	int m_nCurrentIndex = 0;
	int m_nMaxIndex = 0;
};