#pragma once

#include <iostream>
#include <vector>
using namespace std;

using combList = vector<vector<unsigned long>>;
class CCombination
{
public:
	CCombination()
	{
	}

	~CCombination()
	{

	}

	unsigned long Calculate(unsigned long ulRange, unsigned long ulCount)
	{
		m_resultsList.clear();
		m_tempList.resize(ulCount);
		return CalculateInternal(ulRange, ulCount);
	}

	unsigned long CalculateInternal(unsigned long m, unsigned long n)
	{
		for (unsigned long i = m; i >= n; i--)
		{
			m_tempList[n - 1] = i - 1;
			if (n>1)
			{
				CalculateInternal(i - 1, n - 1);
			}
			else
			{
				m_ulTotalCount++;
				m_resultsList.push_back(m_tempList);
			}
		}

		return m_ulTotalCount;
	}

	combList& GetResultsList() { return m_resultsList; }

private:
	combList m_resultsList;
	vector<unsigned long> m_tempList;
	unsigned long m_ulRange = 0;
	unsigned long m_ulCount = 0;
	unsigned long m_ulTotalCount = 0;
};
