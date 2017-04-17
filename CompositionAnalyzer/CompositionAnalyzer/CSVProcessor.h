#pragma once

#include <fstream>
using namespace std;
class CCSVProcessor
{
public:
	CCSVProcessor()
	{

	}
	~CCSVProcessor()
	{

	}

	bool OpenCSV(const wchar_t* strFileName)
	{
		m_ofStream.open(strFileName);
		m_ofStream.imbue(std::locale("chs"));
		if (!m_ofStream.good())
		{
			return false;
		}

		return true;
	}

	void CloseCSV()
	{
		m_ofStream.close();
	}

	void Write(const wstring& rowContent)
	{
		if (rowContent.empty())
		{
			return;
		}

		m_ofStream << rowContent.c_str() << endl;
	}

private:
	wofstream m_ofStream;

};

