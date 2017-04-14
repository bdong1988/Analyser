#pragma once
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
		float fVal = _wtof(str.c_str());

		int nIndex = str.find(L'.');
		if (nIndex < 0)
		{
			return _wtol(str.c_str());
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

		wstring strNew = strInteger + strDecimal + strAddional;
		
		return _wtol(strNew.c_str());
	}
};

