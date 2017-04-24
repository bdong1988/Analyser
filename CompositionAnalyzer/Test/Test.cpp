// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Result.h"
#include <stdlib.h>
#include <string>
#include <locale>
#include <codecvt>
#include "../CompositionAnalyzer/CSVProcessor.h"
#include "../CompositionAnalyzer/Utility.h"


int main()
{
	unsigned long dwContent = CUtility::GetNumFromString(L"123");
	dwContent = CUtility::GetNumFromString(L"12344");
	dwContent = CUtility::GetNumFromString(L"123.1");
	dwContent = CUtility::GetNumFromString(L"123.20");
	dwContent = CUtility::GetNumFromString(L"123.234");
	dwContent = CUtility::GetNumFromString(L"123.234456");
    return 0;
}

