#pragma once

class CSpliter
{
public:
	CSpliter()
	{

	}
	~CSpliter()
	{

	}

	static unsigned long Splite_3(unsigned long ulMax)
	{
		unsigned long maxRatio = ulMax - 2;
		unsigned long ulCount = 0;

		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			for (unsigned long j = 1; j <= maxRatio - i + 1; j++)
			{
				unsigned long k = ulMax - i - j;
				ulCount++;
			}		
		}

		return ulCount;
	}

	static unsigned long Splite_4(unsigned long ulMax)
	{
		unsigned long maxRatio = ulMax - 3;
		unsigned long ulCount = 0;

		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			for (unsigned long j = 1; j <= maxRatio - i + 1; j++)
			{
				for (unsigned long k = 1; k <= maxRatio - i - j + 2; k++)
				{
					unsigned long l = ulMax - i - j - k;
					ulCount++;
				}			
			}
		}

		return ulCount;
	}

	static unsigned long Splite_5(unsigned long ulMax)
	{
		unsigned long maxRatio = ulMax - 4;
		unsigned long ulCount = 0;

		for (unsigned long i = 1; i <= maxRatio; i++)
		{
			for (unsigned long j = 1; j <= maxRatio - i + 1; j++)
			{
				for (unsigned long k = 1; k <= maxRatio - i - j + 2; k++)
				{
					for (unsigned long l = 1; l <= maxRatio - i - j - k + 3; l++)
					{
						unsigned long m = ulMax - i - j - k - l;
						ulCount++;
					}

				}
			}
		}

		return ulCount;
	}

	static unsigned long  Splite(unsigned long ulElementCount, unsigned long ulCount)
	{
		switch (ulElementCount)
		{
		case 3:
			return Splite_3(ulCount);
			break;
		case 4:
			return Splite_4(ulCount);
			break;
		case 5:
			return Splite_5(ulCount);
			break;
		default:
			break;
		}

		return 0;
	}
};

