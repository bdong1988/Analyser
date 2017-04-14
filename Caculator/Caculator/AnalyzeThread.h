#pragma once

class CAnalyzeThread
{
public:
	CAnalyzeThread(CAnalyzer* pAnalyzer, int nElementCount, int nSelectionCount)
		:m_pAnalyzer(pAnalyzer),
		m_nElementCount(nElementCount),
		m_nSelectionCount(nSelectionCount)
	{
		m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, nullptr);
		m_hRunEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		m_hStopEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}
	~CAnalyzeThread()
	{
		if (m_hStopEvent)
		{
			CloseHandle(m_hStopEvent);
			m_hStopEvent = nullptr;
		}

		if (m_hRunEvent)
		{
			CloseHandle(m_hRunEvent);
			m_hRunEvent = nullptr;
		}

		if (m_hThread)
		{
			CloseHandle(m_hThread);
			m_hThread = nullptr;
		}
	}

	void Run()
	{
		HANDLE eventList[2] = {
			m_hRunEvent,
			m_hStopEvent
		};

		unsigned long long ullDataCount = 0;

		int nEventIndex = WaitForMultipleObjects(_countof(eventList), eventList, FALSE, INFINITE);
		switch (nEventIndex)
		{
		case 0:
			ullDataCount = Caculate();
			break;
		default:
			break;
		}

		m_pAnalyzer->AddDataCount(ullDataCount);
		return;
	}

	unsigned long long Caculate()
	{
		unsigned long long ullDataCount = 0;

		CCombination combination;
		combination.Calculate(m_nElementCount, m_nSelectionCount);

		combList selectionList = combination.GetResultsList();
		for (const auto& selection : selectionList)
		{
			ullDataCount += CSpliter::Calculate(m_nSelectionCount, 1000, m_pAnalyzer, selection.data());
		}

		return ullDataCount;
	}

	static DWORD WINAPI ThreadProc(PVOID pParam);

private:
	HANDLE m_hThread = nullptr;
	HANDLE m_hRunEvent = nullptr;
	HANDLE m_hStopEvent = nullptr;
	CAnalyzer* m_pAnalyzer = nullptr;
	int m_nElementCount = 0;
	int m_nSelectionCount = 0;
};

DWORD WINAPI CAnalyzeThread::ThreadProc(PVOID pParam)
{

	if (NULL != pParam)
	{
		((CAnalyzeThread*)pParam)->Run();
	}
	return 0;
}

