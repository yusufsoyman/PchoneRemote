/*
 * DBAdapter.cpp
 * Main Class to manage DB operations
 * Created by: Ozgur Pekcagliyan - 2014-09-26 09:34:38 PM EEST
 * Last edited by: Ozgur Pekcagliyan - 2015-03-01
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
 * * * rest of the codes will be added in time by developers
 * In selectData function, data will be returned in a string vector, fields will be seperated by a user defined char
 * selectData function appendFlag will decide wethrer returnVal will be cleared or results will be appended to the returnVal
 * * 0: no append (default val)
 * * 1: append
*/


#include <string>
#include <cstring>
#include <csignal>
#include <cstdio>
#include <vector>
#include <list>
#include <mysql/mysql.h>
#include <sqlite3.h> 



using std::string;
using std::vector;
using std::list;
using std::istringstream;


#include "DBAdapter.h"
#include "Logger.h"


DBAdapter::DBAdapter()
:type(MYSQL), isConnected(false), SQlitedb(NULL)
{
    //Default constructor
    logger = Logger::getInstance();
    mysql_init(&myInit);
}
DBAdapter::DBAdapter(const db_types_t ty)
:type(ty), isConnected(false), SQlitedb(NULL)
{
    //A very cool constructor :) // Ozgur
    logger = Logger::getInstance();
    //constructor to set db type at initialization, imlicit initialization is not allowed
    if(type == MYSQL)//Mysql
    {
            mysql_init(&myInit);
    }
    else if (type == SQLITE)
    {
        //No need to do anything for now.
    }
    else
    {
        // FIXME: Add others here
    }
}
DBAdapter::~DBAdapter()
{
    if(isConnected)
    {
        int errorCode;
        disconnect(errorCode);
        if(errorCode != 0) //NO!!!! a Giant problem here :/
        {
            char buffer[1024];
            sprintf(buffer, "%s - %d: Can't clean connection. Raising SIGTERM immediately", __FILE__, __LINE__);
            logger -> printErrorLog(buffer);
            raise(SIGTERM);
        }
        else
        {
            isConnected = false;
        }
    }
    else
    {
        // Don't do anything yet
    }
}

bool DBAdapter::setDbType(const int &ty) //function to change db type in runtime, if there is an active connection, this function will not run
{
    if(isConnected == true)
    {
        int errorCode;
        disconnect(errorCode); //we should disconnect communication if there is any
    }
    type = ty;
    return true;
}

bool DBAdapter::connect (const char *ip, const int port, const char *user, const char *pass, int &errorCode) //connects database and resturns the result
{
    char buffer[1024];
    sprintf(buffer, "%s - %d: Connection request recieved", __FILE__, __LINE__);
    logger -> printInfoLog(buffer);
    if(type == MYSQL)//Mysql
    {
        myConnection = mysql_real_connect(&myInit, ip, user, pass, NULL , port, NULL, 0);
        int err = mysql_errno(&myInit);
        if(err != 0)
        {
            sprintf(buffer, "%s - %d: Can't connect to mysql DB", __FILE__, __LINE__);
            logger -> printDebugLog(buffer);
            if(err == 1045)
            {
                errorCode = 1;
            }
            else
            {
                errorCode = err * 1000; //FIXME: Add every possible error code
            }
            isConnected = false;
            return false;
        }
        else
        {
            sprintf(buffer, "%s - %d: MySQL connection is established", __FILE__, __LINE__);
            logger -> printInfoLog(buffer);
            isConnected = true;
            errorCode = 0;
            return true;
        }
    }
    else if (type == SQLITE)
    {
        sprintf(buffer, "%s - %d: Ommitting IP, port, user, pass information since this is SQLITE connection", __FILE__, __LINE__);
        logger -> printWarnLog(buffer);
        return connect(DEFAULT_SQLITE_DBNAME, errorCode);
    }
    else
    {
        return true;
            //Do other DB connections
    }
}

bool DBAdapter::connect(const char *filename, int &errorCode)
{
    char buffer[1024];
    sprintf(buffer, "%s - %d: Connection request recieved", __FILE__, __LINE__);
    logger -> printInfoLog(buffer);
    if (type != SQLITE)
    {
        sprintf(buffer, "%s - %d: Wrong DB function access. DB is not SQLITE", __FILE__, __LINE__);
        logger -> printErrorLog(buffer);
        errorCode = 5;
        return false;
    }
    else
    {
        sprintf(buffer, "%s - %d: Opening SQLITE db: %s", __FILE__, __LINE__, filename);
        logger -> printInfoLog(buffer);
        int res;
        res = sqlite3_open(filename, &SQlitedb);
        if(res != SQLITE_OK)
        {
            sprintf(buffer, "%s - %d: Can't open DB: %s", __FILE__, __LINE__,   sqlite3_errmsg(SQlitedb));
            logger -> printInfoLog(buffer);
            isConnected = false;
            if(res == SQLITE_CANTOPEN)
            {
                errorCode = 5;
            }
            else
            {
                errorCode = res * 1000;
            }
            return false;
        }
        else
        {
            sprintf(buffer, "%s - %d: SQLITE connection is established", __FILE__, __LINE__);
            logger -> printInfoLog(buffer);
            isConnected = true;
            errorCode = 0;
            return true;
        }
    }
}



