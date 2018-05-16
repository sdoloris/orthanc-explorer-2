#include <iostream>
#include <string>
#include <vector>
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
#include "../../../include/filter.hpp"
#include "../../../include/OrthancCPlugin.h"

using namespace CppUnit;
using namespace std;


class FilterTest : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(FilterTest);

  CPPUNIT_TEST(OE2JSTest);
  CPPUNIT_TEST(patientsProtectedTest);
  CPPUNIT_TEST(patientsTest);
  CPPUNIT_TEST(seriesTest);
  CPPUNIT_TEST(queriesAnswersTest);
  CPPUNIT_TEST(OE2ModifyUserTest);
  CPPUNIT_TEST(OE2StudiesFromTagTest);
  CPPUNIT_TEST(OE2ConfigTest);
  CPPUNIT_TEST(toolsFindTest);
  CPPUNIT_TEST(OE2TagsTest);

  CPPUNIT_TEST_SUITE_END();
 
public:
    void setUp(void);
    void tearDown(void);
    
    vector<OE2Permission> pDelete;
    vector<OE2Permission> pRead;
    vector<OE2Permission> pSuperuser;
    vector<OE2Permission> pWrite;
    vector<OE2Permission> pNo;
 
protected:

  /*
    The name of the test is the first route listed for the group followed by 'Test'
  */


  /*
    Particular case. These route are always allowed
    Group 1:
      -/OE2/ .js
      -/OE2/ .json
      -/OE2/ .ico
      -/OE2/ .woff2
      -/OE2/
  */

  void OE2JSTest(void);

  /*
    Group 2:
      -/patients/{id}/protected

      Methods:  Permissions
      -PUT      Read
      -GET      Read
  */

  void patientsProtectedTest(void);


  /*
    Group 3:
      -/patients
      -/studies

      Methods:  Permissions
      -DELETE   Delete
      -GET      Read
  */

  void patientsTest(void);


  /*
    Group 4:
      -/series

      Methods:  Permissions
      -DELETE   Delete
  */

  void seriesTest(void);


  /*
    Group 5:
      -/queries/{id}/answers
      -/queries/{id}/retrieve
      -/patients/{id}/anonymize
      -/studies/{id}/anonymize
      -/series/{id}/anonymize
      -/instances
      -/OE2/addTagStudy
      -/OE2/removeTagStudy

      Methods:  Permissions
      -POST     Write
  */

  void queriesAnswersTest(void);


  /*
    Group 6:
      -/OE2/modifyUser
      -/OE2/login
      -/OE2/initPlugin

      Methods:  Permissions
      -POST     No
  */

  void OE2ModifyUserTest(void);


  /*
    Group 7:
      -/OE2/studiesFromTag

      Methods:  Permissions
      -POST     Read
  */

  void OE2StudiesFromTagTest(void);


  /*
    Group 8:
      -/OE2/config
      -/OE2/disconnect
      -/OE2/home
      -/OE2/plugin
      -/OE2/patients
      -/OE2/series
      -/OE2/instances
      -/OE2/upload
      -/OE2/query
      -/OE2/retrieve
      -/OE2/connect
      -/OE2/localisation
      -/OE2/userdata
      -/OE2/studies
      -/OE2
      -/OE2/disableUsers
      
      Methods:  Permissions
      -GET      No
  */

  void OE2ConfigTest(void);


  /*
    Group 9:
      -/tools/find
      -/patients/{id}/media
      -/studies/{id}/media
      -/series/{id}/media
      -/patients/{id}/archive
      -/studies/{id}/archive
      -/series/{id}/archive
      -/instances/{id}/file
      -/instances/{id}/tags

      Methods:  Permissions
      -POST     Read
  */

  void toolsFindTest(void);


  /*
    Group 10:
      -/OE2/tags
      -/patients/{id}/studies
      -/modalities
      -/instances/{id}/patients
      -/instances/{id}/studies
      -/instances/{id}/series
      -/series/{id}/patient
      -/series/{id}/study
      -/series/{id}/instances
      -/studies/{id}/series
      -/studies/{id}/patients
      -/plugins
      -/statistics
      -/instances/{id}/simplified-tags

      Methods:  Permissions
      -GET      Read
  */

  void OE2TagsTest(void);





















};