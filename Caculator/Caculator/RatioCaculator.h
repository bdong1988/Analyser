#pragma once

#include <vector>
#include <string>
#include <memory>
#include <tuple>
using namespace std;

using RatioList = vector<vector<unsigned long>>;
class CRatioCaculator
{
public:
	CRatioCaculator(unsigned long ulElementNum, unsigned long ulMax)
		:m_ulElementNum(ulElementNum),
		m_ulMax(ulMax)
	{

	}
	~CRatioCaculator()
	{

	}

	RatioList& GetRatioList() { return m_ratioList; }

	void CaculateInternal(unsigned long ulLow, unsigned long ulHigh, unsigned long ulWaitingElemCount, unsigned long ulTotalElemCount, int nCurrentRatioIndex, RatioList& ratioList)
	{
		for (unsigned long i = ulLow; i < ulHigh; i++)
		{
			if (0 == i - ulLow)
			{
				continue;
			}

			if (1 != ulWaitingElemCount && i - ulLow != 1)
			{
				ratioList.push_back(vector<unsigned long>(ulTotalElemCount));
			}

			nCurrentRatioIndex = ratioList.size() - 1;

			if (1 == ulWaitingElemCount)
			{
				ratioList[nCurrentRatioIndex][ulTotalElemCount - ulWaitingElemCount] = ulHigh - ulLow;
				return;
			}
			else
			{
				ratioList[nCurrentRatioIndex][ulTotalElemCount - ulWaitingElemCount] = i - ulLow;
			}


			if (1 != ulWaitingElemCount)
			{
				unsigned long ulNewLow = GetNewLowBound(ratioList[nCurrentRatioIndex]);
				CaculateInternal(ulNewLow, ulHigh, ulWaitingElemCount - 1, ulTotalElemCount, nCurrentRatioIndex, ratioList);
			}
		}
	}

	unsigned long GetNewLowBound(const vector<unsigned long>& ratios)
	{
		unsigned long ulNewLowBound = 0;
		for (unsigned long ulRatio : ratios)
		{
			ulNewLowBound += ulRatio;
		}

		return ulNewLowBound;
	}

	void Caculator()
	{
		m_ratioList.clear();
		int nCurrentRatio = -1;
		m_ratioList.push_back(vector<unsigned long>(m_ulElementNum));
		CaculateInternal(0, m_ulMax, m_ulElementNum, m_ulElementNum, nCurrentRatio, m_ratioList);
	}


private:
	unsigned long m_ulElementNum = 0;
	unsigned long m_ulMax = 1000;
	RatioList m_ratioList;
};

