#include "databaseTest.hpp"

using namespace CppUnit;
using namespace std;


/*
	Some methods are not tested because they are not used in OE2. 
	Their main goal is to help to test/debug (display/drop table, etc)
*/


/*
	All the tests start with the initial configuration of the table:
		there is a user: root - root
		root has all the permission (Read - Write - Delete - SuperUser)
		there is no tags (so there is no studytag entry)
*/

void databaseTest::setUp(void)
{
 	DB->dropDatabase();
 	DB->open();
 	DB->addUser(groot, groot);
	DB->addUserPermission(id_groot, OE2Permission_Read);
	DB->addUserPermission(id_groot, OE2Permission_Write);
	DB->addUserPermission(id_groot, OE2Permission_Delete);
	DB->addUserPermission(id_groot, OE2Permission_SuperUser);
}
 
void databaseTest::tearDown(void)
{
  DB->close();
}


/*
****************************** initialisationTest ******************************

	This method tests the initialisation of the database:

	Here are the methods of the class that are used to achieve the initialisation and the test:
		- construteur
		- dropDatabase
		- open
		- init
		- modifyDatabase 
		- dropTable
		- addRoot
		- addUser
		- existUser
		- existInTable
		- existUserPermission
		- addAllUserPermissions
		- addUserPermission
		- retrieveUserId
		- retrieveInt
		- existTable
		- isAllowedToRead
		- isAllowedToWrite
		- isAllowedToDelete
		- isSuperUser

	Some of these methods will be deeply tested later

	If all the tests are successfully passed, it means some of theses methods that are not explicitely tested work well. In the same way, if some/all the tests failed, the error may come from one of the method that are not explicitely tested
	
*/

void databaseTest::initialisationTest(void)
{
	
	CPPUNIT_ASSERT(DB->CONST_DB_OE_OK == DB->open());

	/* Check if all the tables have been created */
	
	CPPUNIT_ASSERT(DB->existTable("Users"));
	CPPUNIT_ASSERT(DB->existTable("UserPermissions"));
	CPPUNIT_ASSERT(DB->existTable("Tags"));
	CPPUNIT_ASSERT(DB->existTable("StudyTags"));

	/*Check if all the tables have the right number of rows*/

	CPPUNIT_ASSERT(DB->nbEntriesInTable("Users") == 1);
	CPPUNIT_ASSERT(DB->nbEntriesInTable("UserPermissions") == DB->CONST_OE_NB_PERMISSION);
	CPPUNIT_ASSERT(DB->nbEntriesInTable("Tags") == 0);
	CPPUNIT_ASSERT(DB->nbEntriesInTable("StudyTags") == 0);
	
	/* Test if the root account has all the rules */

	CPPUNIT_ASSERT(DB->isAllowedToRead(id_groot));
	CPPUNIT_ASSERT(DB->isAllowedToWrite(id_groot));
	CPPUNIT_ASSERT(DB->isAllowedToDelete(id_groot));
	CPPUNIT_ASSERT(DB->isSuperUser(id_groot));

	/* Try to drop the database */
	
	CPPUNIT_ASSERT(DB->dropDatabase());

	/* Try to drop a second time the database */

	CPPUNIT_ASSERT(!DB->dropDatabase());
	
	//open the database without the method 'open' to check the dropDatebase method

	sqlite3_open(DB->path.c_str(), &DB->db);

	/* Test if all the table have been deleted */

	CPPUNIT_ASSERT(!DB->existTable("Users"));
	CPPUNIT_ASSERT(!DB->existTable("UserPermissions"));
	CPPUNIT_ASSERT(!DB->existTable("Tags"));
	CPPUNIT_ASSERT(!DB->existTable("StudyTags"));


	/*Try to init the database*/
	
	CPPUNIT_ASSERT(DB->CONST_DB_OE_OK == DB->init());

	/* Test if there are only 4 table that have been created */

	/* Test if all the table have been created */
	
	CPPUNIT_ASSERT(DB->existTable("Users"));
	CPPUNIT_ASSERT(DB->existTable("UserPermissions"));
	CPPUNIT_ASSERT(DB->existTable("Tags"));
	CPPUNIT_ASSERT(DB->existTable("StudyTags"));
	
	/* Test if the root account has all the rules */

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_groot));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_groot));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_groot));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_groot));

}