bool DBAdapter::disconnect (int &errorCode) //disconnect from database and return the result
{
    char buffer[1024];
    sprintf(buffer, "%s - %d: Disconnect request recieved", __FILE__, __LINE__);
    logger -> printInfoLog(buffer);
    if(type == MYSQL) //Mysql
    {
        mysql_close(myConnection); //will be freed bt mysql_close
                                   //This function doesn't return any error and always succeeds.
        isConnected = false;
        errorCode = 0;
        sprintf(buffer, "%s - %d: MySQL connection terminated", __FILE__, __LINE__);
        logger -> printInfoLog(buffer);
        return true;
    }
    else
    {
        if(type == SQLITE)
        {
            int res;
            res = sqlite3_close(SQlitedb);
            if (res == SQLITE_OK)
            {
                sprintf(buffer, "%s - %d: SQLite connection terminated", __FILE__, __LINE__);
                logger -> printInfoLog(buffer);
                isConnected = false;
                errorCode = 0;
                return true;
            }
            else
            {
                sprintf(buffer, "%s - %d: SQLite connection terminated", __FILE__, __LINE__);
                logger -> printInfoLog(buffer);
                isConnected = true;
                errorCode = 4;
                return false;
            }
        }
        else
        {
            //Other DBs
        }
        return true;
        //Do other DB disconnections
    }
}

bool DBAdapter::selectDB(const string &dbName, int &errorCode) //selecting DB to connect
{
    char buffer[1024];
    sprintf(buffer, "%s - %d: DB selection procedure initiated", __FILE__, __LINE__);
    logger -> printInfoLog(buffer);
    if(type == MYSQL) //Mysql
    {
        if(mysql_select_db(myConnection, dbName.c_str()) != 0)
        {
            int err = mysql_errno(&myInit);
            char buffer[1024];
            sprintf(buffer, "%s - %d: Can't select DB", __FILE__, __LINE__);
            logger -> printErrorLog(buffer);
            errorCode = err * 1000; //FIXME: Add every possible error code
            return false;
        }
        else
        {
            char buffer[1024];
            sprintf(buffer, "%s - %d: DB %s successfuly selected", __FILE__, __LINE__, dbName.c_str());
            logger -> printInfoLog(buffer);
            errorCode = 0;
            return true;
        }
    }
    else if (type == SQLITE) //no need to select db
    {
        errorCode = 0;
        return true;
    }
    else
    {
        errorCode = 0;
        return true;
    }
}

int callBack(void* data, int argc, char** argv, char** azColName)
{
    DBAdapter *val = (DBAdapter*)data;
    if(argc > 0)
    {
        int i;
        list<string> templist;
        templist.clear();
        for (i = 0; i < argc; ++i)
        {
            string temp;
            if(argv[i] != NULL)
            {
                temp = argv[i];
            }
            else
            {
                temp = "N/A";
            }
            templist.push_back(temp);
        }
        val->rVal->push_back(templist);
    }
    return 0;
}

bool DBAdapter::execQuery(const string &sql, int &errorCode) //overloaded version
{
    char buffer[1024];
    if(type == MYSQL) //Mysql
    {
        mysql_query(myConnection, sql.c_str());
        int err = mysql_errno(&myInit);
        if(err != 0)
        {
            sprintf(buffer, "%s - %d: Can't execute the sql query", __FILE__, __LINE__);
            logger -> printErrorLog(buffer);
            sprintf(buffer, "%s - %d: Problem with query \"%s\"", __FILE__, __LINE__, sql.c_str());
            logger -> printDebugLog(buffer);
            errorCode = err * 1000; //FIXME: Add every possible error code
            return false;
        }
        else
        {
            sprintf(buffer, "%s - %d: SQL Query successfuly executed on MySQL DB\n\"%s\"", __FILE__, __LINE__, sql.c_str());
            logger -> printDebugLog(buffer);
            errorCode = 0;
            return true;
        }
    }
    else if(type == SQLITE)
    {
        return execQuery((void*)sql.c_str(), sql.size(), errorCode);
    }
    else
    {
            errorCode = 0;
            return true;
    }
}

