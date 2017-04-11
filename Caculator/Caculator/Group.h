#pragma once
#include "Condition.h"

struct GroupInfo
{
	DWORD dwItemCount = 0;
	DWORD dwScore = 0;
};

static const vector<GroupInfo> GROUPINFO = {
	{18,500},
	{13, 250},
	{3, 125},
	{2, 125}
};

static const DWORD GroupScoreFactor = 10;

using conditionList = vector<CConditionPtr>;

class CGroup
{
public:
	CGroup();
	~CGroup();

	void SetTotalScore(DWORD dwScore);
private:
	DWORD m_dwTotalScore = 0;
	conditionList m_condtionList;
};

