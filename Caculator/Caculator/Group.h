#pragma once
#include "Condition.h"

struct GroupInfo
{
	unsigned long dwItemCount = 0;
	unsigned long dwScore = 0;
};

static const vector<GroupInfo> GROUPINFO = {
	{18,500},
	{13, 250},
	{3, 125},
	{2, 125}
};

static const unsigned long GroupScoreFactor = 10;

using conditionList = vector<CConditionPtr>;

class CGroup
{
public:
	CGroup();
	~CGroup();

	void SetTotalScore(unsigned long ulScore);
	void AddCondition(const CConditionPtr& pConditon);
	void SumConditionAverage();
	unsigned long GetSumD() { return m_ulSumD; }

	double GetScore(int nElemCount, unsigned long* pUlRatioList, unsigned long* pUlContentList);
private:
	unsigned long m_ulTotalScore = 0;
	unsigned long m_ulSumD = 0;
	conditionList m_condtionList;
};

