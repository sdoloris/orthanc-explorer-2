using namespace std;
#include "../include/database.hpp"
#include "../include/picosha2.h"




/*CONSTRUCTOR*/


DatabaseOE::DatabaseOE(const string pPath)
{
	this->path = pPath;
}

/*GENERAL*/

void DatabaseOE::setPath(const string pPath)
{
	this->path = pPath;
}


int DatabaseOE::open()
{
	int SQLError = sqlite3_open(path.c_str(), &db);
	if( SQLError != SQLITE_OK)
		return SQLError;
	return init();
}


int DatabaseOE::close()
{
	if(sqlite3_close(db)!= SQLITE_OK)
		return CONST_DB_OE_SQLERR;
	return CONST_DB_OE_OK;
}


vector<string> DatabaseOE::retrieveText(string query, vector<string> args)
{

	vector<string> vec;
	sqlite3_stmt *res;
	const char *tail;
	if(sqlite3_prepare_v2(db, query.c_str(), query.size(), &res, &tail) == SQLITE_OK)
  {
  	for(int i = 0; i < args.size(); i++)
  		if(sqlite3_bind_text(res, i+1, args[i].c_str(), args[i].size(), NULL) != SQLITE_OK)
  		{
				sqlite3_finalize(res);
				return vec;  			
  		}
		while (sqlite3_step(res) == SQLITE_ROW) 
			vec.push_back(reinterpret_cast<const char*>(sqlite3_column_text(res, 0)));			
  }
	sqlite3_finalize(res);
	return vec;
}


vector<OE2Permission> DatabaseOE::retrieveOE2Permission(string query, vector<string> args)
{
	vector<OE2Permission> permissionList;
	sqlite3_stmt *res;
	const char *tail;
	if(sqlite3_prepare_v2(db, query.c_str(), query.size(), &res, &tail) == SQLITE_OK)
  {
  	for(int i = 0; i < args.size(); i++)
  	{
  		if(sqlite3_bind_text(res, i+1, args[i].c_str(), args[i].size(), NULL) != SQLITE_OK)
  		{
				sqlite3_finalize(res);
				return permissionList;  			
  		}
  	}
		while (sqlite3_step(res) == SQLITE_ROW) 
		{ 
			permissionList.push_back((OE2Permission) sqlite3_column_int(res, 0));			
		}
  }
	sqlite3_finalize(res);
	return permissionList;
}


int DatabaseOE::retrieveInt(string query, vector<string> args)
{
	int result = -1;
	sqlite3_stmt *res;
		const char *tail;
	if((sqlite3_prepare_v2(db, query.c_str(), query.size(), &res, &tail) == SQLITE_OK))
		for(int i = 0; i < args.size(); i++)
  		if(sqlite3_bind_text(res, i+1, args[i].c_str(), args[i].size(), NULL) != SQLITE_OK)
  		{
				sqlite3_finalize(res);
				return result;  			
  		}
	if(sqlite3_step(res) == SQLITE_ROW)
		result = sqlite3_column_int(res, 0);
	sqlite3_finalize(res);
	return result;
}


int DatabaseOE::displayRes(string query, vector<string> args)
{
	result = CONST_DB_OE_OK;
	sqlite3_stmt *res;
	const char *tail;
	if(sqlite3_prepare_v2(db, query.c_str(), query.size(), &res, &tail) == SQLITE_OK)
  {
  	for(int i = 0; i < args.size(); i++)
  		if(sqlite3_bind_text(res, i+1, args[i].c_str(), args[i].size(), NULL) != SQLITE_OK)
  		{
				sqlite3_finalize(res);
				return result;
  		}
		while (sqlite3_step(res) == SQLITE_ROW) 
		{ 
			for(int i=0; i < sqlite3_column_count(res); i++)
				if(sqlite3_column_type(res, i) == SQLITE_INTEGER)	
					cout << sqlite3_column_int(res, i) << ' ';
				else if(sqlite3_column_type(res, i) == SQLITE3_TEXT)
					cout << sqlite3_column_text(res, i) << ' ';
			cout << endl;
		}
  }
	else
		result = CONST_DB_OE_SQLERR;
	sqlite3_finalize(res);
	return result;
}


