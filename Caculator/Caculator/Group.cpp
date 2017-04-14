#include "stdafx.h"
#include "Group.h"


CGroup::CGroup()
{
}


CGroup::~CGroup()
{
}

void CGroup::SetTotalScore(double lfScore)
{
	m_lfTotalScore = lfScore;
}

void CGroup::AddCondition(const CConditionPtr& pConditon)
{
	m_condtionList.push_back(pConditon);
	pConditon->SetGroupTotalScore(m_lfTotalScore);
}

void CGroup::SumConditionAverage()
{
	for (const auto& condition : m_condtionList)
	{
		m_ulSumD += condition->GetD();
	}

	for (const auto& condition : m_condtionList)
	{
		condition->CalculateH(m_ulSumD);
	}
}

double CGroup::GetScore(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList)
{
	double lfSumE = 0;
	for (const auto& condition: m_condtionList)
	{
		double lfE = condition->CalculateE(nElemCount, pUlRatioList, pUlContentIndexList);
		lfSumE += lfE;
	}

	double lfG = m_lfTotalScore - lfSumE;
	return lfG;
}
