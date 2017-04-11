#pragma once

const static DWORD FACTOR = 1000;

struct range
{
	DWORD low = 0;
	DWORD high = 0;
};

using contentMap = map<wstring, DWORD>;

class CCondition
{
public:
	CCondition();
	~CCondition();


	void SetName(const wstring& strName);
	void SetAverage(DWORD dwAverage);
	void SetRange(DWORD dwLow, DWORD dwHigh);
	void AddContent(const wstring& strName, DWORD dwContent);

private:
	wstring m_strCondtionName;
	range m_range;
	contentMap m_contentMap;
	DWORD m_dwAverage = 0;
	int m_dwGroupIndex = -1;
};

using CConditionPtr = std::shared_ptr<CCondition>;

