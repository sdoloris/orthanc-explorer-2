#include "../include/filter.hpp"


void fillMaps()
{
  //encoding all routes used in plugin by us
	get_route_permission["/patients"]=OE2Permission_Read;
  get_route_permission["/patients/protected"]=OE2Permission_Read;
  put_route_permission["/patients/protected"]=OE2Permission_Read;
  
  get_route_permission["/patients/studies"]=OE2Permission_Read;
  get_route_permission["/studies"]=OE2Permission_Read;
  get_route_permission["/modalities"]=OE2Permission_Read;
  get_route_permission["/instances/patients"]=OE2Permission_Read;
  get_route_permission["/instances/studies"]=OE2Permission_Read;
  get_route_permission["/instances/series"]=OE2Permission_Read;
  get_route_permission["/series/patient"]=OE2Permission_Read;
  get_route_permission["/series/study"]=OE2Permission_Read;
  get_route_permission["/series/instances"]=OE2Permission_Read;
  get_route_permission["/studies/series"]=OE2Permission_Read;
  get_route_permission["/studies/patients"]=OE2Permission_Read;
  get_route_permission["/plugins"]=OE2Permission_Read;
  get_route_permission["/statistics"]=OE2Permission_Read;
  get_route_permission["/system"]=OE2Permission_SuperUser;
  get_route_permission["/plugins"]=OE2Permission_SuperUser;
  get_route_permission["/instances/simplified-tags"]=OE2Permission_Read;

  post_route_permission["/tools/find"]=OE2Permission_Read;
  post_route_permission["/patients/media"]=OE2Permission_Read;
  post_route_permission["/studies/media"]=OE2Permission_Read;
  post_route_permission["/series/media"]=OE2Permission_Read;
  post_route_permission["/patients/archive"]=OE2Permission_Read;
  post_route_permission["/studies/archive"]=OE2Permission_Read;
  post_route_permission["/series/archive"]=OE2Permission_Read;
  post_route_permission["/instances/file"]=OE2Permission_Read;
  post_route_permission["/instances/tags"]=OE2Permission_Read;
  post_route_permission["/queries/answers"]=OE2Permission_Write;
  post_route_permission["/queries/retrieve"]=OE2Permission_Write;
  post_route_permission["/patients/anonymize"]=OE2Permission_Write;
  post_route_permission["/studies/anonymize"]=OE2Permission_Write;
  post_route_permission["/series/anonymize"]=OE2Permission_Write;
  post_route_permission["/instances"]=OE2Permission_Write;
  post_route_permission["/OE2/modifyUser"]=OE2Permission_No;

  delete_route_permission["/patients"]=OE2Permission_Delete;
  delete_route_permission["/studies"]=OE2Permission_Delete;
  delete_route_permission["/series"]=OE2Permission_Delete;

  //routes created specially for OE2 and which not requiered the superuser permission
  get_route_permission["/"]=OE2Permission_No;
  get_route_permission[""]=OE2Permission_No;
  get_route_permission["/OE2/tags"]=OE2Permission_Read;
  get_route_permission["/OE2/config"]=OE2Permission_No;
  get_route_permission["/OE2"]=OE2Permission_No;
  get_route_permission["/OE2/disconnect"]=OE2Permission_No;
  get_route_permission["/OE2/home"]=OE2Permission_No;
  get_route_permission["/OE2/plugin"]=OE2Permission_No;
  get_route_permission["/OE2/patients"]=OE2Permission_No;
  get_route_permission["/OE2/series"]=OE2Permission_No;
  get_route_permission["/OE2/instances"]=OE2Permission_No;
  get_route_permission["/OE2/upload"]=OE2Permission_No;
  get_route_permission["/OE2/query"]=OE2Permission_No;
  get_route_permission["/OE2/retrieve"]=OE2Permission_No;
  get_route_permission["/OE2/connect"]=OE2Permission_No;
  get_route_permission["/OE2/localisation"]=OE2Permission_No;
  get_route_permission["/OE2/userdata"]=OE2Permission_No;
  get_route_permission["/OE2/studies"]=OE2Permission_No;
  get_route_permission["/OE2/tagsList"]=OE2Permission_Read;

  post_route_permission["/OE2/addTagStudy"]=OE2Permission_Read;
  post_route_permission["/OE2/removeTagStudy"]=OE2Permission_Read;
  post_route_permission["/OE2/login"]=OE2Permission_No;
  post_route_permission["/OE2/studiesFromTag"]=OE2Permission_Read;

  post_route_permission["/OE2/initPlugin"]=OE2Permission_No;
  get_route_permission["/OE2/disableUsers"]=OE2Permission_No;

}



