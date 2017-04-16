#pragma once

class CCritSec
{
public:
	CCritSec()
	{
		InitializeCriticalSection(&m_cs);
	}

	~CCritSec()
	{
		DeleteCriticalSection(&m_cs);
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void UnLock()
	{
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs = {0};
};

class CLock
{
public:
	CLock(CCritSec* pCS)
		:m_pCS(pCS)
	{
		m_pCS->Lock();
	}
	~CLock()
	{
		m_pCS->UnLock();
	}

private:
	CCritSec* m_pCS = nullptr;
};