/* 
 * File:   ConfigReader.h
 * Author: zgrw
 *
 * Created on March 2, 2015, 8:54 PM
 */

#ifndef CONFIGREADER_H
#define	CONFIGREADER_H

#include <string>
#include "rapidxml.hpp"

class ConfigReader
{
public:
    ConfigReader();
    ConfigReader(const std::string& configPath);
    virtual ~ConfigReader();
    bool validateConfig();
    void parseConfig();
    int getPort() {return port;}
    std::string getPasswd(){return passwd;}
private:
    std::string configPath;
    int port;
    std::string passwd;
};

#endif	/* CONFIGREADER_H */