int32_t routeFilter(const char* uri, OrthancPluginHttpMethod method, vector<OE2Permission> permissionsTab)
{
 /*
  string method;

	switch(idMethod)
  {
    case 1:
      method="get";
    break;
    case 2:
      method="post";
    break;
    case 3:
      method="put";
    break;
    case 4:
      method=OE2Permission_Delete;
    break;
  }*/
  
 
	/*
		Convert permission to boolean
	*/

 	map<OE2Permission,bool> permissionsMap;

  permissionsMap[OE2Permission_Read] = false;
  permissionsMap[OE2Permission_Write] = false;
  permissionsMap[OE2Permission_Delete] = false;
  permissionsMap[OE2Permission_SuperUser] = false;
  permissionsMap[OE2Permission_No] = true;

  for(unsigned int i = 0; i < permissionsTab.size(); i++)
  	permissionsMap[permissionsTab[i]] = true;

  /*
		Check if the user is superuser
  */

  if(permissionsMap[OE2Permission_SuperUser])
  	return 1;

	/*
		Check if the route requires a permission 
	*/

/*  it = no_route_permission.find(std::string(uri));
  if (it != no_route_permission.end())
    return 1;
  else */
/*  if((string(uri).substr(0,5)=="/OE2/"&& string(uri).length() == 5)||
  		( string(uri).substr(0,4)=="/OE2" && 
  			(string(uri).length() == 4 || 
      		string(uri).substr(string(uri).length()-3,3)==".js" ||
					string(uri).substr(std::string(uri).length()-5,5)==".json" ||
					string(uri).substr(std::string(uri).length()-4,4)==".ico" ||
					string(uri).substr(std::string(uri).length()-6,6)==".woff2")))
    return 1;*/

  if(string(uri).substr(0,5)=="/OE2/" &&  
     (string(uri).length() == 5 ||
     	string(uri).substr(string(uri).length()-3,3)==".js" ||
			string(uri).substr(std::string(uri).length()-5,5)==".json" ||
			string(uri).substr(std::string(uri).length()-4,4)==".ico" ||
			string(uri).substr(std::string(uri).length()-6,6)==".woff2"))
    return 1;

	map<string,OE2Permission> map_route;

if(method == OrthancPluginHttpMethod_Delete)
  map_route = delete_route_permission;
else if(method == OrthancPluginHttpMethod_Get)
  map_route = get_route_permission;
else if(method == OrthancPluginHttpMethod_Post)
  map_route = post_route_permission;
else if(method == OrthancPluginHttpMethod_Put)
  map_route = put_route_permission;
else
  return 0;


  std::map<std::string, OE2Permission>::iterator it;
	it = map_route.find(string(uri));

	if (it != map_route.end())
	{

		/*
			If the whole uri matches a route, check if the user has the right permission
	  */

		if(permissionsMap[map_route[uri]])
  	  return 1;
  	return 0;
	}
  
	/*
		suppress the "useless" part of the uri
	*/

  string begining_uri = getbegining(string(uri));
  it = map_route.find(begining_uri);
  
  /*
		if the route exist, check if the user has the right permission
  */
	
	if (it != map_route.end() &&
			permissionsMap[map_route[begining_uri]])
	  return 1;
	
	/*
		Deny by default
	*/
	
	return 0;  	
}


/*
* This function takes in a route and remove from it the part which is dependant
*
*to the request, such as a patient id, as it cannot be mapped with the id.
*
*/

string getbegining(string uri)
{
  uri= uri +"/";
  std::string toreturn;

  if(uri.substr(0,3)=="/OE")
  {
    toreturn=uri.substr(0,5);
    uri=uri.substr(5,uri.length()-5);
  }
  else
  {
    toreturn=uri.substr(0,1);
    uri=uri.substr(1,uri.length()-1);
  }
  int pos=uri.find("/");
  toreturn = toreturn+ uri.substr(0,pos);
  uri=uri.substr(pos+1, uri.length()-(pos+1));
  pos=uri.find("/");
  uri=uri.substr(pos+1, uri.length()-(pos+1));
  pos=uri.find("/");
  if(pos ==-1)
  {
    return toreturn;
  }
  toreturn+= "/";
  toreturn = toreturn + uri.substr(0,pos);
  return toreturn;
  
}
