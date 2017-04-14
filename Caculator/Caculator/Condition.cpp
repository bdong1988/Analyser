#include "stdafx.h"
#include <math.h>
#include "Condition.h"


CCondition::CCondition()
{
}


CCondition::~CCondition()
{
}

void CCondition::SetName(const wstring& strName)
{
	m_strCondtionName = strName;
}

void CCondition::SetAverage(unsigned long ulAverage)
{
	m_ulAverage = ulAverage;
}

void CCondition::SetRange(unsigned long ulLow, unsigned long ulHigh)
{
	m_range.high = ulHigh*FACTOR;
	m_range.low = ulLow*FACTOR;
}

void CCondition::AddContent(int nIndex, unsigned long ulContent, bool bFixed)
{
	m_contentList.push_back(element(nIndex, ulContent, bFixed));
}

void CCondition::CalculateH(unsigned long ulSumD)
{
	m_lfH = (double)m_ulAverage / (double)ulSumD * m_lfGroupTotalScore;
}

void CCondition::SetGroupTotalScore(double lfScore)
{
	m_lfGroupTotalScore = lfScore;
}

double CCondition::CalculateE(const int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList)
{
	unsigned long ulB = 0;
	for (int i = 0; i < nElemCount; i++)
	{
		ulB += pUlRatioList[i] * m_contentList[pUlContentIndexList[i]].m_ulContent;
	}

	if (ulB >= m_range.low && ulB <= m_range.high)
	{
		return 0;
	}

	double lfC = 0;

	if (ulB < m_range.low)
	{
		lfC = (double)(m_range.low - ulB) / (double)m_range.low;
	}
	else
	{
		lfC = (double)(ulB - m_range.high) / (double)m_range.high;
	}

	double lfE = lfC * m_lfH;

	return lfE;
}
