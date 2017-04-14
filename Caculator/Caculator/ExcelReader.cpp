#include "stdafx.h"
#include "ExcelReader.h"


CExcelReader::CExcelReader()
{
}


CExcelReader::~CExcelReader()
{
}

HRESULT CExcelReader::ReadFile(const wstring& strFile)
{
	if (!m_excelApplication.CreateDispatch(L"Excel.Application", nullptr))
	{
		return E_FAIL;
	}

	m_excelApplication.put_Visible(TRUE);
	m_excelWorkBooks.AttachDispatch(m_excelApplication.get_Workbooks(), TRUE);

	COleVariant vaTrue((short)TRUE);
	COleVariant vaFalse((short)FALSE);
	COleVariant vaOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


	LPDISPATCH lpDisp = m_excelWorkBooks.Open(strFile.c_str(),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing),
		_variant_t(vtMissing)
	);

	m_excelWorkBook.AttachDispatch(lpDisp, TRUE);

	m_excelWorkSheet.AttachDispatch(m_excelWorkBook.get_ActiveSheet(), TRUE);

	m_currentRange.AttachDispatch(m_excelWorkSheet.get_UsedRange(), TRUE);

	long lUsedRowNum{ 0 };
	long lUsedColNum{ 0 };

	m_currentRange.AttachDispatch(m_currentRange.get_Rows(), TRUE);
	lUsedRowNum = m_currentRange.get_Count();

	m_currentRange.AttachDispatch(m_currentRange.get_Columns(), TRUE);
	lUsedColNum = m_currentRange.get_Count();
	
	m_contentArray.resize(lUsedRowNum);
	for (auto& subArray : m_contentArray)
	{
		subArray.resize(lUsedColNum);
	}
	m_currentRange.AttachDispatch(m_excelWorkSheet.get_Cells(), TRUE);

	wstring strSheetName = m_excelWorkBook.get_Name();
	CRange currentCell;
	try
	{
		for (int i = 1; i <= lUsedRowNum; i++)
		{
			for (int j = 1; j <= lUsedColNum; j++)
			{
				VARIANT varDispatch = m_currentRange.get_Item(COleVariant((long)i), COleVariant((long)j));
				currentCell.AttachDispatch(varDispatch.pdispVal, TRUE);
				VARIANT varItemName = currentCell.get_Text();
				m_contentArray[i-1][j-1] = varItemName.bstrVal;
			}
		}
	}
	catch (const std::exception& ex)
	{
		string strException  = ex.what();
		OutputDebugStringA(strException.c_str());
	}


	m_excelWorkBook.Close(vaOptional, COleVariant(strFile.c_str()), vaOptional);
	m_excelWorkBooks.Close();

	m_currentRange.ReleaseDispatch();
	m_excelWorkSheet.ReleaseDispatch();
	m_excelWorkSheets.ReleaseDispatch();
	m_excelWorkBook.ReleaseDispatch();
	m_excelWorkBooks.ReleaseDispatch();
	m_excelApplication.Quit();
	m_excelApplication.ReleaseDispatch();
	


	return S_OK;
}

contentArray& CExcelReader::GetRawContents()
{
	return m_contentArray;
}