bool DatabaseOE::existInTable(string query, vector<string> args)
{
	bool boolResult = false;
	sqlite3_stmt *res;
		const char *tail;
	if(sqlite3_prepare_v2(db, query.c_str(), query.size(), &res, &tail) != SQLITE_OK)
		return boolResult;
	for(int i = 0; i < args.size(); i++)
	{
		if(sqlite3_bind_text(res, i+1, args[i].c_str(), args[i].size(), NULL) != SQLITE_OK)
		{
			sqlite3_finalize(res);
			return boolResult;  			
		}
	}
	if(sqlite3_step(res) == SQLITE_ROW)
		boolResult = true;
	
	sqlite3_finalize(res);
	return boolResult;
}


int DatabaseOE::modifyDatabase(string query, vector<string> args)
{
	result = CONST_DB_OE_OK;
	sqlite3_stmt *res;
	const char *tail;
	if(sqlite3_prepare_v2(db, query.c_str(), query.size(), &res, &tail) != SQLITE_OK)
		result = CONST_DB_OE_SQLERR;
	for(int i = 0; i < args.size(); i++)
		if(sqlite3_bind_text(res, i+1, args[i].c_str(), args[i].size(), NULL) != SQLITE_OK)
		{
			result = CONST_DB_OE_SQLERR;
			sqlite3_finalize(res);
			return result;
		}
	if(sqlite3_step(res) != SQLITE_DONE)
		result = CONST_DB_OE_SQLERR;
	sqlite3_finalize(res);
	return result;
}


bool DatabaseOE::existTable(string tableName)
{
	vector <string> args;
	args.push_back(tableName);
	return existInTable("SELECT * FROM sqlite_master WHERE name = ?1;", args);
}


int DatabaseOE::init()
{
	vector <string> args;
	/*Users*/  
	
	if(modifyDatabase("CREATE TABLE Users(id_user INTEGER PRIMARY KEY AUTOINCREMENT, username VARCHAR(255) NOT NULL UNIQUE, password TEXT NOT NULL);", args) == CONST_DB_OE_OK)
		dropTable("UserPermissions");
	
	/*UserPermissions*/
	modifyDatabase("CREATE TABLE UserPermissions(id_user INTEGER NOT NULL, permission INTEGER NOT NULL, CONSTRAINT Unique_pair UNIQUE (id_user,permission));", args);
		//addRoot();	
	/*Tags*/
	if(modifyDatabase("CREATE TABLE Tags(id_tag INTEGER PRIMARY KEY AUTOINCREMENT, tag VARCHAR(255) NOT NULL UNIQUE);", args) == CONST_DB_OE_OK)
		dropTable("StudyTags");

	/*StudyTag*/
	modifyDatabase("CREATE TABLE StudyTags(id_tag INTEGER, id_study VARCHAR(255), CONSTRAINT Unique_pair UNIQUE (id_tag, id_study));", args);
	
	/*test if all the table are set*/
	
	if(existTable("Users") && 
		existTable("UserPermissions") &&
		existTable("Tags") &&
		existTable("StudyTags"))
		return CONST_DB_OE_OK;
	return CONST_DB_OE_SQLERR;
}


int DatabaseOE::dropTable(string tableName)
{
	vector <string> args;
	if(!existTable(tableName))
		return CONST_DB_OE_TABLE_NOT_EXIST;
	if(tableName == "Tags")
	{
		if(modifyDatabase("DROP TABLE IF EXISTS StudyTags;", args) == CONST_DB_OE_SQLERR)
		return CONST_DB_OE_SQLERR;
	}
	else if(tableName == "Users")
	{
		if(modifyDatabase("DROP TABLE IF EXISTS UserPermissions;", args) == CONST_DB_OE_SQLERR)
		return CONST_DB_OE_SQLERR;
	}
	args.push_back(tableName);
	return modifyDatabase("DROP TABLE IF EXISTS ?1;", args);
}


bool DatabaseOE::dropDatabase()
{
	if(remove(path.c_str()) == 0)
		return true;
	return false;
}


int DatabaseOE::readWholeTable(string tableName)
{
	if(!existTable(tableName))
		return CONST_DB_OE_TABLE_NOT_EXIST;
	
	vector <string> args;
	args.push_back(tableName);
	return displayRes("SELECT * FROM ?1;", args);
}

/*

int DatabaseOE::addRoot()
{
	result = addUser("root", "root");
	if(result != CONST_DB_OE_OK)
		return result;
	return addAllUserPermissions(retrieveUserId("root"));
}

*/