/*
********************************* addUserTest **********************************
	This function test the following methods:
	- addUser
	- retrieveUserId
	-	retrieveUsername
	- existUser
	- existUserString
	- existUserPassword
*/

void databaseTest::addUserTest(void)
{

	/*
	Initial state:
		The user "root" with password "root" is the only user and have the id 1. 
		If the user is not found the function must return -1 
	*/


	CPPUNIT_ASSERT(DB->nbEntriesInTable("Users") == 1);
	//root

	CPPUNIT_ASSERT(DB->CONST_DB_OE_USERNAME_ALREADY_USED == DB->addUser("root", "root"));
	CPPUNIT_ASSERT(DB->retrieveUserId("root") == 1);
	CPPUNIT_ASSERT((DB->retrieveUsername(1)).compare("root") == 0);
	CPPUNIT_ASSERT((DB->retrieveUsername(1)).compare("bird") != 0);
	
	CPPUNIT_ASSERT(DB->existUserString("root"));
	CPPUNIT_ASSERT(DB->existUser(DB->retrieveUserId("root")));
	CPPUNIT_ASSERT(DB->existUserPassword(DB->retrieveUserId("root"), "root"));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("root"), "password"));

	//bird
	CPPUNIT_ASSERT(DB->retrieveUserId("bird") == -1);
	CPPUNIT_ASSERT(!DB->existUserString("bird"));
	CPPUNIT_ASSERT(!DB->existUser(DB->retrieveUserId("bird")));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("bird"), "fly"));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("bird"), "root"));
	
	/*
	add a user:
		id is autoincremented by sqlite then this user has id = 2
	*/	

	CPPUNIT_ASSERT(DB->CONST_DB_OE_OK == DB->addUser("bird", "fly"));
	
	//root
	CPPUNIT_ASSERT(DB->retrieveUserId("root") == 1);
	CPPUNIT_ASSERT(DB->existUserString("root"));
	CPPUNIT_ASSERT(DB->existUser(DB->retrieveUserId("root")));
	CPPUNIT_ASSERT(DB->existUserPassword(DB->retrieveUserId("root"), "root"));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("root"), "password"));

	//bird
	CPPUNIT_ASSERT(DB->retrieveUserId("bird") == 2);
	CPPUNIT_ASSERT(DB->existUserString("bird"));
	CPPUNIT_ASSERT(DB->retrieveUsername(2).compare("bird") == 0);
	CPPUNIT_ASSERT(DB->retrieveUsername(2).compare("root") != 0);
	CPPUNIT_ASSERT(DB->existUser(DB->retrieveUserId("bird")));
	CPPUNIT_ASSERT(DB->existUserPassword(DB->retrieveUserId("bird"), "fly"));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("bird"), "root"));

	/*
	Try to add a user with the same username/password:
		database not modified 
	*/
	
	CPPUNIT_ASSERT(DB->CONST_DB_OE_USERNAME_ALREADY_USED == DB->addUser("bird", "fly"));

	//root
	CPPUNIT_ASSERT(DB->retrieveUserId("root") == 1);
	CPPUNIT_ASSERT(DB->existUserString("root"));
	CPPUNIT_ASSERT(DB->existUser(DB->retrieveUserId("root")));
	CPPUNIT_ASSERT(DB->existUserPassword(DB->retrieveUserId("root"), "root"));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("root"), "password"));

	//bird
	CPPUNIT_ASSERT(DB->retrieveUserId("bird") == 2);
	CPPUNIT_ASSERT(DB->existUserString("bird"));
	CPPUNIT_ASSERT(DB->existUser(DB->retrieveUserId("bird")));
	CPPUNIT_ASSERT(DB->existUserPassword(DB->retrieveUserId("bird"), "fly"));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("bird"), "root"));

	/*
	Try to add a user with the same usernameand a different password:
		database not modified 
	*/

	CPPUNIT_ASSERT(DB->CONST_DB_OE_USERNAME_ALREADY_USED == DB->addUser("bird", "bird"));

	//root
	CPPUNIT_ASSERT(DB->retrieveUserId("root") == 1);
	CPPUNIT_ASSERT(DB->existUserString("root"));
	CPPUNIT_ASSERT(DB->existUser(DB->retrieveUserId("root")));
	CPPUNIT_ASSERT(DB->existUserPassword(DB->retrieveUserId("root"), "root"));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("root"), "password"));

	//bird
	CPPUNIT_ASSERT(DB->retrieveUserId("bird") == 2);
	CPPUNIT_ASSERT(DB->existUserString("bird"));
	CPPUNIT_ASSERT(DB->existUser(DB->retrieveUserId("bird")));
	CPPUNIT_ASSERT(DB->existUserPassword(DB->retrieveUserId("bird"), "fly"));
	CPPUNIT_ASSERT(!DB->existUserPassword(DB->retrieveUserId("bird"), "root"));

}



