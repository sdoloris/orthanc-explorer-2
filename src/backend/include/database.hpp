#include <iostream>
using namespace std;
#include "sqlite3.h"
#include <cstdio>
#include <string>
#include <vector>

/*
  After a meeting with the orthanc team, it has beeen decided to refactore this database class. The refactoring is about the table "Permissions" and the function that return a boolean if a user is allowed to do something or not. 
  The goal is to code in the future a global function (the function will be call by the plugin filter) that takes a route (send by the filter) and a set of permission (send by the database) and compute if the route is authorized base on the set of permission. 
  This function will be more easy to test because it only requiered a string (the route) and a vector of string (the set of permission) rather than a initialisated database and so on. 
  But for this sprint we can keep our first implementation.
*/
typedef enum
{
  OE2Permission_Read = 1,
  OE2Permission_Write = 2,
  OE2Permission_Delete = 3,
  OE2Permission_SuperUser = 4,
  OE2Permission_No = 5
} OE2Permission;

class DatabaseOE
{
	private:
    
  /*
    All the methods have been declared as public to allow an a deeper testing and an easier test implementation
  */

  public:
    
		string path;

		sqlite3 *db;
    int result;

    /*ERROR CODE*/


    const int CONST_DB_OE_OK = 0;
    const int CONST_DB_OE_SQLERR = 1;

    const int CONST_DB_OE_PERMISSION_NOT_EXIST = 2;
    const int CONST_DB_OE_PERMISSION_ALREADY_EXIST = 3;

    const int CONST_DB_OE_TABLE_NOT_EXIST = 4;
    /* CONST_DB_OE_TABLE_NOT_EXIST and
    CONST_DB_OE_TABLE_ALREADY_EXIST are out of date
    const int CONST_DB_OE_TABLE_ALREADY_EXIST = 5;*/

    const int CONST_DB_OE_TAG_NOT_EXIST = 6;
    const int CONST_DB_OE_TAG_ALREADY_EXIST = 7;
    /* CONST_DB_OE_NEW_TAG_ALREADY_USED is out of date
    const int CONST_DB_OE_NEW_TAG_ALREADY_USED = 8;*/
    
    const int CONST_DB_OE_USER_NOT_EXIST = 9;
    /* CONST_DB_OE_USER_ALREADY_EXIST is redundant with 
    CONST_DB_OE_USERNAME_ALREADY_USED
    const int CONST_DB_OE_USER_ALREADY_EXIST = 10;*/
    const int CONST_DB_OE_USERNAME_ALREADY_USED = 11;

    const int CONST_DB_OE_USERPERMISSION_NOT_EXIST = 12;
    const int CONST_DB_OE_USERPERMISSION_ALREADY_EXIST = 13;
    const int CONST_DB_OE_LAST_SUPERUSER = 14;

    const int CONST_DB_OE_STUDYTAG_ALREADY_EXIST = 15;
    const int CONST_DB_OE_STUDYTAG_NOT_EXIST = 16;

    const int CONST_OE_NB_PERMISSION = 4;

    /*CONSTRUCTOR*/
    

    /*
      This function constructs a database object to handle the database in the file passed as parameter
      Parameter:
        -pPath: the path of the database file
      Return:
    */

    DatabaseOE(const string pPath);

    /*GENERAL*/


    /*
      This function sets the path where the database is sorted
      Parameter:
        -pPath: the path of the database file
      Return:
    */

    void setPath(const string pPath);


    /*
      This function opens the database
      Parameter:
      Return:
        -the error code associated to the action
    */

    int open();


    /*
      This function closes the database
      Parameter:
      Return:
        -the error code associated to the action
    */

    int close();


    /*
      This function retrieves the list of string returned by a sql query and its arguments.
      Parameter:
        -query: the sql query 
        -args: the arguments of the query
      Return:
        -the result of the query as a list of string 
    */

    vector<string> retrieveText(string query, vector <string> args);


    /*
      This function retrieves the list of OE2Permission returned by a given sql query and its arguments.
      Parameters:
        -query: the sql query 
        -args: the arguments of the query
      Return:
        -the list of permissions
    */

    vector<OE2Permission> retrieveOE2Permission(string query, vector <string> args);


    /*
      This function retrieve the integer of the first column of the first row returned by a given sql query and its arguments. The int sorted in the database are positive integer, then if there is a probleme in the query, the function return the value -1
      Parameters:
        -query: the sql query 
        -args: the argument of the query
      Return:
        -the int of the first column of the first row if the query return a value
        -the error value -1
    */

