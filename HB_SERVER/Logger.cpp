#include "pch.h"
#include "Logger.h"

CLogger::CLogger()
{
    CTime now = CTime::GetCurrentTime();
    m_startTime.Format(now.Format(L"%Y-%m-%d %H:%M:%S"));
    m_logPath.Format(L"logs\\log_%s.txt", now.Format(L"%Y%m%d_%H%M%S"));
    Log(L"Create log file");
}
CLogger::~CLogger()
{
}

CLogger& CLogger::GetInstance()
{
    static CLogger instance;
    return instance;
}

void CLogger::Log(const CString& message, int level)
{
    CString levelStr;

    switch (level) {
    case LOG_LEVEL_DEBUG:   levelStr = L"DEBUG"; break;
    case LOG_LEVEL_INFO:    levelStr = L"INFO"; break;
    case LOG_LEVEL_WARN:    levelStr = L"WARNING"; break;
    case LOG_LEVEL_ERROR:   levelStr = L"ERROR"; break;
    case LOG_LEVEL_CLIENT:     levelStr = L"CLIENT"; break;
    default:                levelStr = L"UNKNOWN"; break;
    }

    CTime now = CTime::GetCurrentTime();
    CString logLine;
    if (level == LOG_LEVEL_CLIENT) {
        logLine.Format(L"%s\n", message);
    }
    else {
        logLine.Format(L"[%s] [%s] %s\n", now.Format(L"%Y-%m-%d %H:%M:%S"), levelStr, message);
    }

    WriteToFile(logLine);
}

void CLogger::WriteToFile(const CString& logLine)
{

    CStdioFile file;
    if (file.Open(m_logPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeRead))
    {
        file.SeekToEnd();
        file.WriteString(logLine);
        file.Close();
    }
}

void CLogger::DisplayLog(CEdit* pEditCtrl)
{
    CStdioFile file;
    CString line;
    CString content;

    if (file.Open(m_logPath, CFile::modeRead | CFile::typeText))
    {
        while (file.ReadString(line))
        {
            line += L"\r\n";
            content += line;
        }
        file.Close();
    }

    if (pEditCtrl && ::IsWindow(pEditCtrl->GetSafeHwnd()))
    {
        pEditCtrl->SetWindowTextW(content);
    }
}

CString CLogger::GetLogFileContent()
{

    CStdioFile file;
    CString line;
    CString content;

    if (file.Open(m_logPath, CFile::modeRead | CFile::typeText))
    {
        while (file.ReadString(line))
        {
            content += line + L"\n";
        }
        file.Close();
    }

    return content;
}