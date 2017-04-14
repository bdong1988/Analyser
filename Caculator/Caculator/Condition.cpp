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

void CCondition::SetAverage(unsigned long dwAverage)
{
	m_ulAverage = dwAverage;
}

void CCondition::SetRange(unsigned long dwLow, unsigned long dwHigh)
{
	m_range.high = dwHigh;
	m_range.low = dwLow;
}

void CCondition::AddContent(const wstring& strElementName, unsigned long dwContent, bool bFixed)
{
	element newElement(strElementName,dwContent,bFixed);
	m_contentMap[strElementName]
}

double CCondition::CalculateE(unsigned long ulGroupTotalScore, unsigned long ulSumD, int nElemCount, unsigned long* pUlRatioList, unsigned long* pUlContentList)
{
	unsigned long ulB = 0;
	for (int i = 0; i < nElemCount; i++)
	{
		ulB += pUlRatioList[i] * pUlContentList[i];
	}

	if (ulB >= m_range.low || ulB <= m_range.high)
	{
		return 0;
	}

	double dfC = 0;

	if (ulB < m_range.low)
	{
		dfC = (double)(m_range.low - ulB) / (double)m_range.low;
	}
	else
	{
		dfC = (double)(ulB - m_range.high) / (double)m_range.high;
	}

	return (double)ulGroupTotalScore * dfC * m_ulAverage / (double)ulSumD;
}
