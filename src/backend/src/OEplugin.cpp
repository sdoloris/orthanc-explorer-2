


/**
 * \mainpage
 *
 * This C/C++ SDK allows external developers to create plugins that
 * can be loaded into Orthanc to extend its functionality. Each
 * Orthanc plugin must expose 4 public functions with the following
 * signatures:
 * 
 * -# <tt>int32_t OrthancPluginInitialize(const OrthancPluginContext* context)</tt>:
 *    This function is invoked by Orthanc when it loads the plugin on startup.
 *    The plugin must:
 *    - Check its compatibility with the Orthanc version using
 *      ::OrthancPluginCheckVersion().
 *    - Store the context pointer so that it can use the plugin 
 *      services of Orthanc.
 *    - Register all its REST callbacks using ::OrthancPluginRegisterRestCallback().
 *    - Possibly register its callback for received DICOM instances using ::OrthancPluginRegisterOnStoredInstanceCallback().
 *    - Possibly register its callback for changes to the DICOM store using ::OrthancPluginRegisterOnChangeCallback().
 *    - Possibly register a custom storage area using ::OrthancPluginRegisterStorageArea().
 *    - Possibly register a custom database back-end area using OrthancPluginRegisterDatabaseBackendV2().
 *    - Possibly register a handler for C-Find SCP using OrthancPluginRegisterFindCallback().
 *    - Possibly register a handler for C-Find SCP against DICOM worklists using OrthancPluginRegisterWorklistCallback().
 *    - Possibly register a handler for C-Move SCP using OrthancPluginRegisterMoveCallback().
 *    - Possibly register a custom decoder for DICOM images using OrthancPluginRegisterDecodeImageCallback().
 *    - Possibly register a callback to filter incoming HTTP requests using OrthancPluginRegisterIncomingHttpRequestFilter2().
 * -# <tt>void OrthancPluginFinalize()</tt>:
 *    This function is invoked by Orthanc during its shutdown. The plugin
 *    must free all its memory.
 * -# <tt>const char* OrthancPluginGetName()</tt>:
 *    The plugin must return a short string to identify itself.
 * -# <tt>const char* OrthancPluginGetVersion()</tt>:
 *    The plugin must return a string containing its version number.
 *
 * The name and the version of a plugin is only used to prevent it
 * from being loaded twice. Note that, in C++, it is mandatory to
 * declare these functions within an <tt>extern "C"</tt> section.
 * 
 * To ensure multi-threading safety, the various REST callbacks are
 * guaranteed to be executed in mutual exclusion since Orthanc
 * 0.8.5. If this feature is undesired (notably when developing
 * high-performance plugins handling simultaneous requests), use
 * ::OrthancPluginRegisterRestCallbackNoLock().
 **/



//#include <algorithm>
#include "../include/OrthancCPlugin.h"
#include "EmbeddedResources.h"
#include <string>
#include <stdio.h>
//#include "../include/database.hpp"
#include <map>
#include <algorithm>
#include <stdexcept>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include "../include/CookieHandler.hpp"
#include "../include/json.h"
#include "../include/filter.hpp"

static std::string databaseDefaultPath = "OE2.db";
static DatabaseOE DB(databaseDefaultPath);

static bool disableUsers = false;
static string tagDBOE2Path = "OE2DbPath";
static string tagOE2DisableUsers = "OE2DisableUsers";

static CookieHandler cookies;
static OrthancPluginContext* context = NULL;

static OrthancPluginErrorCode customError;

static time_t lastCheckTimestamp;
static double timeBetweenTwoChecks = 600;

/*
*This function takes in the path of an embedded file of the plugin
* It returns the MIME type of the file.
* This function comes from an example plugin offered by Orthanc
*/
static const char* GetMimeType(const std::string& path)
{
  size_t dot = path.find_last_of('.');

  std::string extension = (dot == std::string::npos) ? "" : path.substr(dot);
  std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

  if (extension == ".html")
  {
	return "text/html";
  }
  else if (extension == ".css")
  {
	return "text/css";
  }
  else if (extension == ".js")
  {
	return "application/javascript";
  }
  else if (extension == ".gif")
  {
	return "image/gif";
  }
  else if (extension == ".json")
  {
	return "application/json";
  }
  else if (extension == ".xml")
  {
	return "application/xml";
  }
  else if (extension == ".png")
  {
	return "image/png";
  }
  else if (extension == ".jpg" || extension == ".jpeg")
  {
	return "image/jpeg";
  }
  else
  {
	return "application/octet-stream";
  }
}


/*
  This function checks if the path of the database exist and if the databse can be open  
  HTTP argument:
  HTTP method: GET
  Permission: No
  Return:
    -true the database file exists and can be open
    -false: default return 
*/

bool checkDbPath(OrthancPluginContext* context)
{
  //init config
  Json::Reader reader;
  string path = string(OrthancPluginGetConfigurationPath(context));

  ifstream configFile(path);
  Json::Value obj;
  reader.parse(configFile, obj);

  //retrieve the path of the database
  if(obj.isMember(tagDBOE2Path))
  {
    string pathDb = obj[tagDBOE2Path].asString();
    ifstream testFile(pathDb);
    if(testFile)
      return true;
  }
  return false;
}


