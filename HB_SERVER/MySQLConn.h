#pragma once

#define CON_IP "127.0.0.1"
#define CON_PORT 3306
#define DB_USER "root"
#define DB_PASS "0886"
#define DB_NAME "hbs"

#define WM_UPDATE_DB WM_USER+4

struct HBSData
{
	int id;
	cv::Mat targetImage;
	cv::Mat beforeImage;
	cv::Mat afterImage;
	CString createdAt;
	CString log;
};

class CMySQLConn
{
private:

protected:
	
public:
	CMySQLConn();
	~CMySQLConn();

	HWND m_hWnd;
	void SetWnd(HWND hWnd);
	bool ConnectMySQL();
	void Disconnect();
	bool InsertData(HBSData& data);
	bool DeleteData(int id);
	std::vector<HBSData> FindAll();
	HBSData FindOne(int id);
	cv::Mat ConvertToMat(const std::vector<unsigned char>& blobData);
};