/*USERS*/


vector<string> DatabaseOE::getUsers()
{
	vector <string> args;
	string query = "SELECT username FROM Users;";
	return retrieveText(query, args); 
}


bool DatabaseOE::existUserPassword(int id_user, string password)
{
	string hashedPassword = picosha2::hash256_hex_string(password);
	vector <string> args;
	args.push_back(to_string(id_user));
	args.push_back(hashedPassword);
	return existInTable("SELECT * FROM Users WHERE id_user =?1 AND password =?2;", args);
}


bool DatabaseOE::existUser(int id_user)
{
	vector <string> args;
	args.push_back(to_string(id_user));
	return existInTable("SELECT * FROM Users WHERE id_user =?1;", args);
}


bool DatabaseOE::existUserString(string username)
{
	vector <string> args;
	args.push_back(username);
	return existInTable("SELECT * FROM Users WHERE username =?1;", args);
}


bool DatabaseOE::isLastSuperUser(int id_user)
{
	vector <string> args;
	args.push_back(to_string(OE2Permission_SuperUser));
	string query = "SELECT COUNT(*) FROM UserPermissions WHERE permission = ?1;";
	if((isSuperUser(id_user)) && 
		(1 == retrieveInt(query, args)))
		return true;
	return false;
}


int DatabaseOE::retrieveUserId(string username)
{	
/*
	string query = "SELECT id_user FROM Users WHERE username = ?1;";
	int result = -1;
	sqlite3_stmt *res;
	const char *tail;
	if((sqlite3_prepare_v2(db, query.c_str(), query.size(), &res, &tail) == SQLITE_OK) &&
		 (sqlite3_bind_text(res, 1, username.c_str(), username.size(), NULL) == SQLITE_OK) &&
		 (sqlite3_step(res) == SQLITE_ROW))
		result = sqlite3_column_int(res, 0);
	sqlite3_finalize(res);
	return result;
*/
	vector <string> args;
	args.push_back(username);
	return retrieveInt("SELECT id_user FROM Users WHERE username = ?1;", args);
}


string DatabaseOE::retrieveUsername(int id_user)
{
	vector <string> args;
	args.push_back(to_string(id_user));
	string query = "SELECT username FROM Users WHERE id_user =?1;";
	return retrieveText(query, args)[0];
}


int DatabaseOE::addUser(string username, string password)
{
	if(existUserString(username))
		return CONST_DB_OE_USERNAME_ALREADY_USED;
	string hashedPassword = picosha2::hash256_hex_string(password);
	vector <string> args;
	args.push_back(username);
	args.push_back(hashedPassword);
	return modifyDatabase("INSERT INTO Users (username, password) VALUES (?1, ?2);", args);
}


int DatabaseOE::modifyUser(string username, string nUsername, string nPassword)
{
	
	if(!existUserString(username))
		return CONST_DB_OE_USER_NOT_EXIST;
	if((username != nUsername) && (existUserString(nUsername)))
		return CONST_DB_OE_USERNAME_ALREADY_USED;
	string hashedPassword = picosha2::hash256_hex_string(nPassword);
	vector <string> args;
	args.push_back(nUsername);
	args.push_back(hashedPassword);
	args.push_back(to_string(retrieveUserId(username)));
	return modifyDatabase("UPDATE Users SET username = ?1, password = ?2 WHERE id_user = ?3;", args);
}


int DatabaseOE::readUserData(int id_user)
{
	vector <string> args;
	args.push_back(to_string(id_user));
	return displayRes("SELECT * FROM Users WHERE id_user = ?1 ;", args);
}


int DatabaseOE::removeUser(int id_user)
{
	vector <string> args;
	args.push_back(to_string(id_user));
	if(!existUser(id_user))
		return CONST_DB_OE_USER_NOT_EXIST;
	if(isLastSuperUser(id_user))
		return CONST_DB_OE_LAST_SUPERUSER;
	if((removeAllUserPermissions(id_user) == CONST_DB_OE_OK) &&
		(modifyDatabase("DELETE FROM Users WHERE id_user = ?1;", args) == CONST_DB_OE_OK))
		return CONST_DB_OE_OK;
	return CONST_DB_OE_SQLERR;
}  


/*USERPERMISSIONS*/


