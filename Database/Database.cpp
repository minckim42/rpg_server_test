#include "Database.hpp"
#include <iostream>
#include <sstream>
using namespace std;

/*##############################################################################

	Database

##############################################################################*/
/*---------------------------------
	Constructor & Destructor
---------------------------------*/	
Database::~Database()
{
	mysql_close(&connection);
}

//------------------------------------------------------------------------------
void		Database::init(
	const string&	host,
	const string&	user,
	const string&	password,
	const string&	db_name,
	int				port
)
{
	mysql_init(&connection);
	if (mysql_real_connect(
		&connection,
		host.data(),
		user.data(),
		password.data(),
		db_name.data(),
		port,
		(char*)NULL, 0) == 0
	)
	{
		throw string("Error: database connection");
	}
}
//------------------------------------------------------------------------------
vector<vector<string>>	Database::select(const string& sql)
{
	vector<vector<string>>		result;
	if (mysql_query(&connection, sql.data()) == 0)
	{
		MYSQL_RES*	sql_result = mysql_store_result(&connection);
		MYSQL_ROW	row;
		result.reserve(sql_result->field_count);
		while ((row = mysql_fetch_row(sql_result)))
		{
			result.emplace_back();
			result.back().reserve(sql_result->field_count);
			for (int i = 0; i < sql_result->field_count; i++)
			{
				result.back().emplace_back(row[i]);
			}
		}
		mysql_free_result(sql_result);
		return result;
	}
	else
	{
		return vector<vector<string>>();
	}
}
//------------------------------------------------------------------------------
bool					Database::update(const string& sql)
{
	if (mysql_query(&connection, sql.data()) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}