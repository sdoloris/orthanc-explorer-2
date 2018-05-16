#include "filterTest.hpp"

using namespace CppUnit;
using namespace std;

void FilterTest::setUp(void)
{
	fillMaps();
	pDelete.push_back(OE2Permission_Delete);
	pRead.push_back(OE2Permission_Read);
	pSuperuser.push_back(OE2Permission_SuperUser);
	pWrite.push_back(OE2Permission_Write);
}
 
void FilterTest::tearDown(void)
{
    
}


void FilterTest::OE2JSTest(void)
{
  		
  const char* route = "/OE2/hello.js";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));


}


void FilterTest::patientsProtectedTest(void)
{

  const char* route = "/patients/{id}/protected";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));

}


void FilterTest::patientsTest(void)
{

  const char* route = "/patients";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));
}

void FilterTest::seriesTest(void)
{

  const char* route = "/series";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));
	

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/


	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));

}

void FilterTest::queriesAnswersTest(void)
{
  		
	const char* route = "/queries/answers";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pRead));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));

}


void FilterTest::OE2ModifyUserTest(void)
{
  		
  const char* route = "/OE2/modifyUser";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));
	

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));



	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));

}


void FilterTest::OE2StudiesFromTagTest(void)
{
  		
  const char* route = "/OE2/studiesFromTag";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));

}

void FilterTest::OE2ConfigTest(void)
{
  
  const char* route = "/OE2/config";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));
	
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));
	

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));

}


void FilterTest::toolsFindTest(void)
{
  		
  const char* route = "/tools/find";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));

}



void FilterTest::OE2TagsTest(void)
{
  		const char* route = "/OE2/tags";

	/*Acces authorizated*/

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Delete, pSuperuser));

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pRead));
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Get, pSuperuser));
	
	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Post, pSuperuser));
	

	CPPUNIT_ASSERT(routeFilter(route, OrthancPluginHttpMethod_Put, pSuperuser));

	/*Acces denied*/

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Delete, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Get, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Post, pWrite));

	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pDelete));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pNo));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pRead));
	CPPUNIT_ASSERT(!routeFilter(route, OrthancPluginHttpMethod_Put, pWrite));

}