int DatabaseOE::addUserPermission(int id_user, OE2Permission permission)
{
	if(!existUser(id_user))
		return CONST_DB_OE_USER_NOT_EXIST;
	if(existUserPermission(id_user, permission))
		return CONST_DB_OE_USERPERMISSION_ALREADY_EXIST;

	vector <string> args;
	args.push_back(to_string(id_user));
	args.push_back(to_string(permission));
	return modifyDatabase("INSERT INTO UserPermissions (id_user, permission) VALUES (?1, ?2);", args);
}


bool DatabaseOE::existUserPermission(int id_user, OE2Permission permission)
{
	vector <string> args;
	args.push_back(to_string(id_user));
	args.push_back(to_string(permission));
	return existInTable("SELECT * FROM UserPermissions WHERE id_user =?1 AND permission =?2;", args);
}


int DatabaseOE::removeUserPermission(int id_user, OE2Permission permission)
{
	if(!existUser(id_user))
		return CONST_DB_OE_USER_NOT_EXIST;
	if(!existUserPermission(id_user, permission))
		return CONST_DB_OE_USERPERMISSION_NOT_EXIST;
	if((permission == OE2Permission_SuperUser) && isLastSuperUser(id_user))
		return CONST_DB_OE_LAST_SUPERUSER;
	vector <string> args;
	args.push_back(to_string(id_user));
	args.push_back(to_string(permission));
	return modifyDatabase("DELETE FROM UserPermissions WHERE id_user = ?1 AND permission = ?2;", args);
}


vector<OE2Permission> DatabaseOE::retrieveUserPermission(int id_user)
{
	vector <string> args;
	args.push_back(to_string(id_user));
	string query = "SELECT permission FROM UserPermissions WHERE id_user =?1 ORDER BY permission;";
	return retrieveOE2Permission(query, args);
}


int DatabaseOE::displayUserPermission(int id_user)
{
	vector <string> args;
	args.push_back(to_string(id_user));
	string query = "SELECT permission FROM UserPermissions WHERE id_user =?1;";
	return displayRes(query, args);
}


/*
int DatabaseOE::addAllUserPermissions(int id_user)
{
	int i = 1;
	result = CONST_DB_OE_OK;
	while(((result == CONST_DB_OE_OK) || (result == CONST_DB_OE_USERPERMISSION_ALREADY_EXIST)) && 
		(i <= CONST_OE_NB_PERMISSION))
	{
		result = addUserPermission(id_user, (OE2Permission) i);
		i++;
	}
	return result;
}
*/


bool DatabaseOE::isSuperUser(int id_user)
{
	return existUserPermission(id_user, OE2Permission_SuperUser);	
}


bool DatabaseOE:: isAllowedToDelete(int id_user)
{
	return existUserPermission(id_user, OE2Permission_Delete);	
}


bool DatabaseOE:: isAllowedToRead(int id_user)
{
	return existUserPermission(id_user, OE2Permission_Read);	
}


bool DatabaseOE:: isAllowedToWrite(int id_user)
{
	return existUserPermission(id_user, OE2Permission_Write);
}


int DatabaseOE::removeAllUserPermissions(int id_user)
{

	if(!existUser(id_user))
		return CONST_DB_OE_USER_NOT_EXIST;
	if(isLastSuperUser(id_user))
		return CONST_DB_OE_LAST_SUPERUSER;
	vector <string> args;
	args.push_back(to_string(id_user));
	return modifyDatabase("DELETE FROM UserPermissions WHERE id_user = ?1;", args);	
}


/*TAGS*/


bool DatabaseOE::existTag(string tag)
{
	vector <string> args;
	args.push_back(tag);
	return existInTable("SELECT * FROM Tags WHERE tag =?1;", args);
}


int DatabaseOE::retrieveTagId(string tag)
{
	vector <string> args;
	args.push_back(tag);
	return retrieveInt("SELECT id_tag FROM Tags WHERE tag =?1;", args);
}


vector<string> DatabaseOE::retrieveTagList()
{
	vector <string> args;
	string query = "SELECT tag FROM Tags ORDER BY tag;";
	return retrieveText(query, args);
}


int DatabaseOE::addTag(string tag)
{
	if(existTag(tag))
		return CONST_DB_OE_TAG_ALREADY_EXIST;
	vector <string> args;
	args.push_back(tag);
	return modifyDatabase("INSERT INTO Tags (tag) VALUES (?1);", args);
}


