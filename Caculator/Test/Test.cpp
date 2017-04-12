// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Caculator\RatioCaculator.h"
#include "..\Caculator\Utility.h"

int main()
{
	CRatioCaculator caculator;
	caculator.Caculate(5, 1000);

	int n = CUtility::GetNumFromString(L"20.496");
	int n2 = CUtility::GetNumFromString(L"20.49");
	int n3 = CUtility::GetNumFromString(L"20.4");
	int n4 = CUtility::GetNumFromString(L"20.");
    return 0;
}