/*
  This callback checks if the plugin has a database file and if it is possible to open it.  
  HTTP argument:
  HTTP method: GET
  Permission: No
  Return:
    -the result of the check
*/

static OrthancPluginErrorCode existDBOE2Callback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{
  //check if the method is the one that is expected
  if (request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };

  Json::Value obj;
  obj["isInitialisated"] = checkDbPath(context);
  
  Json::FastWriter converter;
  string jsonString = converter.write(obj);

  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");

  return OrthancPluginErrorCode_Success;
}


/*
  This callback sets the path of the database and defined if user feature is disable or not. If not disable, it sets the first user with a SuperUser permission. This callback checks if the database exist so after the first use of the plugin, this callback is not used anymore unless the database file is deleted or the path of the database is manually modified in the config file. 
  HTTP argument:
    -pathDB: 
    - disableUser: boolean to define if the users feature is disable or not 
    if disableUser = false:
        -username: the name of the first user
        -password: the password of the first user
    if disableUser = false:
  HTTP method: POST
  Permission: No
  Return:
    -the content of the index file
*/

static OrthancPluginErrorCode initPluginCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{
  //check if the method is the one that is expected
  if (request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  if(checkDbPath(context))
    return OrthancPluginErrorCode_BadRequest;

  int errorCode;
  int SQLError;

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject); 
  //check if the fields exist
  if(bodyObject.isMember("pathDb") &&
      bodyObject.isMember("disableUsers") &&
      bodyObject["pathDb"].asString().length() > 0)
  {
    //retrieve the database path and the bool to enable or not the user
    string pathDirDb = bodyObject["pathDb"].asString();
    bool tmpDisableUsers = bodyObject["disableUsers"].asBool();
    //if enable user, check if the fields exist
    if((!tmpDisableUsers &&
          bodyObject.isMember("username") &&
          bodyObject.isMember("password") &&
          (bodyObject["username"].asString().length() > 0) &&
          (bodyObject["password"].asString().length() > 0) ) 
        || tmpDisableUsers)
    {
      //try to create the database
      string pathDb = pathDirDb + "OE2.db";
      DB.setPath(pathDb);
      SQLError = DB.open();

      if(SQLError == SQLITE_OK)
      {
        //set the path in the config
        string pathconfig= std::string(OrthancPluginGetConfigurationPath( context));
        ifstream configFile(pathconfig);

        //retrieve configuration json object
        Json::Value conf;
        reader.parse(configFile, conf);
        
        //modify configuration object
        conf[tagDBOE2Path] = pathDb;
        conf[tagOE2DisableUsers] = tmpDisableUsers;

        disableUsers = tmpDisableUsers;
        //write configuration object
        ofstream file_id;
        file_id.open(pathconfig);
        Json::StyledWriter styledWriter;
        file_id << styledWriter.write(conf);
        file_id.close();

        // ok
        errorCode = 0;
        
        // create the user  
        if(!tmpDisableUsers)
        {
          //add user;
          string username = bodyObject["username"].asString();
          string password = bodyObject["password"].asString();
          int errorDB = DB.addUser(username, password);
          if(errorDB == DB.CONST_DB_OE_OK)
          {
            int id_user = DB.retrieveUserId(username);
            errorDB  = DB.addUserPermission(id_user, permissionEnum("SuperUser"));
          }
          //If the implementation is correct, the following errorCode will never
          //be reach because the database was just created and so contains nothing 
          if(errorDB != DB.CONST_DB_OE_OK)
            errorCode = 6;
        }
      }
      else
        errorCode = 1; //sqlerror
    }
    else
    {
      if(!(bodyObject.isMember("username") &&
            bodyObject["username"].asString().length() > 0))
        errorCode = 4; //username not set
      else
        errorCode = 5; //password not set
    }
  }
  else
  {
    if(!(bodyObject.isMember("pathDb") &&
          bodyObject["pathDb"].asString().length( ) > 0))
      errorCode = 2; //database path not set
    else
      errorCode = 3; //disable user not set
  }

  Json::Value toSend;
  toSend["errorCode"] = errorCode;
  if(errorCode == 1)
    toSend["SQLError"] = SQLError;

  Json::FastWriter converter;
  string jsonString = converter.write(toSend);

  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;
  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  OrthancPluginSendHttpStatusCode(context, output, 200);
  return OrthancPluginErrorCode_Success;
}


/*
  This callback returns the content of the static resssources (the embedded frontend part) of the plugin.  
  HTTP argument:
  HTTP method: GET
  Permission: No
  Return:
    -the content of the asked file, if it exists
*/

static OrthancPluginErrorCode ServeStaticResource(OrthancPluginRestOutput* output,
												  const char* url,
												  const OrthancPluginHttpRequest* request)
{
  //check if the method is the one that is expected
  if (request->method != OrthancPluginHttpMethod_Get)
  {
	OrthancPluginSendMethodNotAllowed(context, output, "GET");
	return OrthancPluginErrorCode_Success;
  };
  
	std::string path = std::string(url).substr(4,std::string(url).length() -4);

  const char* mime = GetMimeType(path);

  try
  {

	std::string s;
	Orthanc::EmbeddedResources::GetDirectoryResource
	  (s, Orthanc::EmbeddedResources::STATIC_RESOURCES, path.c_str());

	const char* resource = s.size() ? s.c_str() : NULL;
	OrthancPluginAnswerBuffer(context, output, resource, s.size(), mime);
  }
  catch (std::runtime_error&)
  {
	std::string s = "Unknown static resource in plugin: " ;
	OrthancPluginLogError(context, s.c_str());
	OrthancPluginSendHttpStatusCode(context, output, 404);
  }

  return OrthancPluginErrorCode_Success;
}


