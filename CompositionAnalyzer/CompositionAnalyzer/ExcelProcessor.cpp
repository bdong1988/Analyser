#include "stdafx.h"
#include "ExcelProcessor.h"


CExcelProcessor::CExcelProcessor()
{
}


CExcelProcessor::~CExcelProcessor()
{
}

HRESULT CExcelProcessor::ReadFile(const wstring& strFile)
{
	try
	{
		if (!m_excelApplication.CreateDispatch(L"Excel.Application", nullptr))
		{
			return E_FAIL;
		}

		m_excelApplication.put_Visible(FALSE);
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
		for (int i = 1; i <= lUsedRowNum; i++)
		{
			for (int j = 1; j <= lUsedColNum; j++)
			{
				VARIANT varDispatch = m_currentRange.get_Item(COleVariant((long)i), COleVariant((long)j));
				currentCell.AttachDispatch(varDispatch.pdispVal, TRUE);
				VARIANT varItemName = currentCell.get_Text();
				m_contentArray[i - 1][j - 1] = varItemName.bstrVal;
			}
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
	}

	catch (const std::exception& ex)
	{
		AfxMessageBox(L"读取excel时，发生了错误");
		return E_FAIL;
	}	
	return S_OK;
}

HRESULT CExcelProcessor::WriteLog(const wstring& strFile, CResultQueue& resultQueue, vector<wstring>& elementList)
{
	try
	{
		if (!m_excelApplication.CreateDispatch(L"Excel.Application", nullptr))
		{
			return E_FAIL;
		}

		m_excelApplication.put_Visible(FALSE);
		m_excelWorkBooks.AttachDispatch(m_excelApplication.get_Workbooks(), TRUE);

		COleVariant vaTrue((short)TRUE);
		COleVariant vaFalse((short)FALSE);
		COleVariant vaOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

		LPDISPATCH lpDisp = m_excelWorkBooks.Add(_variant_t(vtMissing));

		m_excelWorkBook.AttachDispatch(lpDisp, TRUE);

		m_excelWorkSheet.AttachDispatch(m_excelWorkBook.get_ActiveSheet(), TRUE);
		m_excelWorkSheet.put_Name(L"分析结果");

		m_currentRange.AttachDispatch(m_excelWorkSheet.get_Cells(), TRUE);
		int nCount = elementList.size();
		m_currentRange.put_Item(COleVariant((long)1), COleVariant((long)1), COleVariant(L"得分"));
		for (int i = 1; i <= nCount; i++)
		{
			m_currentRange.put_Item(COleVariant((long)1), COleVariant((long)i + 1), COleVariant(elementList[i-1].c_str()));
		}

		nCount = resultQueue.GetCurrent();
		for (int i = 0; i < nCount; i++)
		{
			m_currentRange.put_Item(COleVariant((long)i+2), COleVariant((long)1), COleVariant(resultQueue.m_resultQueue[i].m_lfScore));
			for (int j = 0; j < resultQueue.m_resultQueue[i].m_listElements.size(); j++)
			{
				int nElemIndex = resultQueue.m_resultQueue[i].m_listElements[j].m_nElemIndex;
				double lfRatio = (double)resultQueue.m_resultQueue[i].m_listElements[j].m_ulRatio / 1000;
				m_currentRange.put_Item(COleVariant((long)i + 2), COleVariant((long)nElemIndex+2), COleVariant(lfRatio));
			}
		}

		m_excelWorkBook.SaveAs(COleVariant(strFile.c_str()),
			_variant_t(vtMissing),
			_variant_t(vtMissing),
			_variant_t(vtMissing),
			_variant_t(vtMissing),
			_variant_t(vtMissing),
			1,
			_variant_t(vtMissing),
			_variant_t(vtMissing),
			_variant_t(vtMissing),
			_variant_t(vtMissing),
			_variant_t(vtMissing),
			_variant_t(vtMissing));
		m_excelWorkBook.Close(vaOptional, COleVariant(strFile.c_str()), vaOptional);
		m_excelWorkBooks.Close();

		m_currentRange.ReleaseDispatch();
		m_excelWorkSheet.ReleaseDispatch();
		m_excelWorkSheets.ReleaseDispatch();
		m_excelWorkBook.ReleaseDispatch();
		m_excelWorkBooks.ReleaseDispatch();
		m_excelApplication.Quit();
		m_excelApplication.ReleaseDispatch();
		
	}
	catch (const std::exception& ex)
	{
		AfxMessageBox(L"写入excel时，发生了错误");
		return E_FAIL;
	}

	return S_OK;
}

contentArray& CExcelProcessor::GetRawContents()
{
	return m_contentArray;
}
