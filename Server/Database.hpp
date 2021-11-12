#include <mysql.h>
#include <string>
#include <vector>
struct Database
{
	MYSQL		connection;

	void		init(
		const std::string&	host,
		const std::string&	user,
		const std::string&	password,
		const std::string&	db_name,
		int					port=3306
	);

	std::vector<std::vector<std::string>>	select(const std::string& sql);
};
