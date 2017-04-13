#pragma once

const static unsigned long FACTOR = 1000;

struct range
{
	unsigned long low = 0;
	unsigned long high = 0;
};

struct element
{
	element(const wstring& strName, unsigned long ulContent, bool bFixed)
		:name(strName),
		content(ulContent),
		fixed(bFixed)
	{

	}
	wstring name;
	unsigned long content = 0;
	bool fixed = false;
};

using contentMap = map<wstring, element>;


class CCondition
{
public:
	CCondition();
	~CCondition();


	void SetName(const wstring& strName);
	void SetAverage(unsigned long dwAverage);
	void SetRange(unsigned long dwLow, unsigned long dwHigh);
	void AddContent(const wstring& strElementName, unsigned long dwContent, bool bFixed);
	unsigned long GetD() { return m_ulAverage; }

	double CalculateE(unsigned long ulGroupTotalScore, unsigned long ulSumD, int nElemCount, unsigned long* pUlRatioList, unsigned long* pUlContentList);
private:
	wstring m_strCondtionName;
	range m_range;
	contentMap m_contentMap;
	unsigned long m_ulAverage = 0;
	int m_dwGroupIndex = -1;
};

using CConditionPtr = std::shared_ptr<CCondition>;