/*
**************************** addUserPermissionTest *****************************
	This function test the following methods:

	- addUserPermission
	- removeUserPermission
	- removeAllUserPermissions

*/

void databaseTest::addUserPermissionTest(void)
{
	DB->addUser("bird", "fly");
	int id_bird = DB->retrieveUserId("bird");

	/*the new user has no permission*/

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));


	/*
		SuperUser
	*/

	/*add SuperUser permission*/

	CPPUNIT_ASSERT( DB->CONST_DB_OE_OK == DB->addUserPermission(id_bird, OE2Permission_SuperUser));
	
	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(DB->isSuperUser(id_bird));

	/* 
	Try to add SuperUser permission a second time:
		The database is not modified
	*/

	CPPUNIT_ASSERT( DB->CONST_DB_OE_USERPERMISSION_ALREADY_EXIST == DB->addUserPermission(id_bird, OE2Permission_SuperUser));

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(DB->isSuperUser(id_bird));

	/*remove SuperUser permission*/
	
	CPPUNIT_ASSERT( DB->CONST_DB_OE_OK == DB->removeUserPermission(id_bird, OE2Permission_SuperUser));

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));

	/* 
	Try to remove SuperUser permission a second time:
		The database is not modified
	*/
	
	CPPUNIT_ASSERT( DB->CONST_DB_OE_USERPERMISSION_NOT_EXIST == DB->removeUserPermission(id_bird, OE2Permission_SuperUser));

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));

	/*
		Read
	*/

	/*add Read permission*/

	CPPUNIT_ASSERT( DB->CONST_DB_OE_OK == DB->addUserPermission(id_bird, OE2Permission_Read));

	
	CPPUNIT_ASSERT(DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));

	/* 
	Try to add Read permission a second time:
		The database is not modified
	*/

	CPPUNIT_ASSERT( DB->CONST_DB_OE_USERPERMISSION_ALREADY_EXIST == DB->addUserPermission(id_bird, OE2Permission_Read));

	CPPUNIT_ASSERT(DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));

	/*remove Read permission*/
	
	CPPUNIT_ASSERT( DB->CONST_DB_OE_OK == DB->removeUserPermission(id_bird, OE2Permission_Read));

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));

	/* 
	Try to remove Read permission a second time:
		The database is not modified
	*/
	
	CPPUNIT_ASSERT( DB->CONST_DB_OE_USERPERMISSION_NOT_EXIST == DB->removeUserPermission(id_bird, OE2Permission_Read));

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));



	/*
		add multiple permissions
	*/

	CPPUNIT_ASSERT( DB->CONST_DB_OE_OK == DB->addUserPermission(id_bird, OE2Permission_Write));
	CPPUNIT_ASSERT( DB->CONST_DB_OE_OK == DB->addUserPermission(id_bird, OE2Permission_Delete));

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));


	/*
		remove multiple permissions
	*/

	CPPUNIT_ASSERT( DB->CONST_DB_OE_OK == DB->removeAllUserPermissions(id_bird));

	CPPUNIT_ASSERT(!DB->isAllowedToRead(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToWrite(id_bird));
	CPPUNIT_ASSERT(!DB->isAllowedToDelete(id_bird));
	CPPUNIT_ASSERT(!DB->isSuperUser(id_bird));

}


/*
******************************** removeUserTest ********************************
	This function test the following method:
	-isLastSuperUser
	-removeUser
*/

