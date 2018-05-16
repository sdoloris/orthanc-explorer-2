#include "cookieTest.hpp"


using namespace CppUnit;
using namespace std;


/*
	Some methods are not tested because they are not used in OE2. 
	Their main goal is to help to test/debug (retrieve data, insert mock data, etc)
*/


void cookieTest::setUp(void)
{


}
 
void cookieTest::tearDown(void)
{
    
}



void cookieTest::randomStringTest(void)
{
	string cookie1 = cH->genRandomString();
	string cookie2 = cH->genRandomString();
	
	/*the string have the right size*/
	CPPUNIT_ASSERT(cookie1.size() == cookieSize);
	CPPUNIT_ASSERT(cookie2.size() == cookieSize);

	/*the strings are different*/
	CPPUNIT_ASSERT(cookie1.compare(cookie2) != 0);
	
}

/*
	- getTimeStamp
	- insertCookie
*/

void cookieTest::insertCookieTest(void)
{
	int id_newUser = 2;

	string wrongkey = cH->genRandomString();
	
	/*If the key does not match any entry the timestamp is 0*/
	CPPUNIT_ASSERT(cH->getTimeStamp(wrongkey) == 0);

	string grootkey = cH->insertCookie(id_groot);
	time_t currentTime = time(NULL);
	time_t grootTimestamp = cH->getTimeStamp(grootkey);

	/*Check if the timestamp of the inserted cookie is the current timestamps so the entry has been added*/
	CPPUNIT_ASSERT((currentTime == grootTimestamp) || (currentTime == (grootTimestamp+1)));


	/*Add another entry*/
	/*The user has been added if the value returned by getTimeStamp is not 0*/
	string newUserkey = cH->insertCookie(id_newUser);
	CPPUNIT_ASSERT(cH->getTimeStamp(newUserkey) != 0);
	
}


void cookieTest::checkCookieTest(void)
{
	time_t expirationTime = 20;

	string wrongkey = cH->genRandomString();
	
	/*If the key does not match any entry it return 0 */
	CPPUNIT_ASSERT(cH->checkCookie(wrongkey) == 0);
	
	/*add an entry in the table*/
	string grootkey = cH->insertExpiredCookie(id_groot, expirationTime);
	
	/*retrieve the insertion time*/
	time_t grootTimestamp = cH->getTimeStamp(grootkey);

	/*If the key match an entry it returns the id_value and update the timestamp*/
	CPPUNIT_ASSERT(cH->checkCookie(grootkey) == id_groot);

	/*retrieve the new timestamp*/
	time_t updatedGrootTimestamp = cH->getTimeStamp(grootkey);


	/*Check update timestamp functionnality of the method */

	/*Check if the timestamp has been updated */
	CPPUNIT_ASSERT((updatedGrootTimestamp == (grootTimestamp+expirationTime)) || 
								 (updatedGrootTimestamp == (grootTimestamp+expirationTime+1)));



	/*Check delete functionnality of the method */

	int expiredUser = 2;
	expirationTime = cH->getLifeTime() + 20;
	string expiredkey = cH->insertExpiredCookie(expiredUser, expirationTime);

	/*the entry does not exist*/ 
	CPPUNIT_ASSERT(cH->checkCookie(expiredkey) == 0);
}



void cookieTest::removeCookieTest(void)
{
	int idToRemove = 2;
	/*add entries*/
	string grootkey = cH->insertCookie(id_groot);
	string keyToremove = cH->insertCookie(idToRemove);

	/*the entries exist*/
	CPPUNIT_ASSERT(cH->getTimeStamp(grootkey) != 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(keyToremove) != 0);

	/*The entry is removed*/
	CPPUNIT_ASSERT(cH->removeCookie(keyToremove));

	/*the entry of 'keyToremove' has been remove*/
	CPPUNIT_ASSERT(cH->getTimeStamp(grootkey) != 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(keyToremove) == 0);

	/*no entry match the input*/

	CPPUNIT_ASSERT(!cH->removeCookie(keyToremove));

	/*no modification in the table*/
	CPPUNIT_ASSERT(cH->getTimeStamp(grootkey) != 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(keyToremove) == 0);
}


void cookieTest::checkExpiredCookieTest(void)
{
	int expiredUser1 = 2;
	int expiredUser2 = 3;
	int expiredUser3 = 4;
	int expiredUser4 = 5;
	int notExpiredUser = 6;

	time_t expirationTime = cH->getLifeTime() + 20;

	/*add normal entries*/
	string grootkey = cH->insertCookie(id_groot);
	string notExpiredKey = cH->insertCookie(notExpiredUser);

	/*add expired entries*/
	string key1 = cH->insertExpiredCookie(expiredUser1, expirationTime);
	string key2 = cH->insertExpiredCookie(expiredUser2, expirationTime);
	string key3 = cH->insertExpiredCookie(expiredUser3, expirationTime);
	string key4 = cH->insertExpiredCookie(expiredUser4, expirationTime);

	/*all the entries are in the database*/
	CPPUNIT_ASSERT(cH->getTimeStamp(grootkey) != 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(notExpiredKey) != 0);
	
	CPPUNIT_ASSERT(cH->getTimeStamp(key1) != 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(key2) != 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(key3) != 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(key4) != 0);

	/*The time returned by the method is the time at whicht the check is done*/

	time_t currentTime = time(NULL);
	time_t checkTime = cH->checkExpiredCookie();

	CPPUNIT_ASSERT((checkTime == currentTime) ||
								 (checkTime == (currentTime+1)));

	/*Delete*/

	/*not expired entries are still in the tables*/
	CPPUNIT_ASSERT(cH->getTimeStamp(grootkey) != 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(notExpiredKey) != 0);

	/*all the expirec entry has been removed*/
	CPPUNIT_ASSERT(cH->getTimeStamp(key1) == 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(key2) == 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(key3) == 0);
	CPPUNIT_ASSERT(cH->getTimeStamp(key4) == 0);

}