#include "Database.hpp"
#include <iostream>
#include <iomanip>

#include <string.h>
using namespace std;

int main(int argc, char** argv) {

	Database	database;

	database.init("127.0.0.1", "root", "1234", "rpg_test");

	string		sql = "SELECT * FROM player";

	auto		result = database.select(sql);

	if (result.size() == 0)
	{
		cout << "Select Fail" << endl;
		return 1;
	}
    for (auto i : result)
    {
        for (auto j : i)
        {
            cout << j << "  ";
        }
        cout << endl;
    }

	sql = "UPDATE `rpg_test`.`player` SET `position_x` = '30', `speed` = '0.010' WHERE (`idplayer` = '3')";
	if (database.update(sql))
	{
		cout << "Update Success" << endl;
	}
	else
	{
		cout << "Update fail" << endl;
	}


    return 0;













    // MYSQL* conn;
    // MYSQL       connection;
    // MYSQL_RES* result;
    // MYSQL_ROW   row;

    // char DB_HOST[] = "127.0.0.1";
    // char DB_USER[] = "root";
    // char DB_PASS[] = "1234";
    // char DB_NAME[] = "rpg_test";
    // std::string sql;

    // char str1[1024], str2[1024];

    // mysql_init(&connection);

    // conn = mysql_real_connect(&connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
    // cout << conn << endl;
    // cout << &connection << endl;

    // if (!conn)
    // {
    //     std::cout << "connection failed" << std::endl;
    //     return 1;
    // }

    // // SELECT 
    // sql = "SELECT * FROM employees where emp_no=11000";
    // if (mysql_query(conn, sql.data()) == 0) {
    //     result = mysql_store_result(conn);
    //     while ((row = mysql_fetch_row(result)) != NULL) {
    //         for (int i = 0; i < result->field_count; i++)
    //         {
    //             std::cout << std::setw(20) << row[i];
    //         }
    //         std::cout << '\n';
    //     }
    //     mysql_free_result(result);
    // }
    // else {
    //     // 
    // }

    // // UPDATE
    // sql = "UPDATE `rpg_test`.`player` SET `position_x` = '30', `speed` = '0.010' WHERE (`idplayer` = '3')";
    // if (mysql_query(conn, sql.data()) == 0) {
    //     cout << "Update: Success";
    //     result = mysql_store_result(conn);
    //     while ((row = mysql_fetch_row(result)) != NULL) {
    //         for (int i = 0; i < result->field_count; i++)
    //         {
    //             std::cout << std::setw(20) << row[i];
    //         }
    //         std::cout << '\n';
    //     }
    //     mysql_free_result(result);
    // }
    // else {
    //     cout << "Update: Fail";
    //     // 
    // }


    // mysql_close(conn);

    // return 0;
}