void databaseTest::removeUserTest(void)
{
	//root is the last SuperUser
	CPPUNIT_ASSERT(DB->isLastSuperUser(id_groot));
	//unexisting user
	CPPUNIT_ASSERT(!DB->isLastSuperUser(5));

	//add bird
	DB->addUser("bird", "fly");
	int id_bird = DB->retrieveUserId("bird");

	//existing user without the SuperUser permission
	CPPUNIT_ASSERT(!DB->isLastSuperUser(id_bird));

	//add to the SuperUser permission to bird
	DB->addUserPermission(id_bird, OE2Permission_SuperUser);


	//There are 2 SuperUser
	CPPUNIT_ASSERT(!DB->isLastSuperUser(id_groot));
	CPPUNIT_ASSERT(!DB->isLastSuperUser(id_bird));

	int nbEntryUsersPermissions = DB->nbEntriesInTable("UserPermissions");
	
	//delete root
	
	CPPUNIT_ASSERT( DB->CONST_DB_OE_OK == DB->removeUser(id_groot));

	//the 4 permisions about root has been deleted too
	CPPUNIT_ASSERT((nbEntryUsersPermissions-4) == DB->nbEntriesInTable("UserPermissions"));
	
	//Bird is the last SuperUser
	CPPUNIT_ASSERT(DB->isLastSuperUser(id_bird));

	/*
	Try to remove bird:
		Cannot remove the last superUser
		then bird is not deleted
	*/
	
	CPPUNIT_ASSERT( DB->CONST_DB_OE_LAST_SUPERUSER == DB->removeUser(id_bird));
	
	/*
	Try to remove root:
		root is not in the database
		then the databse is not modified
	*/
	
	CPPUNIT_ASSERT( DB->CONST_DB_OE_USER_NOT_EXIST == DB->removeUser(id_groot));

}




/*
************************** retrieveUserPermissionTest **************************
	This function test the following method:
	-retrieveUserPermission (that calls 'retrieveOE2Permission')
*/

void databaseTest::retrieveUserPermissionTest(void)
{
	//add bird, with no permission
	DB->addUser("bird", "fly");
	int id_bird = DB->retrieveUserId("bird");

	vector<OE2Permission> birdPermission = DB->retrieveUserPermission(id_bird);
	CPPUNIT_ASSERT(birdPermission.size() == 0);

	vector<OE2Permission> grootPermission = DB->retrieveUserPermission(id_groot);
	CPPUNIT_ASSERT(grootPermission.size() == 4);
	if(grootPermission.size() == 4)
	{
		CPPUNIT_ASSERT(grootPermission[0] == 1);
		CPPUNIT_ASSERT(grootPermission[1] == 2);
		CPPUNIT_ASSERT(grootPermission[2] == 3);
		CPPUNIT_ASSERT(grootPermission[3] == 4);
	}

	DB->addUserPermission(id_bird, OE2Permission_SuperUser);
	birdPermission = DB->retrieveUserPermission(id_bird);
	CPPUNIT_ASSERT(birdPermission[0] == OE2Permission_SuperUser);

	
}

/*
******************************** modifyUserTest ********************************
	This function test the following method:
	- modifyUser
*/

void databaseTest::modifyUserTest(void)
{
	CPPUNIT_ASSERT(DB->existUserPassword(id_groot, "root"));
	CPPUNIT_ASSERT(!DB->existUserPassword(id_groot, "password"));
	
	/*modify the password*/
	CPPUNIT_ASSERT(DB->CONST_DB_OE_OK == DB->modifyUser("root", "root", "password"));
	
	CPPUNIT_ASSERT(!DB->existUserPassword(id_groot, "root"));
	CPPUNIT_ASSERT(DB->existUserPassword(id_groot, "password"));

	/*modify the username*/
	CPPUNIT_ASSERT(DB->CONST_DB_OE_OK == DB->modifyUser("root", "newusername", "password"));

	CPPUNIT_ASSERT(DB->retrieveUserId("newusername") == id_groot);
	
	CPPUNIT_ASSERT(!DB->existUserPassword(id_groot, "root"));
	CPPUNIT_ASSERT(DB->existUserPassword(id_groot, "password"));

	/*The first username must exist*/
	CPPUNIT_ASSERT(DB->CONST_DB_OE_USER_NOT_EXIST == DB->modifyUser("root", "anotherName", "password"));


	/*The new username cannot be used by another user*/
	DB->addUser("bird", "fly");
	CPPUNIT_ASSERT(DB->CONST_DB_OE_USERNAME_ALREADY_USED == DB->modifyUser("newusername", "bird", "password"));

}


