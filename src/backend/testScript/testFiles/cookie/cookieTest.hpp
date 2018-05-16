#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <thread>
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
#include "../../../include/CookieHandler.hpp"

using namespace CppUnit;
using namespace std;





class cookieTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(cookieTest);

    CPPUNIT_TEST(randomStringTest);
    CPPUNIT_TEST(insertCookieTest);
    CPPUNIT_TEST(checkCookieTest);
    CPPUNIT_TEST(removeCookieTest);
    CPPUNIT_TEST(checkExpiredCookieTest);

    CPPUNIT_TEST_SUITE_END();
 
public:
    void setUp(void);
    void tearDown(void);
 
protected:

    void randomStringTest(void);
		void insertCookieTest(void);
 		void checkCookieTest(void);
		void removeCookieTest(void);
 		void checkExpiredCookieTest(void);


private:
	CookieHandler *cH= new CookieHandler();
	unsigned int cookieSize = 128;
	int id_groot = 1;
 
    
};