/*
  This callback returns the content of the index file of the plugin. the file is emmbedded in the plugin  
  HTTP argument:
  HTTP method: GET
  Permission: No
  Return:
    -the content of the index file
*/

static OrthancPluginErrorCode IndexCallback(OrthancPluginRestOutput* output,
												  const char* url,
												  const OrthancPluginHttpRequest* request)
{
  
  if(request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };

  std::string path = "/index.html";
  const char* mime = GetMimeType(path);
  try
  {

  std::string s;
  Orthanc::EmbeddedResources::GetDirectoryResource
	(s, Orthanc::EmbeddedResources::STATIC_RESOURCES, path.c_str());

  const char* resource = s.size() ? s.c_str() : NULL;
  OrthancPluginAnswerBuffer(context, output, resource, s.size(), mime);
  }
  catch (std::runtime_error&)
  {
  std::string s = "Unknown static resource in plugin: " ;
  OrthancPluginLogError(context, s.c_str());
  OrthancPluginSendHttpStatusCode(context, output, 404);
  }
	  return OrthancPluginErrorCode_Success;
}


/*
  This function is called to filter the incoming request. 
  data used to filter:
    -method: the HTTP method
    -uri: the route
    -
  Return:
    - true if the users are disable
    - false if the users are esable
*/

ORTHANC_PLUGINS_API int FilterIncomingHttpRequest(OrthancPluginHttpMethod  method,
													  const char*              uri,
													  const char*              ip,
													  uint32_t                 headersCount,
													  const char* const*       headersKeys,
													  const char* const*       headersValues,
                            uint32_t                 getArgumentsCount,
                            const char* const*       getArgumentsKeys,
                            const char* const*       getArgumentsValues)
{

  /*if it is time to check for expired cookies, it must be done before using the cookie table so it is the very first instruction of the filter*/
  if(difftime(time(NULL), lastCheckTimestamp) > timeBetweenTwoChecks)
  {
    lastCheckTimestamp = cookies.checkExpiredCookie();   
  }

  //if user are disable the filter is not used
  if(disableUsers)
    return 1;
  
  /*
    if the query is about the existence of the database 
    the route cannot be handle in the filter function because the database may 
    be uninitialisated)
  */

  string toTest(uri);
  if(toTest.compare("/OE2/existDatabase") == 0 && 
      method == OrthancPluginHttpMethod_Get)
    return 1;

  uint32_t i;
  int ID = 0;
  //OrthancPluginLogWarning(context, uri);
  std::map<std::string,int>::iterator it;

  if (headersCount > 0)
  {
  	OrthancPluginLogInfo(context, "HTTP headers of an incoming REST request:");
  	for (i = 0; i < headersCount; i++)
  	{
  	 
  	 if(std::string(headersKeys[i])=="cookie") 
  	 	if(std::string(headersValues[i]).substr(0,4)=="OE2="||std::string(headersValues[i]).substr(0,4)=="oe2=")
  	 	{
  	 		ID=cookies.checkCookie(std::string(headersValues[i]).substr(4,std::string(headersValues[i]).length()-4));
  	 	}
  	}
  }
  int32_t resultOfFilter = routeFilter(uri, method, DB.retrieveUserPermission(ID));
  return resultOfFilter;
}


/*
  This callback checks if the user are disable or not 
  HTTP argument:
  HTTP method: GET
  Permission: No
  Return:
    - true if the users are disable
    - false if the users are esable
*/

static OrthancPluginErrorCode disableUsersCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };

  Json::Value obj;
  obj["disableUsers"] = disableUsers;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);

  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");

  return OrthancPluginErrorCode_Success;
}


/*
  This callback connects a user
  HTTP method: GET
  HTTP arguments:
    -username
    -password
  Route: /OE2/login
  Permission: No
  Return:
    -an OE2 cookie if the connection succeeds
    -the string "connection refused" if the connection failed
*/

static OrthancPluginErrorCode connectionCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("username") &&
        bodyObject.isMember("password") &&
        bodyObject["username"].asString().length() > 0 &&
        bodyObject["password"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;


  
  string username = bodyObject["username"].asString();
  string password = bodyObject["password"].asString();

  int id_user = DB.retrieveUserId(username);

  if(DB.existUserPassword(id_user, password))
    {
      string key=cookies.insertCookie(id_user);
      key= "oe2="+key +"; HttpOnly; SameSite=Strict;";
      
      OrthancPluginSetHttpHeader(context, output, std::string("Set-Cookie").c_str(), key.c_str());
      OrthancPluginSendHttpStatusCode(context, output, 200);
    }
  else
  {
    OrthancPluginSendUnauthorized(context, output, std::string("connection refused").c_str() );
  }
  return OrthancPluginErrorCode_Success;
}


