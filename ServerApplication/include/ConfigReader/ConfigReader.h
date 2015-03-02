/* 
 * File:   ConfigReader.h
 * Author: zgrw
 *
 * Created on March 2, 2015, 8:54 PM
 */

#ifndef CONFIGREADER_H
#define	CONFIGREADER_H

#include <string>
#include "Logger.h"
//#include "rapidxml.hpp"

#define DEFAULT_PORT 63783

class ConfigReader
{
public:
    static ConfigReader * getInstance();
    void setConfig(const char *configPath = NULL);
    //ConfigReader(const std::string& configPath);
    virtual ~ConfigReader();
    //bool validateConfig();
    void parseConfig();
    int getPort() {return port;}
    std::string getPasswd();
    bool createConfig();
    const std::string & getPasswd();
private:
    ConfigReader();
    std::string configPath;
    int port;
    std::string passwd;
    Logger *logger;
};

#endif	/* CONFIGREADER_H */

