#include <iostream>
#include <string>
#include <map>
#include <ctime>

using namespace std;

class CookieHandler
{

	private:
		std::map< string, std::pair<int, time_t>> table;
		std::map< time_t, string> timerTable;
    std::map< string, std::pair<int, time_t>>::iterator it;
    string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   	double lifeTime = 3600;

  /*
    All the methods have been declared as public to allow an a deeper testing and an easier test implementation
  */

	public:

		/*
			This function creates a cookie, which is a unique string of 256 characters.
			Parameter:
			Return:
				- The created string
		*/

    string genRandomString();


    /*
			This is the constructor of the class it initialisates the function "rand" used to create the random string.
		*/

		CookieHandler();


    /*
			This function creates an entry in the "CookieHandler". The table stores the user id (id in the database) and the timestamp at which the entry has been created.
			The function is only used after a successful connection.
			Parameter:
				- id_user: the id of the user
			Return:
				- The key of the entry in the table 
		*/
		
		string insertCookie(int id_user);


  	/*
			This function checks if a key has an entry in the cookie table and if the entry is not expired (if it is, the entry is removed). If the entry is not expired, its timestamp is updated with the current timestamp.
			Parameter:
				- key :the key of an entry
			Return:
				- non nul value: the user id stored in the entry that matches the key.
				- nul value: No entry matches the key or there is an entry but the timestamp is expired (the previous check/insertion is to long ago)
		*/
	
		int checkCookie(string key);


		/*
			This function remove an entry in the CookieHandler
			Parameter:
				- key: the key of the entry
			Return:
				- true: the entry has been found and remove
				- false: no entry match the key
		*/

		bool removeCookie(string key);


		/*
			This function checks if there are expired cookies in the table and deletes them
			Parameter:
			Return:
				- the timestamp of the check
		*/

    time_t checkExpiredCookie();


    /*Functions for test and debug */


		/*
			This function creates mock entry as if the entry has been created for a certain time. It is used in the cookieHandler tests to verify the behaviour of the 'checkExpiredCookie' method
			Parameter:
				- id_user:the id of the user to add
				- expirationTime: the laps of time. The function creates an entry as if it has been created "the laps of time" before
			Return:
				- The key of the entry in the table
		*/

		string insertExpiredCookie(int id_user, time_t expirationTime);

		/*
			This function display all field of each entry of the table.
		*/

    string readTab();

    /*GETTERS*/

    time_t getTimeStamp(string key);

    time_t getLifeTime();


    
};


