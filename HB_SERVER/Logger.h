#pragma once

// 로그 레벨 상수
enum LogLevel {
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_CLIENT
};

class CLogger
{
private:
	CLogger();
	~CLogger();

	void WriteToFile(const CString& logLine);

	CString m_logPath;
public:
	CString m_startTime;

	static CLogger& GetInstance();
	
	void Log(const CString& message, int level = LOG_LEVEL_DEBUG);
	void DisplayLog(CEdit* pEditCtrl);
	CString GetLogFileContent();
};