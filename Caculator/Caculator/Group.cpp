#include "stdafx.h"
#include "Group.h"


CGroup::CGroup()
{
}


CGroup::~CGroup()
{
}

void CGroup::SetTotalScore(DWORD dwScore)
{
	m_dwTotalScore = dwScore;
}

void CGroup::AddCondition(const CConditionPtr& pConditon)
{
	m_condtionList.push_back(pConditon);
}