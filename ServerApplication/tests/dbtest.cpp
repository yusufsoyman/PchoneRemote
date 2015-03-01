/* 
 * File:   dbtest.cpp
 * Author: zgrw
 *
 * Created on Feb 24, 2015, 9:30:23 PM
 */

// DB tester below

#include <iostream>
#include <vector>
#include <string>
#include <signal.h>
#include "DBAdapter.h"
#include "Logger.h"
#include <stdlib.h>

using namespace std;


DBAdapter dbConn (DBAdapter::SQLITE);

void sigHandler(int sig)
{
        char buffer[128];
        sprintf(buffer, "%s - %d: SIGTERM caught", __FILE__, __LINE__);
        Logger::printInfoLog(buffer);
        Logger::finalize();
        int dummy;
        dbConn.disconnect(dummy);
        signal(sig, SIG_DFL);
        raise(sig);
}


int main(int argv, char **argc)
{
        signal(SIGTERM, sigHandler);
        signal(SIGINT, sigHandler);
        Logger::setLogConfig("./","test.log", Logger::DEBUG);
//      DBAdapter dbConn (DBAdapter::MYSQL);
        int err;

        dbConn.connect("test.db", err);
        if(err !=0 )
        {
                cout<<err<<endl;
                return 1;
        }
        
        
        dbConn.execQuery("CREATE TABLE test ( id int(11) , name varchar(20) ,surname varchar(20))", err);

        
        
        dbConn.insertData("id, name, surname", "1, 'sdf', '534'", "test", err);

        vector< list< string > >  myresult;
        dbConn.selectData("id, name, surname", "test", myresult, err);
        for(int i = 0; i < myresult.size(); i++)
        {
                for(list <string>::iterator it = myresult[i].begin(); it != myresult[i].end(); it++)
                {
                        cout<<*it<<' ';
                }
                cout<<endl;
        }

        dbConn.deleteData("id > 15", "test", err);
        dbConn.updateData("name, surname", "'jane', 'doe'", "test", err);

        dbConn.selectData("id, name, surname", "test", myresult, err);
        for(int i = 0; i < myresult.size(); i++)
        {
                for(list <string>::iterator it = myresult[i].begin(); it != myresult[i].end(); it++)
                {
                        cout<<*it<<' ';
                }
                cout<<endl;
        }
        dbConn.disconnect(err);
//      cout<<err<<endl;
        return 0;
}


