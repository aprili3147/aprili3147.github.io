#pragma once

// 로그 레벨 상수
enum LogLevel {
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
};

class CLogger
{
private:
	CLogger();
	~CLogger();

	CString m_logPath;
	CEdit* m_pLogEdit = nullptr;

	void WriteToFile(const CString& logLine);
	void WriteToEdit(const CString& logLine);

public:
	CString m_startTime;

	static CLogger& GetInstance();
	void SetEditControl(CEdit* pEdit);
	void Log(const CString& message, int level = LOG_LEVEL_DEBUG); //기본값 = [Debug]
	void DisplayLog(CEdit* pEditCtrl);
	CString GetLogFileContent(); //로그 파일 내용 가져오기
};