    int retrieveInt(string query, vector <string> args);


    /*
      This function displays all the columns of all the rows of a sql query
      Parameters:
        -query: the sql query 
        -args: the arguments of the query
      Return:
        -the error code associated to the query
    */

    int displayRes(string query, vector <string> args);
    

    /*
      This function checks if a sql query has at least a row of result
      Parameters:
        -query: the sql query 
        -args: the argument of the query
      Return:
        -the result of the check
    */

    bool existInTable(string query, vector <string> args);


    /*
      This function performes the modification of the database defined by a sql query and its arguments
      Parameters:
        -query: the sql query 
        -args: the argument of the query
      Return:
        -the error code associated to the action
    */

    int modifyDatabase(string query, vector <string> args);


    /*
      This function checks if a table exists in the database.
      Parameter:
        -tableName
      Return:
        -the result of the check
    */

    bool existTable(string tableName);


    /*
      This function initializes the database. it creates the tables of the database and they checks is they are all created
      Parameter:
      Return:
        -the error code associated to the creation of tables
    */

    int init();


    /*
      This function drops the table passed as parameter (and also drops 'UserPermission' if the tabe is 'Users' or "Permission")
      Parameter:
        -tableName: the name of the table to drop
      Return:
        -the error code associated to the action
    */

    int dropTable(string tableName);

        
    /*
      This function deletes the database file
      Parameter:
      Return:
        - true if the database has been correctely deleted
        - false if the remove function failed
    */

    bool dropDatabase();


    /*
      This function displays all the fields of a table 
      Parameter:
        -tableName: the name of the table to display
      Return:
        -the error code associated to the action
    */

    int readWholeTable(string tableName);


    /*
      This function adds a root user that has all the permissions
      Parameter:
        -the error code associated to the action
    */

//	  int addRoot();


	  /*USERS*/


    /*
      This function returns the list of users stored in the database
      Parameter:
      Return:
        -the list of users
    */

    vector<string> getUsers();


    /*
      This function checks if a user has the given password
      Parameters:
        -id_user: the id of the userk
        -password: the password
      Return:
        -the result of the check
    */
	 
    bool existUserPassword(int id_user, string password);

    
    /*
      This function checks if a user exists
      Parameter:
        -id_user: the id of the user
      Return:
        -the result of the check
    */

	  bool existUser(int id_user);


    /*
      This function checks if a user exists
      Parameter:
        -username: the name of the user
      Return:
        -the result of the check
    */

    bool existUserString(string username);


    /*
      This function checks if a user is the last user with the permission SuperUser 
      Parameter:
        -the id of the user
      Return:
        -the result of the check
    */

	  bool isLastSuperUser(int id_user);


    /*
      This function returns the id of a user from it name
      Parameter:
        -the name of the user
      Return:
        -the id of the user
    */

	  int retrieveUserId(string username);


    /*
      This function returns the name of a user from it id
      Parameter:
        -the id of the user
      Return:
        -the name of the user
    */

    string retrieveUsername(int id_user);

    /*
      This function adds a user 
      Parameters:
        -username: the name of the user
        -password: the password of the user
      Return:
        -the error code associated to the action
    */

    int addUser(string username, string password);


    /*
      This function modifies the user data 
      Parameters:
        -username: the name of the user to modify
        -nUsername: the new name of the user
        -nPassword: the new password of the user
      Return:
        -the error code associated to the action
    */

    int modifyUser(string username, string nUsername, string nPassword);


    /*
      This function displays the name, id and password (hashd)  of a user
      Parameter:
        -id_user: the id of the user
      Return:
        -the error code associated to the action
    */
    
    int readUserData(int id_user);


    /*
      This function removes the user data (and also his permissions) 
      Parameter:
        -id_user: the id of the user
      Return:
        -the error code associated to the action
    */

    int removeUser(int id_user);


	  /*USERPERMISSIONS*/


    /*
      This function add a permission to a user
      Parameters:
        -id_user: the id of the user
        -permission: the permission to check
      Return:
        -the error code associated to the action
    */

	  int addUserPermission(int id_user, OE2Permission permission);


    /*
      This function checks if a user has a particular permission
      Parameter:
        -id_user: the id of the user
        -permission: the permission to check
      Return:
        -the result of the check
    */

	  bool existUserPermission(int id_user, OE2Permission permission);


    /*
      This function removes a permission of a user
      Parameter:
        -id_user: the id of the user
        -permission: the permission to remove
      Return:
        -the error code associated to the action
    */

