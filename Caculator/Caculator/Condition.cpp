#include "stdafx.h"
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
void CCondition::SetAverage(DWORD dwAverage)
{
	m_dwAverage = dwAverage;
}
void CCondition::SetRange(DWORD dwLow, DWORD dwHigh)
{
	m_range.high = dwHigh;
	m_range.low = dwLow;
}

void CCondition::AddContent(const wstring& strName, DWORD dwContent)
{
	m_contentMap[strName] = dwContent;
}
