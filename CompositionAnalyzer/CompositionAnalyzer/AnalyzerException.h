#pragma once

class CAnalyzeException :exception
{
public:
	CAnalyzeException(const string& strExcDetail)
		:m_strExcDetail(strExcDetail)
	{

	}
	~CAnalyzeException()
	{

	}


	virtual char const* what() const
	{
		return m_strExcDetail.c_str();
	}

private:
	string m_strExcDetail;

};
