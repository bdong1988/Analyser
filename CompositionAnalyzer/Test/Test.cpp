// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Result.h"
#include <stdlib.h>

CResultQueue queue;

void AssertQueue()
{
	for (size_t i = 0; i < queue.m_nCurrentIndex - 1; i++)
	{
		if (queue.m_resultQueue[i] < queue.m_resultQueue[i + 1])
		{
			_ASSERT(0);
		}
	}
}
int main()
{
	int nMax = 0;
	for (size_t i = 0; i < 40000; i++)
	{
		int n = rand();
		if (n > nMax)
		{
			nMax = n;
		}
		queue.PushResult(n);
	}

	AssertQueue();
    return 0;
}

