#pragma once

#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include <chrono>
#include <iostream>
#include <stdio.h>
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
		m_ullDataCount = 0;
		unsigned long maxRatio = ulMax - 2;
		printtime();
		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			for (unsigned long j = 1; j <= maxRatio - i + 1; j++)
			{
				unsigned long k = ulMax - i - j;
				m_ullDataCount++;
			}		
		}
		printtime();
	}

	void Caculater_4(unsigned long ulMax, RatioList& ratioList)
	{
		m_ullDataCount = 0;
		unsigned long maxRatio = ulMax - 3;
		printtime();
		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			for (unsigned long j = 1; j <= maxRatio - i + 1; j++)
			{
				for (unsigned long k = 1; k <= maxRatio - i - j + 2; k++)
				{
					unsigned long l = ulMax - i - j - k;
					m_ullDataCount++;
				}			
			}
		}
		printtime();
	}

	void Caculater_5(unsigned long ulMax, RatioList& ratioList)
	{
		m_ullDataCount = 0;
		unsigned long maxRatio = ulMax - 4;
		printtime();
		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			for (unsigned long j = 1; j <= maxRatio - i + 1; j++)
			{
				for (unsigned long k = 1; k <= maxRatio - i - j + 2; k++)
				{
					for (unsigned long l = 1; l <= maxRatio - i - j - k + 3; l++)
					{
						unsigned long m = ulMax - i - j - k - l;
						m_ullDataCount++;
					}

				}
			}
		}
		printtime();
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

	void printtime(void)
	{
		char tmpbuf[128] = { 0 };
		time_t ltime;
		struct tm *today;

		time(&ltime);
		today = localtime(&ltime);
		strftime(tmpbuf, 128, "%Y-%m-%d %H:%M:%S", today);
		cout << tmpbuf << endl;
	}


	RatioList m_ratioList;
	unsigned long long m_ullDataCount = 0;
};

