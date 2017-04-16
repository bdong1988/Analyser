// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\Caculator\CSVProcessor.h"
#include <string>
#include <strsafe.h>
#include <Windows.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <memory>
#include <regex>

using namespace std;
int main()
{
	CCSVProcessor csvProcessor;
	wstring strFileName;
	strFileName.resize(1024);
	GetModuleFileName(nullptr, (LPWSTR)strFileName.data(), 1024);
	wstring::size_type pos = strFileName.rfind(L"\\");
	strFileName = strFileName.substr(0, pos + 1);
	strFileName += L"AnalyzeResult.csv";
	if (!csvProcessor.OpenCSV(strFileName.c_str()))
	{
		return 0;
	}

	//csvProcessor.Write(L"score, element 1, element 2, element 3, elemnt 4 \r\n");
	csvProcessor.Write(L"得分 , 元素  1, 元素 2, 元素 3, 元素 4 \r\n");
	csvProcessor.CloseCSV();
    return 0;
}