	  int removeUserPermission(int id_user, OE2Permission permission);


    /*
      This function returns the list of permissions associated to a user 
      Parameter:
        -id_user: the id of the user
      Return:
        -the list of permission
    */

    vector<OE2Permission> retrieveUserPermission(int id_user);


    /*
      This function displays all the permission of a user
      Parameter:
        -id_user: the id of the user
      Return:
        -the error code associated to the action
    */

    int displayUserPermission(int id_user);


    /*
      This function adds all the permission for the user
      Parameter:
        -id_user: the id of the user
      Return:
        -the error code associated to the action
    */

//    int addAllUserPermissions(int id_user);


    /*
      This function checks if the user has the SuperUser permission
      Parameter:
        -id_user: the id of the user
      Return:
        -the resut of the check
    */

    bool isSuperUser(int id_user);


    /*
      This function checks if the user has the permission to delete data in the orthanc server
      Parameter:
        -id_user: the id of the user
      Return:
        -the resut of the check
    */

    bool isAllowedToDelete(int id_user);

  
    /*
      This function checks if the user has the permission to read data from the orthanc server
      Parameter:
        -id_user: the id of the user
      Return:
        -the resut of the check
    */

    bool isAllowedToRead(int id_user);


    /*
      This function checks if the user has the permission to write data in the orthanc server
      Parameter:
        -id_user: the id of the user
      Return:
        -the resut of the check
    */

    bool isAllowedToWrite(int id_user);


    /*
      This function removes all the user permission
      Parameter:
        -id_user: the id of the user
      Return:
        -the error code associated to the action
    */

    int removeAllUserPermissions(int id_user);


		/*TAGS*/


    /*
      This function checks if a tag exists
      Parameter:
        -tag: the tag name
      Return:
        -the result of the check
    */

		bool existTag(string tag);


    /*
      This function returns the id of a tag
      Parameter:
        -tag: the tag name
      Return:
        -the tag id
    */

		int retrieveTagId(string tag);


    /*
      This function adds a tag in the database
      Parameter:
        -tag: the name of the tag
      Return:
        -the error code associated to the action
    */

    int addTag(string tag);


     /*
      This function modifies the name of a tag in the database
      Parameters:
        -tag: the name of the tag to modify
        -nTag: the new name of the tag
      Return:
        -the error code associated to the action
    */

    int modifyTag(string tag, string nTag);


    /*
      This function removes a tag from the database
      Parameter:
        -tag: the name of the tag to modify
      Return:
        -the error code associated to the action
    */

    int removeTag(string tag);


		/*STUDYTAGS*/


    /*
      This function checks if a particular study has a particular tag
      Parameters:
        -studyId: the id of the study
        -tag: the tag name
      Return:
        -the result of the check
    */

		bool existStudyTag(string studyId, string tag);


    /*
      This function returns the list of all the tags sorted in the database 
      Parameter:
      Return:
        -the list of tags
    */

    vector<string> retrieveTagList();


    /*
      This function returns the list of tags associated to a study
      Parameter:
        -studyId: the Orthanc id of the study
      Return:
        -the list of tags
    */

    vector<string> retrieveStudyTagList(string id_study);


		/*
    	This function links a tag with a study
    	Parameter:
      	-studyId: the orthanc id of the study
      	-tag: the name of the tag
      Return:
        -the error code associated to the action
   	*/

		int addStudyTag(string studyId, string tag);


		/*
    	This function removes a tag of a study
    	Parameter:
      	-studyId: the orthanc id of the study
      	-tag: the name of the tag
      Return:
        -the error code associated to the action
   	*/

		int removeStudyTag(string studyId, string tag);


    /*
      This function returns the number of column of the given table 
      Parameter:
        -tableName: the name of the table
      Return: 
        -the number of columns of the table
    */

    int nbEntriesInTable(string tableName);


    /*
      This function returns the list of study associated to the given tag 
      Parameter:
        -tag: the name of the tag
      Return: 
        -the list of Orthanc study id
    */

    vector<string> retrieveStudyListFromTag(string tag);
};


/*EXTERNAL FUNCTIONS*/


/*
  This function returns the name associated to the given OE2Permission
  Parameter: 
    -an OE2Permission
  Return:
    -the name of the permsision as a string
*/

string permissionString(OE2Permission permission);


/*
  This function returns the OE2Permission associated to the given permission's name 
  Parameter: 
    -an OE2Permission element
  Return:
    -the name of the permission as a string
*/

OE2Permission permissionEnum(string permission);