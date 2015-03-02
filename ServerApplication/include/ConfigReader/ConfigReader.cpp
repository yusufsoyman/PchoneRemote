/* 
 * File:   ConfigReader.h
 * Author: zgrw
 *
 * Created on March 2, 2015, 8:54 PM
 */
#include <fstream>
#include <string>
#include <cstdlib>
#include "rapidxml.hpp"
#include "ConfigReader.h"

using namespace std;
using namespace rapidxml;

ConfigReader::ConfigReader()
:configPath("./pchone.cfg"), port(DEFAULT_PORT)
{
    char buffer[5];
    int pass = rand() % 9000 + 1000;
    sprintf(buffer, "%d", pass);
    passwd = buffer;
}

ConfigReader::~ConfigReader()
{

}

ConfigReader::ConfigReader(const std::string& configPath)
:configPath(configPath), port(DEFAULT_PORT)
{
    char buffer[5];
    int pass = rand() % 9000 + 1000;
    sprintf(buffer, "%d", pass); //always assign a default random password, just in case
    passwd = buffer;
}

void ConfigReader::parseConfig()
{
    ifstream in(configPath, ios::in);
    in.seekg(0, ios::end);
    int size = in.tellg();
    in.seekg(0, ios::beg);
    string data;
    in.read(data, size);
}

bool ConfigReader::validateConfig()
{

}

bool ConfigReader::createConfig()
{

}
