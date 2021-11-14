#include <mysql.h>
#include <string>
#include <vector>
/*##############################################################################

	Database

##############################################################################*/
struct Database
{
	typedef std::vector<std::vector<std::string>>	SelectData;

	MYSQL		connection;
	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/	
	~Database();

	/*---------------------------------
		Methods
	---------------------------------*/
	void		init(
		const std::string&	host,
		const std::string&	user,
		const std::string&	password,
		const std::string&	db_name,
		int					port=3306
	);

	SelectData	select(const std::string& sql);
	bool		update(const std::string& sql);
};
