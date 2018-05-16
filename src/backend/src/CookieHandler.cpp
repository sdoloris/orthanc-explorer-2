#include "../include/CookieHandler.hpp"


string CookieHandler::genRandomString()
{
	string randomString;
    for (int i = 0; i < 128; i++)
        randomString += alphabet[rand() % alphabet.length()];
    return randomString;
}


CookieHandler::CookieHandler()
{
	srand(time(NULL));
}



string CookieHandler::insertCookie(int id_user)
{
	string key;
	do
	{
		key = genRandomString();
	} 
	while(table.find(key) != table.end());
	table.insert(std::make_pair(key, std::make_pair(id_user, time(NULL))));
	return key;
}




int CookieHandler::checkCookie(string key)
{
	it = table.find(key); 
	if(it != table.end())
	{
		time_t currentTime = time(NULL);

		if(difftime(currentTime, it->second.second) > lifeTime)
		{

			table.erase(it);
			return 0;
		}
		it->second.second = currentTime;
		return it->second.first;
	}
	return 0;
}



bool CookieHandler::removeCookie(string key)
{
	it = table.find(key); 
	if(it == table.end())
		return false;
	table.erase(it);
	return true;
}



time_t CookieHandler::checkExpiredCookie()
{
	time_t currentTime = time(NULL);
	it = table.begin(); 
	while(it != table.end())
	{
		if(difftime(currentTime, it->second.second) > lifeTime)
			table.erase(it);
		it++;
	}
	return currentTime;

}



string CookieHandler::insertExpiredCookie(int id_user, time_t expirationTime)
{
	string key;
	do
	{
		key = genRandomString();
	} 
	while(table.find(key) != table.end());
	table.insert(std::make_pair(key, std::make_pair(id_user, time(NULL)-expirationTime)));
	return key;
}



string CookieHandler::readTab()
{
	cout << "Lecture table: " << endl;
	it = table.begin();
	string toreturn;
    while(it != table.end())
    {
        toreturn += "Clé = ";
        toreturn += it->first;
        toreturn += " id_User = "; 
        toreturn += to_string(it->second.first); 
        toreturn += " lifeTime = ";
        toreturn += to_string(it->second.second);
        it++;
    }
    cout << toreturn << endl;
    return toreturn;
}


time_t CookieHandler::getTimeStamp(string key)
{
	it = table.find(key); 
	if(it != table.end())
		return it->second.second;
	return 0;
}

time_t CookieHandler::getLifeTime()
{
	return (time_t) lifeTime;
}
