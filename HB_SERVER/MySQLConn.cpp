#include "pch.h"
#include "MySQLConn.h"

using namespace cv;

MYSQL conn;

CMySQLConn::CMySQLConn()
{
    
}

CMySQLConn::~CMySQLConn()
{

}

void CMySQLConn::SetWnd(HWND hWnd)
{
    m_hWnd = hWnd;
}

bool CMySQLConn::ConnectMySQL()
{
    mysql_init(&conn);

    if (!mysql_real_connect(&conn, CON_IP, DB_USER, DB_PASS, DB_NAME, CON_PORT, NULL, 0)) {
        AfxMessageBox(L"Failed to connect DB!");
        CLogger::GetInstance().Log(L"Failed to connect DB!", LOG_LEVEL_ERROR);
        return false;
    }
    CLogger::GetInstance().Log(L"DB Connect!");
    return true;
}

void CMySQLConn::Disconnect()
{
    mysql_close(&conn);
}

bool CMySQLConn::InsertData(HBSData& data)
{
    if (mysql_ping(&conn) != 0) {
        AfxMessageBox(L"DB connection is disconnected. Please reconnect!");
        return false;
    }

    // BLOB 데이터를 바이너리 문자열로 변환
    std::vector<uchar> beforeBlob, targetBlob, afterBlob;
    cv::imencode(".png", data.beforeImage, beforeBlob);
    cv::imencode(".png", data.targetImage, targetBlob);
    cv::imencode(".png", data.afterImage, afterBlob);

    const char* query = "INSERT INTO hbs (before_image, target_image, after_image, log) VALUES (?, ?, ?, ?)";
    MYSQL_STMT* stmt = mysql_stmt_init(&conn);
    if (!stmt) {
        CLogger::GetInstance().Log(L"STMT creation failed", LOG_LEVEL_ERROR);
        AfxMessageBox(L"STMT creation failed");
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        CString errMsg;
        errMsg.Format(L"Prepare failed : %S", mysql_stmt_error(stmt));
        CLogger::GetInstance().Log(errMsg, LOG_LEVEL_ERROR);
        AfxMessageBox(errMsg);
        mysql_stmt_close(stmt);
        return false;
    }
    MYSQL_BIND bind[4] = { 0 };

    bind[0].buffer_type = MYSQL_TYPE_BLOB;
    bind[0].buffer = beforeBlob.data();
    bind[0].buffer_length = beforeBlob.size();

    bind[1].buffer_type = MYSQL_TYPE_BLOB;
    bind[1].buffer = targetBlob.data();
    bind[1].buffer_length = targetBlob.size();

    bind[2].buffer_type = MYSQL_TYPE_BLOB;
    bind[2].buffer = afterBlob.data();
    bind[2].buffer_length = afterBlob.size();

    CString log = CLogger::GetInstance().GetLogFileContent();
    std::string logStr(CT2A(log.GetString()));
    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = (void*)logStr.c_str();
    bind[3].buffer_length = logStr.length();

    // 바인딩 수행
    if (mysql_stmt_bind_param(stmt, bind)) {
        CLogger::GetInstance().Log(L"Binding failed!", LOG_LEVEL_ERROR);
        AfxMessageBox(L"Binding failed!");
        mysql_stmt_close(stmt);
        return false;
    }

    if (mysql_stmt_execute(stmt)) {
        CString errMsg;
        errMsg.Format(L"INSERT execution failed : %S", mysql_stmt_error(stmt));

        CString debugMsg;
        debugMsg.Format(L"%s\nBlob sizes: before=%d, target=%d, after=%d",
            errMsg, beforeBlob.size(), targetBlob.size(), afterBlob.size());

        CLogger::GetInstance().Log(debugMsg, LOG_LEVEL_ERROR);

        AfxMessageBox(debugMsg);

        mysql_stmt_close(stmt);
        return false;
    }

    mysql_stmt_close(stmt);
    return true;
}

bool CMySQLConn::DeleteData(int id)
{
    CString query;
    query.Format(L"DELETE FROM hbs WHERE id=%d", id);
    //바인드 형식으로 바꾸기

    if (mysql_query(&conn, CT2A(query))) {
        CLogger::GetInstance().Log(L"DELETE failed!", LOG_LEVEL_ERROR);
        AfxMessageBox(L"DELETE failed!");
        return false;
    }

    CString logStr;
    logStr.Format(L"DELETE data id : %d", id);
    CLogger::GetInstance().Log(logStr);

    SendMessage(m_hWnd, WM_UPDATE_DB, 0, 0);
    return true;
}

    std::vector<HBSData> CMySQLConn::FindAll()
    {
        std::vector<HBSData> dataList;

        CString query;
        query.Format(L"SELECT * FROM hbs");

        if (mysql_query(&conn, CT2A(query))) {
            CLogger::GetInstance().Log(L"Failed to execute FindAll query!", LOG_LEVEL_ERROR);
            AfxMessageBox(L"Failed to execute FindAll query!");
            //mysql_close(&conn);
            return dataList;
        }

        MYSQL_RES* res = mysql_store_result(&conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res))) {
            unsigned long* lengths = mysql_fetch_lengths(res);

            HBSData data;

            data.id = std::atoi(row[0]);

            CString timeStr(row[1]);
        
            std::vector<uchar> beforeBlob(row[2], row[2] + lengths[2]);
            std::vector<uchar> targetBlob(row[3], row[3] + lengths[3]);
            std::vector<uchar> afterBlob(row[4], row[4] + lengths[4]);

            CString logStr(row[5]);

            data.createdAt = timeStr;
            data.beforeImage = cv::imdecode(beforeBlob, IMREAD_COLOR);
            data.targetImage = cv::imdecode(targetBlob, IMREAD_COLOR);
            data.afterImage = cv::imdecode(afterBlob, IMREAD_COLOR);
            data.log = logStr;

            dataList.push_back(data);
        }

        mysql_free_result(res);
        return dataList;
    }

HBSData CMySQLConn::FindOne(int id)
{
    HBSData data;

    CString query;
    query.Format(L"SELECT * FROM hbs Where id=%d", id);
    //바인드 형식으로 바꾸기

    if (mysql_query(&conn, CT2A(query))) {
        CLogger::GetInstance().Log(L"Failed to execute FindOne query!", LOG_LEVEL_ERROR);
        AfxMessageBox(L"Failed to execute FindOne query!");
        //mysql_close(&conn);
        return data;
    }

    MYSQL_RES* res = mysql_store_result(&conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (row) {
        unsigned long* lengths = mysql_fetch_lengths(res);

        data.id = std::stoi(row[0]);

        CString timeStr(row[1]);

        std::vector<uchar> beforeBlob(row[2], row[2] + lengths[2]);
        std::vector<uchar> targetBlob(row[3], row[3] + lengths[3]);
        std::vector<uchar> afterBlob(row[4], row[4] + lengths[4]);

        CString logStr(row[5]);

        data.createdAt = timeStr;
        data.beforeImage = cv::imdecode(beforeBlob, IMREAD_COLOR);
        data.targetImage = cv::imdecode(targetBlob, IMREAD_COLOR);
        data.afterImage = cv::imdecode(afterBlob, IMREAD_COLOR);
        data.log = logStr;
    }

    mysql_free_result(res);
    return data;
}