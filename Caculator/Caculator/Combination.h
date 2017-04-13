#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

#define N 100
class CCombination
{
public:
	CCombination(unsigned long ulRange, unsigned long ulCount):
		m_ulRange(ulRange),
		m_ulCount(ulCount)
	{
		m_tempList.resize(ulCount);
	}

	CCombination()
	{

	}

	unsigned long Caculate()
	{
		return CaculateInternal(m_ulRange, m_ulCount);
	}

	unsigned long CaculateInternal(int m, int n)
	{
		for (unsigned long i = m; i >= n; i--)
		{
			m_tempList[n - 1] = i;
			if (n>1)
			{
				CaculateInternal(i - 1, n - 1);
			}
			else
			{
				m_ulTotalCount++;
				m_totalList.push_back(m_tempList);
			}
		}

		return m_ulTotalCount;
	}

private:
	vector<vector<unsigned long>> m_totalList;
	vector<unsigned long> m_tempList;
	unsigned long m_ulRange = 0;
	unsigned long m_ulCount = 0;
	unsigned long m_ulTotalCount = 0;
};
