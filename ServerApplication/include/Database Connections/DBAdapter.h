/*
 * DBAdapter.h
 * Main Class to manage DB operations
 * Created by: Ozgur Pekcagliyan - 2014-09-25 09:28:43 PM EEST
 * Last edited by: Ozgur Pekcagliyan - 2015-02-24
 * Notes:
 * errorCode will be used to return specific error values back to user;
 * * Potential error codes are;
 * * * 0: success
 * * * 1: user name or password is incorrect
 * * * 2: db doesn't exist
 * * * 3: invalid sql query
 * * * 4: problem with disconnect
 * * * 5: failed to create sqlfile
 * * * 6-1000: unknown (reserved)
 * * * bigger than 1000 - SQL server error system (Ex: 1045000 means 1045)
 * type will hold the database that will be connected
 * * possible values are;
 * * * 1: mysql (default value)
 * * * 2: oracle
 * * * 3: sqlite
 * * * rest of the codes will be added in time by developers
 * In selectData function, data will be returned in a string vector, fields will be seperated by a user defined char
 * selectData function appendFlag will decide wethrer returnVal will be cleared or results will be appended to the returnVal
 * * 0: no append (default val)
 * * 1: append
*/

#ifndef DBADAPTER_H
#define DBADAPTER_H
#include <mysql/mysql.h>
#include <sqlite3.h> 
#include <string>
#include <vector>
#include <list>

// using namespace std; //FIXME:Remove this line and change everything to std::

#define DEFAULT_SQLITE_DBNAME "defaultSQDB.db"

class DBAdapter
{
public:
    enum db_types_t {MYSQL = 1, ORACLE = 2, SQLITE = 3};


    DBAdapter(); //Default constructor
    explicit DBAdapter(const db_types_t type); //constructor to set db type at initialization, imlicit initialization is not allowed
    ~DBAdapter(); //Destructor

    bool setDbType(const int &ty); //function to change db type in runtime, if there is an active connection, this function will not run

    bool connect (const char *ip, const int port, const char *user, const char *pass, int &errorCode); //connects database and resturns the result
    bool connect (const char *filename = DEFAULT_SQLITE_DBNAME, int &errorCode = 0); //connects database and resturns the result for sqlite
    bool disconnect (int &errorCode); //disconnect from database and return the result
    bool selectDB(const std::string &dbName, int &errorCode); //selecting DB to connect
    //bool execQuery(const string &sql, int &errorCode, char * reason); //execute a specific user written query, also returns a failure reason as set of chars
    bool execQuery(const std::string &sql, int &errorCode); //overloaded version
    bool execQuery(const void *sql, const int& querySize, int &errorCode); //overloaded version which accepts binary data
    bool insertData(const std::string &fields, const std::string& values, const std::string &table, int &errorCode); /*inserts data into specific tables
                                                            Example usage:
                                                            insertData("name, surname, age", "'john', 'smith', 52", "users", errorCodeVar);
                                                            */
    bool selectData(const std::string &fields, const std::string& condition, const std::string &table, std::vector< std::list< std::string > > &returnVal, int &errorCode, int appendFlag = 0); /*select data from specific tables
                                                            Example usage:
                                                            selecttData("name, surname, age", "name='john' or age < 52", "users", errorCodeVar);
                                                            */
    bool selectData(const std::string &fields, const std::string &table, std::vector< std::list< std::string > >  &returnVal, int &errorCode, int appendFlag = 0); //overloaded version of select
    bool deleteData(const std::string& condition, const std::string &table, int &errorCode); /*delete data from specific tables
                                                            Example usage:
                                                            deleteData("name=john and age >= 63", "users", errorCodeVar);
                                                            */
    bool updateData(const std::string &fields, const std::string& values, const std::string &condition, const std::string &table, int &errorCode); /*inserts data to specific tables
                                                            Example usage
                                                            updatetData("name, surname, age", "'john', 'smith', 52", "name=john and age >= 63", "users", errorCodeVar);
                                                            */
    bool updateData(const std::string &fields, const std::string& values, const std::string &table, int &errorCode); // overloaded version
private:
	
    int type; //this variable will hold the type of connection
    bool isConnected; //holds the status of connection

    //MYSQL variables start here
    ::MYSQL *myConnection, myInit; //used for mysql operations and holds connection, myInit is not a pointer because we don't want to handle memory operations for it - Ozgur
    //MYSQL_RES *myResult; //holds results of the queries
    //MYSQL_ROW myRow; //holds rows for each result

    //ORACLE variables will be here

    //SQLITE variables and functions will be here
    sqlite3 *SQlitedb;
    int callBack(void *data, int argc, char **argv, char **azColName);
    //Other DB variables will be here

    //Here starts special functions
	
};

#endif