bool DBAdapter::execQuery(const void *sql, const int& querySize, int &errorCode) //overloaded version which accepts binary data
{
    char buffer[1024];
    if(type == MYSQL) //Mysql
    {
        mysql_real_query(myConnection, static_cast<const char*>(sql), querySize);
        int err = mysql_errno(&myInit);
        if(err != 0)
        {
            sprintf(buffer, "%s - %d: Can't execute the sql query. Return code: %d", __FILE__, __LINE__, err);
            logger -> printErrorLog(buffer);
            sprintf(buffer, "%s - %d: Problem with query \"%s\"", __FILE__, __LINE__, static_cast<const char*>(sql));
            logger -> printDebugLog(buffer);
            errorCode = err * 1000; //FIXME: Add every possible error code
            return false;
        }
        else
        {
            sprintf(buffer, "%s - %d: SQL Query successfuly executed on MySQL DB\n\"%s\"", __FILE__, __LINE__, static_cast<const char*>(sql));
            logger -> printDebugLog(buffer);
            errorCode = 0;
            return true;
        }
    }
    else if(type == SQLITE)
    {
        char *dummy = NULL;
        errorCode = sqlite3_exec(SQlitedb, static_cast<const char*>(sql), callBack, (void*)this, &dummy) * 1000; //FIXME: Dangerous to pass this pointer. Think something more sensible
        if(errorCode != 0)
        {
            sprintf(buffer, "%s - %d: Can't execute the sql query. Return code: %d. Message is: %s.", __FILE__, __LINE__, errorCode, dummy);
            logger -> printErrorLog(buffer);
            sprintf(buffer, "%s - %d: Problem with query \"%s\"", __FILE__, __LINE__, static_cast<const char*>(sql));
            logger -> printDebugLog(buffer);
            return false;
        }
        else
        {
            sprintf(buffer, "%s - %d: SQL Query successfuly executed on MySQL DB\n\"%s\"", __FILE__, __LINE__, static_cast<const char*>(sql));
            logger -> printDebugLog(buffer);
            return true;
        }
    }
    else
    {
        errorCode = 0;
        return true;
    }
}
bool DBAdapter::insertData(const string &fields, const string& values, const string &table, int &errorCode) /*inserts data into specific tables
							Example usage:
							insertData("name, surname, age", "'john', 'smith', 52", "users", errorCodeVar);
							*/
{
//  int queryLength = fields.size() + values.size() + table.size();
    string sql = "insert into " + table + " ( " + fields + " ) " + " values ( " + values + " )";
    logger -> printDebugLog(sql.c_str());
    return execQuery(static_cast<const void*>(sql.c_str()), sql.size() + 1, errorCode);
}
bool DBAdapter::selectData(const string &fields, const string& condition, const string &table, vector< list< string > >  &returnVal, int &errorCode, int appendFlag) /*select data from specific tables
							Example usage:
							selecttData("name, surname, age", "name='john' or age < 52", "users", errorCodeVar);
							*/
{
    if(appendFlag == 0)
    {
            returnVal.clear(); //vector should be cleared in order to return healthy results
    }
    string sql = "select " + fields + " from " + table + " where " + condition;
    if(type == SQLITE)
    {
        rVal = &returnVal;
    }
    bool rValue = execQuery(static_cast<const void*>(sql.c_str()), sql.size(), errorCode);
    if(rValue == false)
    {
            return false;
    }
    if(type == MYSQL) //Mysql
    {
            MYSQL_RES *result = mysql_store_result(myConnection);
            MYSQL_ROW row;
            list<string> templist;

            int fieldNum = mysql_num_fields(result);
            while(row = mysql_fetch_row(result))
            {
                    templist.clear();
                    for(int i = 0; i < fieldNum; i++)
                    {
                            string temp = row[i];
                            templist.push_back(temp);
                    }
                    returnVal.push_back(templist);
            }
    }
    else
    {
        errorCode = 0;
        return true;
    }
}
bool DBAdapter::selectData(const string &fields, const string &table, vector< list< string > >  &returnVal, int &errorCode, int appendFlag)
{
    return selectData(fields, "1 = 1", table, returnVal, errorCode, appendFlag);
}
bool DBAdapter::deleteData(const string& condition, const string &table, int &errorCode) /*delete data from specific tables
							Example usage:
							deleteData("name=john and age >= 63", "users", errorCodeVar);
							*/
{
    string sql = "delete from " + table + " where " + condition;
    return execQuery(sql, errorCode);
}
bool DBAdapter::updateData(const string &fields, const string& values, const string &condition, const string &table, int &errorCode) /*inserts data to specific tables
							Example usage
							updatetData("name, surname, age", "'john', 'smith', 52", "name=john and age >= 63", "users", errorCodeVar);
							*/
{
    string sql = "update " + table + " set ";
    string tempFields(fields);
    string tempValues(values);
    string fieldTok, valueTok;
    size_t fieldPos, valuePos;
    while((fieldPos = tempFields.find(", ")) != string::npos && (valuePos = tempValues.find(", ")) != string::npos)
    {
        fieldTok = tempFields.substr(0, fieldPos);
        valueTok = tempValues.substr(0, valuePos);
        sql += fieldTok + " = " + valueTok + " , ";
        tempValues.erase(0, valuePos + 2);
        tempFields.erase(0, fieldPos + 2);
    }
    sql += tempFields + " = " + tempValues + ' ';
    sql += " where " + condition; 
    return execQuery(sql, errorCode);
}
bool DBAdapter::updateData(const string &fields, const string& values, const string &table, int &errorCode) // overloaded version
{
    return updateData(fields, values, "1 = 1", table, errorCode);
}