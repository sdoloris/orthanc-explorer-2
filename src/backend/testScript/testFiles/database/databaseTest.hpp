#include <iostream>
#include <string>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>
#include "../../../include/database.hpp"

using namespace CppUnit;
using namespace std;


class databaseTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(databaseTest);
    
    CPPUNIT_TEST(initialisationTest);
    CPPUNIT_TEST(addUserTest);
    CPPUNIT_TEST(addUserPermissionTest);      
    CPPUNIT_TEST(removeUserTest);
    CPPUNIT_TEST(retrieveUserPermissionTest);
    CPPUNIT_TEST(modifyUserTest);
    
    CPPUNIT_TEST(addTagTest);
    CPPUNIT_TEST(removeTagTest);
    CPPUNIT_TEST(modifyTagTest);
    CPPUNIT_TEST(studyTagTest);
    CPPUNIT_TEST(retrieveStudyListFromTagTest);

    CPPUNIT_TEST(permissionStringEnumTest);

    CPPUNIT_TEST_SUITE_END();
 
public:
    void setUp(void);
    void tearDown(void);
 
protected:
    //USER
    void initialisationTest(void);
    void addUserTest(void);
    void addUserPermissionTest(void);
    void removeUserTest(void); 
    void retrieveUserPermissionTest(void);
    void modifyUserTest(void);
    //TAG
    void addTagTest(void);
    void removeTagTest(void);
    void modifyTagTest(void);
    void studyTagTest(void);
    void retrieveStudyListFromTagTest(void);

    //GENERAL
    void permissionStringEnumTest(void);
 
private:
	string dbName = "testFiles/database/hello.db";
	string groot = "root";
	int id_groot = 1; 
    DatabaseOE *DB = new DatabaseOE(dbName);

};