/*
  This callback connects a user
  HTTP method: GET
  HTTP argument:
  Route: /OE2/disconnect
  Permission: No
  Return:
*/

static OrthancPluginErrorCode DisconnectCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{
  if(request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };

  for (unsigned int i = 0; i < request->headersCount; i++)
  {

   if(std::string(request->headersKeys[i])=="cookie") 
    if(std::string(request->headersValues[i]).substr(0,4)=="OE2="||std::string(request->headersValues[i]).substr(0,4)=="oe2=")
    {
      cookies.removeCookie(std::string(request->headersValues[i]).substr(4,std::string(request->headersValues[i]).length()-4));
    }
  }
    OrthancPluginSendHttpStatusCode(context, output, 200);
    return OrthancPluginErrorCode_Success;
}


/*
  This callback adds a tag of a study in the database 
  HTTP method: POST
  HTTP arguments: 
    -tag: the name of the tag
    -study: the id of the study
  Route: /OE2/addTagStudy
  Permission: Write
  Return:
*/

static OrthancPluginErrorCode addTagStudyCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("tag") &&
        bodyObject.isMember("study") && 
        bodyObject["tag"].asString().length() > 0 && 
        bodyObject["study"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;
  
  string tag = bodyObject["tag"].asString();
  string  study = bodyObject["study"].asString();

  int errorDB = DB.addStudyTag(study, tag);

   Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback removes a tag of a study in the database 
  HTTP method: POST
  HTTP arguments: 
    -tag: the name of the tag
    -study: the id of the study
  Route: /OE2/removeTagStudy
  Permission: Write
  Return:
*/

static OrthancPluginErrorCode removeTagStudyCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{
  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("tag") &&
        bodyObject.isMember("study") && 
        bodyObject["tag"].asString().length() > 0 && 
        bodyObject["study"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string tag = bodyObject["tag"].asString();
  string  study = bodyObject["study"].asString();

  int errorDB = DB.removeStudyTag(study, tag);

  Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback returns the list of studies linked to a tag 
  HTTP method: POST
  HTTP argument: 
    -tag: the name of the tag
  Route: /OE2/studiesFromTag
  Permission: Read
  Return:
    -the list of studies associated to the tag
*/

static OrthancPluginErrorCode studiesFromTagCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("tag") &&
        bodyObject["tag"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;


  string tag = bodyObject["tag"].asString();

  vector<string> studyList = DB.retrieveStudyListFromTag(tag);
  Json::Value obj;
  for(unsigned int i = 0; i < studyList.size(); i++)
    obj["studyList"].append(studyList[i]);
  Json::FastWriter converter;
  string jsonString = converter.write(obj);


  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;
  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");

  return OrthancPluginErrorCode_Success;
}


/*
  This callback returns the list of tags associated to a study
  HTTP method: GET
  HTTP argument: 
    -the id of the study at the end of the URL
  Route: /OE2/tags/{id_study}
  Permission: Read
  Return:
    -the list of tags associated to the study
*/

static OrthancPluginErrorCode StudyTagListCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if (request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };

  string id_study = string(url).substr(10,std::string(url).length() -4);
  vector<string> tagList = DB.retrieveStudyTagList(id_study);

  Json::Value obj;
  for(unsigned int i = 0; i < tagList.size(); i++)
    obj["tags"].append(tagList[i]);
  Json::FastWriter converter;
  string jsonString = converter.write(obj);



  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;
  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");

    return OrthancPluginErrorCode_Success;
}


/*
  This callback returns the list of users stored in the database
  HTTP method: GET
  HTTP argument:
  Route: /OE2/login
  Permission: No
  Return:
    -the list of users
*/

static OrthancPluginErrorCode getUsersCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };

  vector<string> userList = DB.getUsers();

  Json::Value obj;
  for(unsigned int i = 0; i < userList.size(); i++)
    obj["userList"].append(userList[i]);
  Json::FastWriter converter;
  string jsonString = converter.write(obj);

  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;
  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");

    return OrthancPluginErrorCode_Success;
}


