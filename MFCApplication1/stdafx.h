#include "mysql.h"
#include <WinSock2.h>

#pragma comment (lib, "libmysql.lib")
#pragma comment (lib, "ws2_32.lib")

#define CON_IP "127.0.0.1:3306"
#define DB_USER "root"
#define DB_PASS "infonet"
#define DB_NAME "hbs"

extern MYSQL Connect;
extern MYSQL_RES* sql_result;
extern MYSQL_ROW sql_row;