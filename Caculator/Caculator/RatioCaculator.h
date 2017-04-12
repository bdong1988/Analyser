#pragma once

#include <vector>
#include <string>
#include <memory>
#include <tuple>
using namespace std;

using RatioList = vector<vector<unsigned long>>;
class CRatioCaculator
{
public:
	CRatioCaculator()
	{

	}
	~CRatioCaculator()
	{

	}

	void Caculater_3(unsigned long ulMax, RatioList& ratioList)
	{
		unsigned long maxRatio = ulMax - 2;
		unsigned long ulCountA = 0;
		unsigned long ulCountB = 0;
		unsigned long ulCountC = 0;

		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			ulCountA = i;
			for (unsigned long j = 1; j <= maxRatio; j++)
			{
				ulCountB = j;
				if (ulCountA + ulCountB >= ulMax)
				{
					break;
				}

				ulCountC = ulMax - i - j;
				ratioList.push_back({ ulCountA , ulCountB , ulCountC });
			}		
		}
	}

	void Caculater_4(unsigned long ulMax, RatioList& ratioList)
	{
		unsigned long maxRatio = ulMax - 3;
		unsigned long ulCountA = 0;
		unsigned long ulCountB = 0;
		unsigned long ulCountC = 0;
		unsigned long ulCountD = 0;

		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			ulCountA = i;
			for (unsigned long j = 1; j <= maxRatio; j++)
			{
				ulCountB = j;
				for (unsigned long k = 1; k <= maxRatio; k++)
				{
					ulCountC = k;
					if (ulCountA + ulCountB + ulCountC >= ulMax)
					{
						break;
					}
					ulCountD = ulMax - i - j - k;
					ratioList.push_back({ ulCountA , ulCountB , ulCountC, ulCountD });
				}			
			}
		}
	}

	void Caculater_5(unsigned long ulMax, RatioList& ratioList)
	{
		unsigned long maxRatio = ulMax - 3;
		unsigned long ulCountA = 0;
		unsigned long ulCountB = 0;
		unsigned long ulCountC = 0;
		unsigned long ulCountD = 0;
		unsigned long ulCountE = 0;

		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			ulCountA = i;
			for (unsigned long j = 1; j <= maxRatio; j++)
			{
				ulCountB = j;
				for (unsigned long k = 1; k <= maxRatio; k++)
				{
					ulCountC = k;
					for (unsigned long l = 1; l <= maxRatio; l++)
					{
						ulCountD = l;
						if (ulCountA + ulCountB + ulCountC + ulCountD >= ulMax)
						{
							break;
						}
						ulCountE = ulMax - i - j - k - l;
						m_ullDataCount++;
					}

				}
			}
		}
	}

	void Caculate(unsigned long ulElementCount, unsigned long ulCount)
	{
		switch (ulElementCount)
		{
		case 3:
			Caculater_3(ulCount, m_ratioList);
			break;
		case 4:
			Caculater_4(ulCount, m_ratioList);
			break;
		case 5:
			Caculater_5(ulCount, m_ratioList);
			break;
		default:
			break;
		}
	}

private:
	RatioList m_ratioList;
	unsigned long long m_ullDataCount = 0;
};

