#pragma once
#include "AnalyzerException.h"

class CUtility
{
public:
	CUtility()
	{

	}
	~CUtility()
	{

	}

	static unsigned long GetNumFromString(const wstring& str)
	{
		int nIndex = str.find(L'.');
		if (nIndex < 0)
		{
			return _wtol(str.c_str())*1000;
		}
		
		int nLength = str.length();

		wstring strInteger = str.substr(0, nIndex);
		wstring strDecimal = str.substr(nIndex + 1, nLength - nIndex - 1);
		wstring strAddional;
		switch (nLength - nIndex - 1)
		{
		case 0:
			strAddional = L"000";
			break;
		case 1:
			strAddional = L"00";
			break;
		case 2:
			strAddional = L"0";
			break;
		default:
			break;
		}
		if (nLength - nIndex > 2)
		{
			strDecimal = strDecimal.substr(0, 3);
		}

		wstring strNew = strInteger + strDecimal + strAddional;
		
		return _wtol(strNew.c_str());
	}
};