int DatabaseOE::modifyTag(string tag, string nTag)
{
	if(!existTag(tag))
		return CONST_DB_OE_TAG_NOT_EXIST;
	if(((tag != nTag) && existTag(nTag)) || (tag == nTag) )
		return CONST_DB_OE_TAG_ALREADY_EXIST;
	vector <string> args;
	args.push_back(nTag);
	args.push_back(tag);
	return	modifyDatabase("UPDATE Tags SET tag = ?1 WHERE tag =?2;", args);
}


int DatabaseOE::removeTag(string tag)
{
	vector <string> args;
	args.push_back(to_string(retrieveTagId(tag)));
	if(!existTag(tag))
		return CONST_DB_OE_TAG_NOT_EXIST;
	int id = retrieveTagId(tag);
	if((modifyDatabase("DELETE FROM Tags WHERE id_tag = ?1;", args) == CONST_DB_OE_OK) && 
		(modifyDatabase("DELETE FROM StudyTags WHERE id_tag = ?1;", args) == CONST_DB_OE_OK))
		return CONST_DB_OE_OK;
	return CONST_DB_OE_SQLERR;
}


/*STUDYTAGS*/


bool DatabaseOE::existStudyTag(string studyId, string tag)
{
	if(existTag(tag))
	{
		vector <string> args;
		args.push_back(to_string(retrieveTagId(tag)));
		args.push_back(studyId);
		return existInTable("SELECT * FROM StudyTags WHERE id_tag =?1 AND id_study =?2 ;", args);
	}		
	return false;
}


vector<string> DatabaseOE::retrieveStudyTagList(string id_study)
{
	vector <string> args;
	args.push_back(id_study);
	string query = "SELECT tag FROM StudyTags INNER JOIN Tags ON Tags.id_tag = StudyTags.id_tag  WHERE id_study = ?1 ORDER BY tag;";
	return retrieveText(query, args);
}


int DatabaseOE::addStudyTag(string studyId, string tag)
{
	if(!existTag(tag))
		return CONST_DB_OE_TAG_NOT_EXIST;
	if(existStudyTag(studyId, tag))
		return CONST_DB_OE_STUDYTAG_ALREADY_EXIST;
	vector <string> args;
	args.push_back(studyId);
	args.push_back(to_string(retrieveTagId(tag)));
	return modifyDatabase("INSERT INTO StudyTags(id_study, id_Tag) VALUES (?1, ?2);", args);

}


int DatabaseOE::removeStudyTag(string studyId, string tag)
{
	if(!existTag(tag))
		return CONST_DB_OE_TAG_NOT_EXIST;
	if(!existStudyTag(studyId, tag))
		return CONST_DB_OE_STUDYTAG_NOT_EXIST;
	vector <string> args;
	args.push_back(to_string(retrieveTagId(tag)));
	args.push_back(studyId);
	return modifyDatabase("DELETE FROM StudyTags WHERE id_Tag = ?1 AND id_study = ?2;", args);
}


int DatabaseOE::nbEntriesInTable(string tableName)
{
	vector <string> args;
	return retrieveInt("SELECT COUNT(*) FROM " + tableName + ";", args);
}


vector<string> DatabaseOE::retrieveStudyListFromTag(string tag)
{
	vector <string> args;
	args.push_back(tag);
	string query = "SELECT id_study FROM Tags INNER JOIN StudyTags ON Tags.id_tag = StudyTags.id_tag  WHERE tag = ?1;";
	return retrieveText(query, args);
}


/*EXTERNAL FUNCTIONS*/


string permissionString(OE2Permission permission)
{
  switch(permission)
  {
    case OE2Permission_Delete:
      return "Delete";
    case OE2Permission_Read:
      return "Read";
    case OE2Permission_Write:
      return "Write";
    case OE2Permission_SuperUser:
      return "SuperUser";
    case OE2Permission_No:
      return "No";
    default:
      return "Error";
  }
}


OE2Permission permissionEnum(string permission)
{
  if(permission.compare("Delete") == 0)
    return OE2Permission_Delete;
  else if(permission.compare("Read") == 0)
    return OE2Permission_Read;
  else if(permission.compare("Write") == 0)
    return OE2Permission_Write;
  else if(permission.compare("SuperUser") == 0)
    return OE2Permission_SuperUser;
  else 
    return OE2Permission_No;
}


