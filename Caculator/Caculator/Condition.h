#pragma once

const unsigned long FACTOR = 1000;

struct range
{
	unsigned long low = 0;
	unsigned long high = 0;
};

struct element
{
	element(int nIndex, unsigned long ulContent, bool bFixed):
		m_nIndex(nIndex),
		m_ulContent(ulContent),
		m_bfixed(bFixed)
	{

	}
	int m_nIndex = -1;
	unsigned long m_ulContent = 0;
	bool m_bfixed = false;
};

using contentList = vector<element>;


class CCondition
{
public:
	CCondition();
	~CCondition();

	void SetName(const wstring& strName);
	void SetAverage(unsigned long ulAverage);
	void SetRange(unsigned long ulLow, unsigned long ulHigh);
	void AddContent(int nIndex, unsigned long dwContent, bool bFixed);
	unsigned long GetD() { return m_ulAverage; }
	void CalculateH(unsigned long ulSumD);
	void SetGroupTotalScore(double lfScore);
	double CalculateE(const int nElemCount, const unsigned long* pUlRatioList, const unsigned long* pUlContentIndexList);
private:
	wstring m_strCondtionName;
	range m_range;
	contentList m_contentList;
	unsigned long m_ulAverage = 0;
	int m_dwGroupIndex = -1;
	double m_lfH = 0;
	double m_lfGroupTotalScore = 0;
};

using CConditionPtr = std::shared_ptr<CCondition>;

