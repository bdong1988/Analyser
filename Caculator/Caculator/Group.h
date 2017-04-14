#pragma once
#include "Condition.h"

struct GroupInfo
{
	unsigned long dwItemCount = 0;
	double lfScore = 0;
};

static const vector<GroupInfo> GROUPINFO = {
	{18,50},
	{13,25},
	{3, 12.5},
	{2, 12.5}
};

const unsigned long GROUPSCOREFACTOR = 10;

using conditionList = vector<CConditionPtr>;

class CGroup
{
public:
	CGroup();
	~CGroup();

	void SetTotalScore(double lfScore);
	void AddCondition(const CConditionPtr& pConditon);
	void SumConditionAverage();
	unsigned long GetSumD() { return m_ulSumD; }

	double GetScore(int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList);
private:
	double m_lfTotalScore = 0;
	unsigned long m_ulSumD = 0;
	conditionList m_condtionList;
};