/*
********************************** addTagTest **********************************
	This function test the following method:
	- addTag
	- retrieveTagId
	- ExistTag
*/

void databaseTest::addTagTest(void)
{
	CPPUNIT_ASSERT(DB->retrieveTagId("newTag") == -1);
	int nbTags = DB->nbEntriesInTable("Tags");
	CPPUNIT_ASSERT(!DB->existTag("newTag"));

	CPPUNIT_ASSERT(DB->addTag("newTag") == DB->CONST_DB_OE_OK);

	CPPUNIT_ASSERT(DB->retrieveTagId("newTag") == 1);
	CPPUNIT_ASSERT((nbTags+1) == DB->nbEntriesInTable("Tags"));
	CPPUNIT_ASSERT(DB->existTag("newTag"));

	/*Try to add the same tag name*/
	CPPUNIT_ASSERT(DB->addTag("newTag") == DB->CONST_DB_OE_TAG_ALREADY_EXIST);
	/*The tag still exists*/
	CPPUNIT_ASSERT(DB->existTag("newTag"));


	CPPUNIT_ASSERT(DB->addTag("secondTag") == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT(DB->retrieveTagId("secondTag") == 2);
}

/*
******************************** removeTagTest *********************************
	This function test the following method:
	- removeTag
	- ExistTag
*/

void databaseTest::removeTagTest(void)
{
	CPPUNIT_ASSERT(DB->addTag("newTag") == DB->CONST_DB_OE_OK);

	int nbTags = DB->nbEntriesInTable("Tags");
	CPPUNIT_ASSERT(DB->existTag("newTag"));

	CPPUNIT_ASSERT(DB->removeTag("newTag") == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT((nbTags-1) == DB->nbEntriesInTable("Tags"));
	CPPUNIT_ASSERT(!DB->existTag("newTag"));

	/*Try to remove the same tag name*/
	CPPUNIT_ASSERT(DB->removeTag("newTag") == DB->CONST_DB_OE_TAG_NOT_EXIST);
	/*The tag still not exist*/
	CPPUNIT_ASSERT(!DB->existTag("newTag"));
}

/*
******************************** modifyTagTest *********************************
	This function test the following method:
	- modifyTag
	- retrieveTagId
	- ExistTag
*/

void databaseTest::modifyTagTest(void)
{
	
	CPPUNIT_ASSERT(DB->addTag("newTag") == DB->CONST_DB_OE_OK);
	int nbTags = DB->nbEntriesInTable("Tags");
	CPPUNIT_ASSERT(DB->existTag("newTag"));
	CPPUNIT_ASSERT(DB->retrieveTagId("newTag") == 1);

	/*modify a tag name*/

	CPPUNIT_ASSERT(DB->modifyTag("newTag", "modifiedTag") == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT(nbTags == DB->nbEntriesInTable("Tags"));
	CPPUNIT_ASSERT(!DB->existTag("newTag"));
	CPPUNIT_ASSERT(DB->existTag("modifiedTag"));
	//keep the same id
	CPPUNIT_ASSERT(DB->retrieveTagId("modifiedTag") == 1);


	/* Try to modifu a tag name that does not exist */
	CPPUNIT_ASSERT(DB->modifyTag("newTag", "wrongInput") == DB->CONST_DB_OE_TAG_NOT_EXIST);
	CPPUNIT_ASSERT(!DB->existTag("wrongInput"));


	/*add a new tag*/
	CPPUNIT_ASSERT(DB->addTag("existingName") == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT(DB->existTag("existingName"));
	CPPUNIT_ASSERT(DB->existTag("modifiedTag"));
	
	/* Try to modify a tag name with a name that already */
	CPPUNIT_ASSERT(DB->modifyTag("modifiedTag", "existingName") == DB->CONST_DB_OE_TAG_ALREADY_EXIST);
	/*no modification*/
	CPPUNIT_ASSERT(DB->existTag("existingName"));
	CPPUNIT_ASSERT(DB->existTag("modifiedTag"));


	/* Try to modify a tag name with the same name that already */
	CPPUNIT_ASSERT(DB->modifyTag("modifiedTag", "modifiedTag") == DB->CONST_DB_OE_TAG_ALREADY_EXIST);
	CPPUNIT_ASSERT(DB->existTag("modifiedTag"));
}




/*
********************************* studyTagTest *********************************
	This function test the following method:
	- addStudyTag
	- existStudyTag
	- retrieveStudyTagList
	- retrieveTagList
	- removeStudyTag
*/

void databaseTest::studyTagTest(void)
{
	vector<string> TagList;
	string study_id = "study_id";
	string study2 = "study2";
	string study3 = "study3";


	string tagName = "newTag";
	string tag2 = "tag2";
	string tag3 = "tag3";
	string tag4 = "tag4";

	TagList = DB->retrieveTagList();
	CPPUNIT_ASSERT(TagList.size() == 0);


	/* try to link a study to a tag that does not exist */
	CPPUNIT_ASSERT(DB->addStudyTag(study_id, tagName) == DB->CONST_DB_OE_TAG_NOT_EXIST);
	CPPUNIT_ASSERT(!DB->existStudyTag(study_id, tagName));
	
	/*tag list with no tag*/
	TagList = DB->retrieveTagList();
	CPPUNIT_ASSERT(TagList.size() == 0);


	/* try to link a study to an existin tag */
	CPPUNIT_ASSERT(DB->addTag(tagName) == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT(DB->addStudyTag(study_id, tagName) == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT(DB->existStudyTag(study_id, tagName));

	/*tag list with one tag*/
	TagList = DB->retrieveTagList();
	CPPUNIT_ASSERT(TagList.size() == 1);
	if(TagList.size() == 1)
		CPPUNIT_ASSERT(TagList[0].compare(tagName) == 0);


	/* try to link a second time a study to a tag */
	CPPUNIT_ASSERT(DB->addStudyTag(study_id, tagName) == DB->CONST_DB_OE_STUDYTAG_ALREADY_EXIST);
	CPPUNIT_ASSERT(DB->existStudyTag(study_id, tagName));

	/*link several tag to the study*/
	CPPUNIT_ASSERT(DB->addTag(tag2) == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT(DB->addStudyTag(study_id, tag2) == DB->CONST_DB_OE_OK);

	CPPUNIT_ASSERT(DB->addTag(tag3) == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT(DB->addStudyTag(study_id, tag3) == DB->CONST_DB_OE_OK);
	
	/*add a tag linked with any study*/
	CPPUNIT_ASSERT(DB->addTag(tag4) == DB->CONST_DB_OE_OK);
	
	/*link a tag to another study*/
	CPPUNIT_ASSERT(DB->addStudyTag(study2, tagName) == DB->CONST_DB_OE_OK);

	/*tag list with multiple tag*/
	TagList = DB->retrieveTagList();
	CPPUNIT_ASSERT(TagList.size() == 4);
	if(TagList.size() == 4)
	{
		CPPUNIT_ASSERT(TagList[0].compare(tagName) == 0);
		CPPUNIT_ASSERT(TagList[1].compare(tag2) == 0);
		CPPUNIT_ASSERT(TagList[2].compare(tag3) == 0);
		CPPUNIT_ASSERT(TagList[3].compare(tag4) == 0);
	}


	/*retrieve Studytag*/

	/*no tag linked to the study*/
	TagList = DB->retrieveStudyTagList(study3);
	CPPUNIT_ASSERT(TagList.size() == 0);

	/*one tag linked to the study*/
	TagList = DB->retrieveStudyTagList(study2);
	CPPUNIT_ASSERT(TagList.size() == 1);
	if(TagList.size() == 1)
		CPPUNIT_ASSERT(TagList[0].compare(tagName) == 0);

	/*multiple tags linked to the study*/
	TagList = DB->retrieveStudyTagList(study_id);
	CPPUNIT_ASSERT(TagList.size() == 3);
	if(TagList.size() == 3)
	{
		CPPUNIT_ASSERT(TagList[0].compare(tagName) == 0);
		CPPUNIT_ASSERT(TagList[1].compare(tag2) == 0);
		CPPUNIT_ASSERT(TagList[2].compare(tag3) == 0);
	}


	/*Remove StudyTag*/

	CPPUNIT_ASSERT(DB->existStudyTag(study_id, tagName));
	
	/*remove a study tag*/
	CPPUNIT_ASSERT(DB->removeStudyTag(study_id, tagName) == DB->CONST_DB_OE_OK);
	CPPUNIT_ASSERT(!DB->existStudyTag(study_id, tagName));

	/*Try to remove a study tag that does not exist*/
	CPPUNIT_ASSERT(DB->removeStudyTag(study_id, tagName) == DB->CONST_DB_OE_STUDYTAG_NOT_EXIST);
	CPPUNIT_ASSERT(!DB->existStudyTag(study_id, tagName));

	/*Try to remove a study tag with a tag that does not exist*/
	CPPUNIT_ASSERT(DB->removeStudyTag(study_id, "unknownTag") == DB->CONST_DB_OE_TAG_NOT_EXIST);

}




/*
************************* retrieveStudyListFromTagTest *************************
	This function test the following method:
	- retrieveStudyListFromTag
*/


void databaseTest::retrieveStudyListFromTagTest(void)
{

	vector<string> studyList;
	string study1 = "study1";
	string study2 = "study2";
	string study3 = "study3";

	string tag1 = "tag1";
	string tag2 = "tag2";
	string tag3 = "tag3";
	string tag4 = "tag4";


	DB->addTag(tag1);
	DB->addTag(tag2);
	DB->addTag(tag3);
	DB->addTag(tag4);

	DB->addStudyTag(study1, tag1);
	
	DB->addStudyTag(study2, tag1);
	DB->addStudyTag(study2, tag2);

	DB->addStudyTag(study3, tag1);
	DB->addStudyTag(study3, tag2);
	DB->addStudyTag(study3, tag3);
	

	/*The 3 studies are linked to tag1 */

	studyList = DB->retrieveStudyListFromTag("tag1");
	CPPUNIT_ASSERT(studyList.size() == 3);
	
	if(studyList.size() == 3)
	{
		CPPUNIT_ASSERT(studyList[0].compare(study1) == 0);
		CPPUNIT_ASSERT(studyList[1].compare(study2) == 0);
		CPPUNIT_ASSERT(studyList[2].compare(study3) == 0);
	}


	/* 2 studies are linked to tag3 */
	
	studyList = DB->retrieveStudyListFromTag("tag2");
	CPPUNIT_ASSERT(studyList.size() == 2);
	if(studyList.size() == 3)
	{
		CPPUNIT_ASSERT(studyList[0].compare(study1) == 0);
		CPPUNIT_ASSERT(studyList[1].compare(study2) == 0);
	}


	/* 1 study is linked to tag3 */
	
	studyList = DB->retrieveStudyListFromTag("tag3");
	CPPUNIT_ASSERT(studyList.size() == 1);
	if(studyList.size() == 3)
	{
		CPPUNIT_ASSERT(studyList[0].compare(study1) == 0);
	}


	/* no study is linked to tag4 */

	studyList = DB->retrieveStudyListFromTag("tag4");
	CPPUNIT_ASSERT(studyList.size() == 0);


}



/*
************************** permissionStringEnumTest ****************************
	This function test the following method:
	- permissionString
	- permissionEnum
*/

void databaseTest::permissionStringEnumTest(void)
{
	/*expected input*/
	CPPUNIT_ASSERT(permissionString(OE2Permission_Read).compare("Read") == 0);
	CPPUNIT_ASSERT(permissionString(OE2Permission_Write).compare("Write") == 0);
	CPPUNIT_ASSERT(permissionString(OE2Permission_Delete).compare("Delete") == 0);
	CPPUNIT_ASSERT(permissionString(OE2Permission_SuperUser).compare("SuperUser") == 0);
	CPPUNIT_ASSERT(permissionString(OE2Permission_No).compare("No") == 0);
	/*wrong input*/
	CPPUNIT_ASSERT(permissionString((OE2Permission) 25).compare("Error") == 0);	

	/*expected input*/
	CPPUNIT_ASSERT(permissionEnum("Read") == OE2Permission_Read);
	CPPUNIT_ASSERT(permissionEnum("Write") == OE2Permission_Write);
	CPPUNIT_ASSERT(permissionEnum("Delete") == OE2Permission_Delete);
	CPPUNIT_ASSERT(permissionEnum("SuperUser") == OE2Permission_SuperUser);
	CPPUNIT_ASSERT(permissionEnum("No") == OE2Permission_No);
	/*wrong input*/
	CPPUNIT_ASSERT(permissionEnum("WrongInput") == OE2Permission_No);
}








