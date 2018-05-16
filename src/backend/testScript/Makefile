filter : build/database.o build/CookieHandler.o build/filter.o testFiles/filter/filterTest.cpp testFiles/filter/main.cpp
		g++ -std=c++11 -o filter testFiles/filter/filterTest.cpp testFiles/filter/main.cpp build/database.o build/CookieHandler.o build/filter.o -lcppunit -lsqlite3 -Wall -g; ./filter

database : build/database.o testFiles/database/main.cpp testFiles/database/databaseTest.cpp
		g++ -std=c++11 -o database testFiles/database/main.cpp testFiles/database/databaseTest.cpp build/database.o -lcppunit -lsqlite3 -Wall -g; ./database

cookie : testFiles/cookie/main.cpp testFiles/cookie/cookieTest.cpp build/CookieHandler.o
		g++ -std=c++11 -o cookie testFiles/cookie/main.cpp testFiles/cookie/cookieTest.cpp build/CookieHandler.o -lcppunit -Wall -g; ./cookie

build/database.o : ../src/database.cpp
		g++ -std=c++11 -c ../src/database.cpp -o build/database.o

build/CookieHandler.o : ../src/CookieHandler.cpp
		g++ -std=c++11 -c ../src/CookieHandler.cpp -o build/CookieHandler.o

build/filter.o : ../src/filter.cpp
		g++ -std=c++11 -c ../src/filter.cpp -o build/filter.o