/*
  This callback add a user in the database
  HTTP method: POST
  HTTP arguments:
    -username: the name of the user
    -password: the passord of the user
  Route: /OE2/addUser
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode addUserCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

	Json::Reader reader;
	Json::Value bodyObject;
	reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("username") &&
        bodyObject.isMember("password") && 
        bodyObject["username"].asString().length() > 0 && 
        bodyObject["password"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

	string username = bodyObject["username"].asString();
	string password = bodyObject["password"].asString();

  int errorDB = DB.addUser(username, password);

  Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback modifies the password of a user if the pair "username" "password" passed as argument matches an entry in the database
  HTTP method: POST
  HTTP arguments:
    -username: the name of the user
    -password: the passord of the user
    -nPassword: the new password of the user
  Route: /OE2/modifyUser
  Permission: No
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode modifyUserCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("username") &&
        bodyObject.isMember("password") && 
        bodyObject.isMember("nPassword") && 
        bodyObject["username"].asString().length() > 0 && 
        bodyObject["password"].asString().length() > 0 && 
        bodyObject["nPassword"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string username = bodyObject["username"].asString();
  string password = bodyObject["password"].asString();
  string nPassword = bodyObject["nPassword"].asString();
  int errorDB= DB.CONST_DB_OE_USER_NOT_EXIST;
  if(DB.existUserPassword(DB.retrieveUserId(username), password))
    errorDB = DB.modifyUser(username, username, nPassword);
  else
    errorDB = DB.CONST_DB_OE_USER_NOT_EXIST;

    Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback reinitialiase the password of a user with his username
  HTTP method: POST
  HTTP argument:
    -username: the name of the user
  Route: /OE2/reinitialiseUser
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode reinitialiseUserCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{


  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("username") &&
        bodyObject["username"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string username = bodyObject["username"].asString();

  int errorDB = DB.modifyUser(username, username, username);

   Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback removes a user in the database
  HTTP method: POST
  HTTP argument:
    -username: the name of the user
  Route: /OE2/removeUser
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode removeUserCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("username") &&
        bodyObject["username"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string username = bodyObject["username"].asString();

  int errorDB = DB.removeUser(DB.retrieveUserId(username));

  Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
    return OrthancPluginErrorCode_Success;
}


/*
  This callback returns the permission of the user that makes the request (from his cookie)
  HTTP method: GET
  HTTP argument:
  Route: /OE2/userdata
  Permission: No
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode UserPermissionCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };

  int id_user=0;
  for (unsigned int i = 0; i < request->headersCount; i++)
  {
   if(std::string(request->headersKeys[i])=="cookie") 
    if(std::string(request->headersValues[i]).substr(0,4)=="OE2="||std::string(request->headersValues[i]).substr(0,4)=="oe2=")
    {
      id_user=cookies.checkCookie(std::string(request->headersValues[i]).substr(4,std::string(request->headersValues[i]).length()-4));
    }
    
  }

  Json::Value obj;

  if(id_user == 0)
  {

    OrthancPluginSendHttpStatusCode(context, output, 418);
    return OrthancPluginErrorCode_Success;
  }

  string username = DB.retrieveUsername(id_user);
  vector<OE2Permission> permissions = DB.retrieveUserPermission(id_user);

  obj["username"] = username;
  for(unsigned int i = 0; i < permissions.size(); i++)
    obj["permissions"].append(permissionString(permissions[i]));
  

  Json::FastWriter converter;
  string jsonString = converter.write(obj);

  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;


  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");

    return OrthancPluginErrorCode_Success;
}


/*
  This callback returns the permission of a user
  HTTP method: POST
  HTTP argument:
    -username: the name of the user
  Route: /OE2/userPermissions
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode getUserPermissionCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Value obj;
  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("username") &&
        bodyObject["username"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string username = bodyObject["username"].asString();

  int id_user = DB.retrieveUserId(username);
  vector<OE2Permission> permissions = DB.retrieveUserPermission(id_user);

  for(unsigned int i = 0; i < permissions.size(); i++)
      obj["permissions"].append(permissionString(permissions[i]));

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");

    return OrthancPluginErrorCode_Success;
}


/*
  This callback adds a permission to a user in the database 
  HTTP method: POST
  HTTP arguments:
    -username: the name of the user
    -permission: the string associated to the permission
  Route: /OE2/addPermission
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode addUserPermissionCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{


  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

	Json::Reader reader;
	Json::Value bodyObject;
	reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("username") &&
        bodyObject.isMember("permission") && 
        bodyObject["username"].asString().length() > 0 && 
        bodyObject["permission"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

	string username = bodyObject["username"].asString();
	string permission = bodyObject["permission"].asString();
	

  int errorDB = DB.addUserPermission(DB.retrieveUserId(username), permissionEnum(permission));
  Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback removes a permission to a user in the database 
  HTTP method: POST
  HTTP arguments:
    -username: the name of the user
    -permission: the string associated to the permission
  Route: /OE2/removePermission
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode removeUserPermissionCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

	Json::Reader reader;
	Json::Value bodyObject;
	reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("username") &&
        bodyObject.isMember("permission") && 
        bodyObject["username"].asString().length() > 0 && 
        bodyObject["permission"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

	string username = bodyObject["username"].asString();
	string permission = bodyObject["permission"].asString();

  int errorDB = DB.removeUserPermission(DB.retrieveUserId(username), permissionEnum(permission));
   Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback adds a tag in the database 
  HTTP method: GET
  HTTP argument:
  Route: /OE2/tagsList
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode TagListCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if (request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };
 
  vector<string> tagList = DB.retrieveTagList();

  Json::Value obj;
  for(unsigned int i = 0; i < tagList.size(); i++)
    obj["tags"].append(tagList[i]);
  Json::FastWriter converter;
  string jsonString = converter.write(obj);

  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;
  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");

  return OrthancPluginErrorCode_Success;
}


/*
  This callback adds a tag in the database 
  HTTP method: POST
  HTTP argument:
    -tag: name of the tag
  Route: /OE2/addTag
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode addTagCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{
  //check if the method is the one that is expected
  if (request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);
  if(!(bodyObject.isMember("tag") && 
        bodyObject["tag"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string tag = bodyObject["tag"].asString();

  int errorDB = DB.addTag(tag);

   Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback modifies the name of a tag in the database 
  HTTP method: POST
  HTTP arguments:
    -tag: name of the tag
    -nTag: new name of the tag
  Route: /OE2/modifyTag
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode modifyTagCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("tag") &&
        bodyObject.isMember("nTag") &&
        bodyObject["tag"].asString().length() > 0 && 
        bodyObject["nTag"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string tag = bodyObject["tag"].asString();
  string nTag = bodyObject["nTag"].asString();


  int errorDB = DB.modifyTag(tag, nTag);

  Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
    return OrthancPluginErrorCode_Success;
}


/*
  This callback removes a tag in the database 
  HTTP method: POST
  HTTP argument:
    -tag: name of the tag
  Route: /OE2/removeTag
  Permission: SuperUser
  Return: The error code returned by the database
*/

