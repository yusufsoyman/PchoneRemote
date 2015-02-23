/*
 * Logger.h
 * Main Class to manage Logging operations
 * Created by: Ozgur Pekcagliyan - 2014-09-25 09:28:43 PM EEST
 * Last edited by: Ozgur Pekcagliyan - 2014-10-25
 * Notes:
 * logLevel will be used to define level of verbosing
 * * Potential error codes are;
 * * * 0: no log will be printed
 * * * 1: information logs will be printed
 * * * 2: along with level 1, warnings and errors will be printed too
 * * * 3: along with level 1 and 2, debug logs will be printed too
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

inline void getDate(char *dateBuf);

class Logger
{
public:
	enum log_level_t {NO_LOG = 0, INFO = 1, ERROR = 2, DEBUG = 3};

	static bool printInfoLog(const std::string log); // function for info logs
	static bool printWarnLog(const std::string log); // function will print warn level logs
	static bool printErrorLog(const std::string log); //function will print error level logs
	static bool printDebugLog(const std::string log); //function will print debug level logs
	static void setLogConfig(const std::string path, const std::string fileName, const int level, bool append = true); //will configure log system to necessary settings
	static void finalize (); //closes log file
private:
	Logger(); //it is not allowed to have an instance of this class
	static bool printLog(const std::string &log);

	//static std::string logFilePath; // will hold the path of log file
	//static std::string logFileName; // will hold the name of log file
	static std::ofstream out;
	static bool isLogSet;
	static int logLevel; //logLevel
};



#endif