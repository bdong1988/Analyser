#include "stdafx.h"
#include "Group.h"


CGroup::CGroup()
{
}


CGroup::~CGroup()
{
}

void CGroup::SetTotalScore(unsigned long ulScore)
{
	m_ulTotalScore = ulScore;
}

void CGroup::AddCondition(const CConditionPtr& pConditon)
{
	m_condtionList.push_back(pConditon);
}

void CGroup::SumConditionAverage()
{
	for (const auto& condition : m_condtionList)
	{
		m_ulSumD += condition->GetD();
	}
}

double CGroup::GetScore(int nElemCount, unsigned long* pUlRatioList, unsigned long* pUlContentList)
{
	double dfSumE = 0;
	for (const auto& condition: m_condtionList)
	{
		dfSumE += condition->CalculateE(m_ulTotalScore, m_ulSumD, nElemCount, pUlRatioList, pUlContentList);
	}

	return m_ulTotalScore - dfSumE;
}
