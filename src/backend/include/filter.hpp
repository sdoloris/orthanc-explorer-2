#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "OrthancCPlugin.h"
#include "database.hpp"

using namespace std;


static std::map<std::string, OE2Permission> get_route_permission;
static std::map<std::string, OE2Permission> post_route_permission;
static std::map<std::string, OE2Permission> put_route_permission;
static std::map<std::string, OE2Permission> delete_route_permission;



/*
	This function fill the static map of the file to create the set of data used by the filter function to allow or not a user to use a route. Each map represents a HTTP method allowed by the Orthanc server. Each entry maps a route with the required permission to used it.
	Parameter:
	Output: 
*/

void fillMaps();

/*
	This function allow a user to use a route or not depending of the method and the set of permissions passed as parameters. The method is the one used by the user to request the route. The set of permissions is the set of permissions of the user
	Parameters:
    - uri: the route to filter
    - method: the http method (delete, get, post, put) used
    - permission: a vector containing all the permission of the user
	Output: 
    - 0: the user is not allowed to use the route 'uri' 
    - 1: the user is allowed to use the route 'uri' 
*/

int32_t routeFilter(const char* uri, OrthancPluginHttpMethod method, vector<OE2Permission> permissionTab);

string getbegining(string uri);
