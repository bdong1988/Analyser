#pragma once
#include "Analyzer.h"

class CSpliter
{
public:
	CSpliter()
	{

	}
	~CSpliter()
	{

	}

	static unsigned long Calculater_3(unsigned long ulMax, CAnalyzer* pAnalyzer, const unsigned long* pUlContentIndexList)
	{
		unsigned long maxRatio = ulMax - 2;
		unsigned long ulCount = 0;
		unsigned long ratioList[3] = { 0 };
		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			ratioList[0] = i;
			for (unsigned long j = 1; j <= maxRatio - i + 1; j++)
			{
				ratioList[1] = j;
				ratioList[2] = ulMax - i - j;
				pAnalyzer->Analyze(3, ratioList, pUlContentIndexList);
				ulCount++;
			}		
		}

		return ulCount;
	}

	static unsigned long Calculater_4(unsigned long ulMax, CAnalyzer* pAnalyzer, const unsigned long* pUlContentIndexList)
	{
		unsigned long maxRatio = ulMax - 3;
		unsigned long ulCount = 0;
		unsigned long ratioList[4] = { 0 };
		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			ratioList[0] = i;
			for (unsigned long j = 1; j <= maxRatio - i + 1; j++)
			{
				ratioList[1] = j;
				for (unsigned long k = 1; k <= maxRatio - i - j + 2; k++)
				{
					ratioList[2] = k;
					ratioList[3] = ulMax - i - j - k;
					pAnalyzer->Analyze(4, ratioList, pUlContentIndexList);
					ulCount++;
				}			
			}
		}

		return ulCount;
	}

	static unsigned long Calculater_5(unsigned long ulMax, CAnalyzer* pAnalyzer, const unsigned long* pUlContentIndexList)
	{
		unsigned long maxRatio = 960;
		unsigned long ulCount = 0;
		unsigned long ratioList[5] = { 0 };
		for (unsigned long i = 10; i <= maxRatio; i += 10)
		{
			ratioList[0] = i;
			for (unsigned long j = 10; j <= maxRatio - i + 10; j+=10)
			{
				ratioList[1] = j;
				for (unsigned long k = 10; k <= maxRatio - i - j + 20; k+=10)
				{
					ratioList[2] = k;
					for (unsigned long l = 10; l <= maxRatio - i - j - k + 30; l+=10)
					{
						ratioList[3] = l;
						ratioList[4] = ulMax - i - j - k - l;

						pAnalyzer->Analyze(5, ratioList, pUlContentIndexList);
						ulCount++;
					}

				}
			}
		}

		return ulCount;
	}

	static unsigned long Calculate(unsigned long ulElementCount, unsigned long ulMax, CAnalyzer* pAnalyzer, const unsigned long* pUlContentIndexList)
	{
		unsigned long ulCount = 0;
		switch (ulElementCount)
		{
		case 3:
			ulCount = Calculater_3(ulMax, pAnalyzer, pUlContentIndexList);
			break;
		case 4:
			ulCount = Calculater_4(ulMax, pAnalyzer, pUlContentIndexList);
			break;
		case 5:
			ulCount = Calculater_5(ulMax, pAnalyzer, pUlContentIndexList);
			break;
		default:
			break;
		}

		return ulCount;
	}

	static unsigned long CalcuateHighPresicion5(unsigned long ulMax, CAnalyzer* pAnalyzer, const unsigned long* pUlContentIndexList, const unsigned long* pUlRatioList)
	{
		unsigned long ulCount = 0;
		unsigned long ratioList[5] = { 0 };
		unsigned long ulRatioI = pUlRatioList[0];
		unsigned long ulRatioJ = pUlRatioList[1];
		unsigned long ulRatioK = pUlRatioList[2];
		unsigned long ulRatioL = pUlRatioList[3];
		unsigned long ulRatioM = pUlRatioList[4];
		for (unsigned long i = ulRatioI - 10; i <= ulRatioI + 10; i ++)
		{
			ratioList[0] = i;
			for (unsigned long j = ulRatioJ - 10; j <= ulRatioJ + 10; j++)
			{
				ratioList[1] = j;
				for (unsigned long k = ulRatioK - 10; k <= ulRatioK + 10; k++)
				{
					ratioList[2] = k;
					for (unsigned long l = ulRatioL - 10; l <= ulRatioL + 10; l++)
					{
						ratioList[3] = l;
						ratioList[4] = ulMax - i - j - k - l;
						if (ratioList[4] < ulRatioM - 10 || ratioList[4] > ulRatioM + 10)
						{
							continue;
						}

						pAnalyzer->Analyze(5, ratioList, pUlContentIndexList);
						ulCount++;
					}

				}
			}
		}

		return ulCount;
	}
};