static OrthancPluginErrorCode removeTagCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("tag") &&
        bodyObject["tag"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string tag = bodyObject["tag"].asString();


  int errorDB = DB.removeTag(tag);
  Json::Value obj;
  obj["errorCode"] = errorDB;

  Json::FastWriter converter;
  string jsonString = converter.write(obj);
  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;

  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
    return OrthancPluginErrorCode_Success;
}


/*
  This callback returns the list of modalities contained in the configuration file 
  HTTP method: GET
  HTTP argument:
  Route: /OE2/modalities
  Permission: SuperUser
  Return: The list of modalities
*/

static OrthancPluginErrorCode getModalitiesCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };


  string path= std::string(OrthancPluginGetConfigurationPath( context));
  ifstream configFile(path);

  Json::Reader reader;
  Json::Value obj;
  //parse
  reader.parse(configFile, obj);
  Json::Value value;
  value = obj["DicomModalities"];

  //DicomModalities object
  Json::Value modality;
  int j = 0;
  for (auto const& name : value.getMemberNames()) {
      modality[j]["name"] = name;
      modality[j]["aet"] = value[name][0].asString();
      modality[j]["ip"] = value[name][1].asString();
      modality[j]["port"] = value[name][2].asUInt();
      if(value[name].size() == 4)
        modality[j]["patch"] = value[name][3].asString();
      j++;
  }

  //convert to string
  Json::FastWriter fast;
  string modalityArrayString = fast.write(modality);
  const char* resource = modalityArrayString.size() ? modalityArrayString.c_str() : NULL;
  OrthancPluginAnswerBuffer(context, output, resource, modalityArrayString.size(), "application/json");
  return OrthancPluginErrorCode_Success;
}


/*
  This callback adds a modality in the configuration file 
  HTTP method: POST
  HTTP arguments:
    -name: the name of the modality
    -AET: the AET of the modality
    -IP: the IP of the modality
    -patch: the type of modality. the value of the parameter must be one of the following : 
            "Generic"
            "StoreScp"
            "ClearCanvas"
            "MedInria"
            "Dcm4Chee"
            "SyngoVia"
            "AgfaImpax"
            "EFilm2"
            "Vitrea"
  Route: /OE2/addModalities
  Permission: SuperUser
  Return:
*/

static OrthancPluginErrorCode addModalityCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("patch") &&
        bodyObject.isMember("name") &&
        bodyObject["patch"].asString().length() > 0 &&
        bodyObject["name"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string patch = bodyObject["patch"].asString();
  string name = bodyObject["name"].asString();


  string path= std::string(OrthancPluginGetConfigurationPath( context));
  ifstream configFile(path);
  
  Json::Value obj;
  reader.parse(configFile, obj);
  Json::Value value;
  if(!obj["DicomModalities"].isMember(name))
  {
    obj["DicomModalities"][name].append(bodyObject["AET"].asString());
    obj["DicomModalities"][name].append(bodyObject["IP"].asString());
    obj["DicomModalities"][name].append(bodyObject["port"].asUInt());
    if(patch.compare("undefined"))
    {
      if((patch.compare("Generic") == 0) ||
        (patch.compare("StoreScp") == 0) ||
        (patch.compare("ClearCanvas") == 0) ||
        (patch.compare("MedInria") == 0) ||
        (patch.compare("Dcm4Chee") == 0) ||
        (patch.compare("SyngoVia") == 0) ||
        (patch.compare("AgfaImpax") == 0) ||
        (patch.compare("EFilm2") == 0) ||
        (patch.compare("Vitrea") == 0))
      {
        obj["DicomModalities"][name].append(patch);  
      }
      else
      {
        return OrthancPluginErrorCode_BadParameterType;  
      }
    }
  }
  else
  {
    return OrthancPluginErrorCode_BadParameterType;
  }

  ofstream file_id;
  file_id.open(path);
  Json::StyledWriter styledWriter;
  file_id << styledWriter.write(obj);
  file_id.close();
  OrthancPluginSendHttpStatusCode(context, output, 200);
  return OrthancPluginErrorCode_Success;
}


/*
  This callback delete a modality in the configuration file 
  HTTP method: POST
  HTTP argument:
    -name: the name of the modality
  Route: /OE2/removeModality
  Permission: SuperUser
  Return:
*/

