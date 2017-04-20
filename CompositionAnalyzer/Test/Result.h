#pragma once

#define BUFFER_SIZE 1024
#define QUEUE_SIZE 1000

#include <vector>
#include <algorithm>
using namespace std;

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

	void PushResult(double lfScore)
	{
		if (m_nCurrentIndex < m_nMaxIndex)
		{
			m_resultQueue[m_nCurrentIndex++] = lfScore;
		}
		else if (m_nCurrentIndex == m_nMaxIndex)
		{
			if (lfScore <= m_resultQueue[m_nCurrentIndex -1])
			{
				return;
			}

			m_resultQueue[m_nCurrentIndex] = lfScore;
		}

		std::sort(m_resultQueue.begin(), m_resultQueue.end(), [](double a, double b) {return a > b; });
	}

	void PushResult2(double lfScore)
	{
		int nIndex = -1;
		if (m_nCurrentIndex < m_nMaxIndex)
		{
			nIndex = m_nCurrentIndex++;
		}
		else if(m_nCurrentIndex == m_nMaxIndex)
		{
			nIndex == m_nCurrentIndex;
		}

		int nInsertIndex = FindSortIndex(lfScore, 0, nIndex);
		m_resultQueue[nIndex] = lfScore;
		if (nInsertIndex != nIndex)
		{
			std::sort(m_resultQueue.begin() + nInsertIndex, m_resultQueue.end(), [](double a, double b) {return a > b; });
		}
	}

	int FindSortIndex(double lfScore, int nBeginIndex, int nEndIndex)
	{
		int nMidIndex = (nEndIndex - nBeginIndex) / 2;
		if (0 == nMidIndex)
		{
			double lfFirstVal = m_resultQueue[nBeginIndex];
			double lfSecondVal = m_resultQueue[nEndIndex];
			if (lfFirstVal >= lfScore)
			{
				return nEndIndex;
			}
			else
			{
				return nBeginIndex;
			}
		}
		double lfMiddleValue = m_resultQueue[nMidIndex];
		if (lfMiddleValue < lfScore)
		{
			double lfNeighbourValue = m_resultQueue[nMidIndex - 1];
			if (lfNeighbourValue >= lfScore)
			{
				return nMidIndex;
			}
			else
			{
				return FindSortIndex(lfScore, nBeginIndex, nMidIndex - 1);
			}
		}
		else
		{
			double lfNeighbourValue = m_resultQueue[nMidIndex + 1];
			if (lfNeighbourValue < lfScore)
			{
				return nMidIndex;
			}
			else
			{
				return FindSortIndex(lfScore, nMidIndex + 1, nEndIndex);
			}

		}
	}

	int FindIndex(double lfScore)
	{

	}

	void Clear()
	{
		m_resultQueue.clear();
	}

	vector<double> m_resultQueue;
	int m_nCurrentIndex = 0;
	int m_nMaxIndex = 0;
};