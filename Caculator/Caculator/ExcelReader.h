#pragma once

#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"

class CExcelReader
{
public:
	CExcelReader();
	~CExcelReader();

	HRESULT ReadFile(const wstring& strFile);
	contentArray& GetRawContents();
private:
	CApplication m_excelApplication;
	CWorksheet m_excelWorkSheet;
	CWorksheets m_excelWorkSheets;
	CWorkbook m_excelWorkBook;
	CWorkbooks m_excelWorkBooks;
	CRange m_currentRange;

	contentArray m_contentArray;
};