static OrthancPluginErrorCode deleteModalityCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{


  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("name") &&
        bodyObject["name"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string name = bodyObject["name"].asString();

  string path= std::string(OrthancPluginGetConfigurationPath( context));
  ifstream configFile(path);

  Json::Value obj;
  reader.parse(configFile, obj);
  if(obj["DicomModalities"].isMember(name))
  {
    obj["DicomModalities"].removeMember(name);
  }

  ofstream file_id;
  file_id.open(path);
  Json::StyledWriter styledWriter;
  file_id << styledWriter.write(obj);
  file_id.close();
  OrthancPluginSendHttpStatusCode(context, output, 200);
  return OrthancPluginErrorCode_Success;
}


/*
  This callback return the list of plugin paths from the configuration file 
  HTTP method: GET
  HTTP argument:
  Route: /OE2/pluginPath
  Permission: SuperUser
  Return:
    -the list of plugin paths
*/

static OrthancPluginErrorCode getPluginPathCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Get)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "GET");
    return OrthancPluginErrorCode_Success;
  };


  Json::Reader reader;
  string path= std::string(OrthancPluginGetConfigurationPath( context));
  ifstream configFile(path);

  Json::Value obj;
  reader.parse(configFile, obj);

  Json::Value toSend;

  for (int i = 0; i < obj["Plugins"].size(); i++)
  {
     toSend["paths"].append(obj["Plugins"][i]);
  }

  Json::FastWriter converter;
  string jsonString = converter.write(toSend);

  const char* resource = jsonString.size() ? jsonString.c_str() : NULL;
  OrthancPluginAnswerBuffer(context, output, resource, jsonString.size(), "application/json");
  OrthancPluginSendHttpStatusCode(context, output, 200);
  return OrthancPluginErrorCode_Success;
}


/*
  This callback adds a plugin path in the configuration file 
  HTTP method: POST
  HTTP argument:
    -path: the plugin path
  Route: /OE2/addPlugin
  Permission: SuperUser
  Return:
*/

static OrthancPluginErrorCode addPluginCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("path") &&
        bodyObject["path"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;


  string pluginPath = bodyObject["path"].asString();


  string path= std::string(OrthancPluginGetConfigurationPath( context));
  ifstream configFile(path);

  Json::Value obj;
  reader.parse(configFile, obj);
  Json::Value value;
  for (int i = 0; i < obj["Plugins"].size(); i++)
  {
    if(obj["Plugins"][i].compare(pluginPath) == 0)
    {
      string disp = "The path for the plugin is already used";
      OrthancPluginLogWarning(context, disp.c_str());  
      OrthancPluginSendHttpStatusCode(context, output, 200);
      return OrthancPluginErrorCode_Success;
    }
  }
  obj["Plugins"].append(pluginPath);


  ofstream file_id;
  file_id.open(path);
  Json::StyledWriter styledWriter;
  file_id << styledWriter.write(obj);
  file_id.close();

  OrthancPluginSendHttpStatusCode(context, output, 200);
  return OrthancPluginErrorCode_Success;
}


/*
  This callback deletes a plugin path in the configuration file 
  HTTP method: POST
  HTTP argument:
    -path: the plugin path to remove
  Route: /OE2/removePlugin
  Permission: SuperUser
  Return:
*/

static OrthancPluginErrorCode deletePluginCallback(OrthancPluginRestOutput* output,
                          const char* url,
                          const OrthancPluginHttpRequest* request)
{

  if(request->method != OrthancPluginHttpMethod_Post)
  {
    OrthancPluginSendMethodNotAllowed(context, output, "POST");
    return OrthancPluginErrorCode_Success;
  };

  Json::Reader reader;
  Json::Value bodyObject;
  reader.parse(string(request->body), bodyObject);

  if(!(bodyObject.isMember("path") &&
        bodyObject["path"].asString().length() > 0))
    return OrthancPluginErrorCode_BadRequest;

  string pluginPath = bodyObject["path"].asString();

  string path= std::string(OrthancPluginGetConfigurationPath( context));
  ifstream configFile(path);

  Json::Value obj;
  reader.parse(configFile, obj);

  vector<string> pluginList;
  //copy the data to keep
  for (int i = 0; i < obj["Plugins"].size(); i++)
  {
    if(obj["Plugins"][i].compare(pluginPath) != 0)
    {
      pluginList.push_back(obj["Plugins"][i].asString());
    }
  }
  //clear the array
  obj["Plugins"].clear();
  //copy the data
  for (int i = 0; i < pluginList.size(); i++)
  {
      obj["Plugins"].append(pluginList[i]);
  }

  ofstream file_id;
  file_id.open(path);
  Json::StyledWriter styledWriter;
  file_id << styledWriter.write(obj);
  file_id.close();
  OrthancPluginSendHttpStatusCode(context, output, 200);
  return OrthancPluginErrorCode_Success;
}


/*
* this extern "C" structure is obligatory to create a plugin working with Orthanc.
*/

