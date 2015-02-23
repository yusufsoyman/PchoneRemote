/*
 * Logger.cpp
 * Main Class to manage Logging operations
 * Created by: Ozgur Pekcagliyan - 2014-09-28 08:19:56 PM EEST
 * Last edited by: Ozgur Pekcagliyan - 2014-10-25 11:02:03 PM EEST
 * Notes:
 * logLevel will be used to define level of verbosing
 * * Potential error codes are;
 * * * 0: no log will be printed
 * * * 1: information logs will be printed
 * * * 2: along with level 1, warnings and errors will be printed too
 * * * 3: along with level 1 and 2, debug logs will be printed too
 * FIXME: What if user forgets to initialize logger? Maybe singleton is a better idea?
 */

#include <string>
#include <fstream>
#include <stdlib.h>
#include <ctime>


using std::string;
using std::ofstream;
using std::endl;
using std::time_t;


#include "Logger.h"


std::ofstream Logger::out;
bool Logger::isLogSet;
int Logger::logLevel; //logLevel

inline void getDate(char *dateBuf)
{
	time_t t = time(0);   // get current time in seconds
	struct tm * now = localtime( & t ); // turn time into local time
	sprintf(dateBuf, "%d:%d:%d %d:%d:%d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
}


bool Logger::printInfoLog(const string log) // function for info logs
{
	char dateBuf[21];
	getDate(dateBuf);
	if(logLevel >= INFO)
	{
		string temp = dateBuf;
		temp += "\tINFO\t" + log;
		Logger::printLog(temp);
	}
}
bool Logger::printWarnLog(const string log) // function will print warn level logs
{
	char dateBuf[21];
	getDate(dateBuf);
	if(logLevel >= ERROR)
	{
		string temp = dateBuf;
		temp += "\tWARNING\t" + log;
		Logger::printLog(temp);
	}
}
bool Logger::printErrorLog(const string log) //function will print error level logs
{
	char dateBuf[21];
	getDate(dateBuf);
	if(logLevel >= ERROR)
	{
		string temp = dateBuf;
		temp += "\tERROR\t" + log;
		Logger::printLog(temp);
	}
}
bool Logger::printDebugLog(const string log) //function will print debug level logs
{
	char dateBuf[21];
	getDate(dateBuf);
	if(logLevel >= DEBUG)
	{
		string temp = dateBuf;
		temp += "\tDEBUG\t" + log;
		Logger::printLog(temp);
	}
}
void Logger::setLogConfig(const string path, const string fileName, const int level, bool append)
{
	if(isLogSet != true)
	{
		isLogSet = true;
		string temp(path);
		temp += '/' + fileName;
		if(append == true)
		{
			out.open(temp.c_str(), ofstream::out|ofstream::app);
		}
		else
		{
			out.open(temp.c_str(), ofstream::out);
		}
		logLevel = level;
	}
	else
	{
		printErrorLog("Can't re-set log path");
	}
}

Logger::Logger() //it is not allowed to have an instance of this class
{
}

bool Logger::printLog(const string &log)
{
	out<<log<<endl;
	out.flush();
}

void Logger::finalize ()
{
	out.close();
}