extern "C"
{


  /*
    This function is the function called to initialize the plugin. It open the database if the path exists and define if the user feature is enable of not
    thanks to the configuration file
  */

  ORTHANC_PLUGINS_API int32_t OrthancPluginInitialize(OrthancPluginContext* c)
  {

    lastCheckTimestamp = time(NULL);
    OrthancPluginMemoryBuffer tmp;
    char info[1024];
    OrthancPluginDictionaryEntry entry;
    fillMaps();
      
    /*retrieve path of the database*/
    string path= std::string(OrthancPluginGetConfigurationPath(c));
    ifstream configFile(path);

    Json::Reader reader;
    Json::Value obj;
    reader.parse(configFile, obj);

    //check is the user are enable or disable or not (enable by default)
    if(obj.isMember(tagOE2DisableUsers) && 
        obj[tagOE2DisableUsers].asBool())
    {
      disableUsers = true;
    }

    if(checkDbPath(c))
    {
      DB.setPath(obj[tagDBOE2Path].asString());

      DB.open();
    }

    context = c;

    /* Register the callbacks */
    OrthancPluginRegisterRestCallback(context, "", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/""(.*)"".js", ServeStaticResource);
    OrthancPluginRegisterRestCallback(context, "/OE2/""(.*)"".json", ServeStaticResource);
    OrthancPluginRegisterRestCallback(context, "/OE2/""(.*)"".ico", ServeStaticResource);
    OrthancPluginRegisterRestCallback(context, "/OE2/""(.*)"".woff2", ServeStaticResource);
    OrthancPluginRegisterRestCallback(context, "/OE2/userdata" "(.*)", UserPermissionCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/config",  IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/home", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/plugin", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/patients", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/series", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/instances", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/upload", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/query", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/retrieve", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/connect", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/studies", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/redirect", IndexCallback);

    OrthancPluginRegisterRestCallback(context, "/OE2/addTagStudy", addTagStudyCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/removeTagStudy", removeTagStudyCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/login", connectionCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/addUser", addUserCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/addPermission", addUserPermissionCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/disconnect", DisconnectCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/removePermission", removeUserPermissionCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/removeUser", removeUserCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/tagsList", TagListCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/tags/" "(.*)", StudyTagListCallback); 
    OrthancPluginRegisterRestCallback(context, "/OE2/removeTag", removeTagCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/modifyTag", modifyTagCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/addTag", addTagCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/usernameList", getUsersCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/userPermissions", getUserPermissionCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/modifyUser", modifyUserCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/reinitialiseUser", reinitialiseUserCallback);

    OrthancPluginRegisterRestCallback(context, "/OE2", IndexCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/", IndexCallback);


    OrthancPluginRegisterRestCallback(context, "/OE2/modalities", getModalitiesCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/addModalities", addModalityCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/removeModality", deleteModalityCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/addPlugin", addPluginCallback); 
    OrthancPluginRegisterRestCallback(context, "/OE2/pluginPath", getPluginPathCallback);
    OrthancPluginRegisterRestCallback(context, "/OE2/removePlugin", deletePluginCallback);
    OrthancPluginRegisterRestCallback(context,"/OE2/studiesFromTag", studiesFromTagCallback);

    OrthancPluginRegisterRestCallback(context,"/OE2/initPlugin", initPluginCallback);
    
    OrthancPluginRegisterRestCallback(context,"/OE2/disableUsers", disableUsersCallback);
      /*
      the route is not handle by the filter function written in "filter.cpp", 
      because this function needs the the value returned by the database and the 
      database may be unitialisated when this route is call. It is a particular 
      case that is handle at the beginning of FilterIncomingHttpRequest
      */
    OrthancPluginRegisterRestCallback(context,"/OE2/existDatabase", existDBOE2Callback);
    OrthancPluginRegisterIncomingHttpRequestFilter2(context, &FilterIncomingHttpRequest);


    /* Make REST requests to the built-in Orthanc API */
    memset(&tmp, 0, sizeof(tmp));
    OrthancPluginRestApiGet(context, &tmp, "/changes");
    OrthancPluginFreeMemoryBuffer(context, &tmp);
    OrthancPluginRestApiGet(context, &tmp, "/changes?limit=1");
    OrthancPluginFreeMemoryBuffer(context, &tmp);
    
    /* Play with PUT by defining a new target modality. */
    sprintf(info, "[ \"STORESCP\", \"localhost\", 2000 ]");
    OrthancPluginRestApiPut(context, &tmp, "/modalities/demo", info, strlen(info));

    customError = OrthancPluginRegisterErrorCode(context, 4, 402, "Hello world");

    OrthancPluginRegisterDictionaryTag(context, 0x0014, 0x1020, OrthancPluginValueRepresentation_DA,
  									 "ValidationExpiryDate", 1, 1);

    OrthancPluginLookupDictionary(context, &entry, "ValidationExpiryDate");
    OrthancPluginLookupDictionary(context, &entry, "0010-0010");
    return 0;
  }

  /*
    This function is the function called to finalize the plugin. It closes the database
  */

  ORTHANC_PLUGINS_API void OrthancPluginFinalize()
  {
    DB.close();
    OrthancPluginLogWarning(context, "Sample plugin is finalizing");
  }


  /*
    This function return the name of the plugin
  */

  ORTHANC_PLUGINS_API const char* OrthancPluginGetName()
  {
    return "OE2";
  }


  /*
    This function return the version of the plugin
  */

  ORTHANC_PLUGINS_API const char* OrthancPluginGetVersion()
  {
    return "1.0";
  }
}