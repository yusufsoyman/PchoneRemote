/* 
 * File:   ConfigReader.h
 * Author: zgrw
 *
 * Created on March 2, 2015, 8:54 PM
 */
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <exception>
#include "rapidxml.hpp"
#include "ConfigReader.h"
#include "Logger.h"

using std::string;
using std::ifstream;
using std::exception;
using std::ios;
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
    logger = Logger::getInstance();
}
/*ConfigReader::ConfigReader(const std::string& configPath)
:configPath(configPath), port(DEFAULT_PORT)
{
    char buffer[5];
    int pass = rand() % 9000 + 1000;
    sprintf(buffer, "%d", pass); //always assign a default random password, just in case
    passwd = buffer;
    logger = Logger::getInstance();
}*/


ConfigReader& ConfigReader::getInstance()
{
    static ConfigReader instance;
    return &instance;
}

void ConfigReader::setConfig(const char* configPath)
{
    this -> configPath = configPath;
}


void ConfigReader::parseConfig()
{
    char buffer[512];
    ifstream in(configPath, ios::in);
    in.seekg(0, ios::end);
    int size = in.tellg();
    in.seekg(0, ios::beg);
    if(size <= 0)
    {
        sprintf(buffer, "%s - %d: Config file %s is corrupted\n\t\t\t\tRewriting it with default configuration.", __FILE__, __LINE__, configPath);
        logger -> printErrorLog(buffer);
        in.close();
        createConfig();
    }
    else
    {
        string data;
        bool invalid = false;
        in.read(data, size);
        xml_document<> conf;
        in.close();
        try
        {
            conf.parse(const_cast<char*>(data.c_str()));
        }
        catch (exception &e) //
        {
            createConfig();
            sprintf(buffer, "%s - %d: Config file %s is corrupted\n\t\t\t\tRewriting it with default configuration.", __FILE__, __LINE__, configPath);
            logger -> printErrorLog(buffer);
            invalid = true;
        }
        if(!invalid)
        {
            xml_node<>* head = conf.first_node("pchone");
            if(head == NULL)
            {
                sprintf(buffer, "%s - %d: Config file %s is corrupted\n\t\t\t\tRewriting it with default configuration.", __FILE__, __LINE__, configPath);
                logger -> printErrorLog(buffer);
                createConfig();
            }
            else
            {
                xml_node<>* childeren = head->first_node();
                int c = 0;
                while(childeren != NULL)
                {
                    ++c;
                    if(strcmp(childeren->name(), "port", 4))
                    {
                        port = atoi(childeren->value());
                    }
                    else if(strcmp(childeren->name(), "passwd", 4))
                    {
                        passwd = childeren->value();
                    }
                    else
                    {
                        sprintf(buffer, "%s - %d: Unrecognized config %s in config file %s.", __FILE__, __LINE__, childeren->name(), configPath);
                        logger -> printWarnLog(buffer);
                    }
                    childeren = childeren -> next_sibling();
                }
                if(c == 0)
                {
                    sprintf(buffer, "%s - %d: Config file %s is corrupted\n\t\t\t\tRewriting it with default configuration.", __FILE__, __LINE__, configPath);
                    logger -> printErrorLog(buffer);
                    createConfig();
                }
            }
        }
    }
}

/*bool ConfigReader::validateConfig()
{

}*/

bool ConfigReader::createConfig()
{
    char buffer[1024];
    sprintf(buffer,"<pchone><port>%d</port><passwd>%s</passwd></pchone>", port, passwd.c_str());
    ofstream out(configPath, ios::out);
    out<<buffer;
    out.flush();
    out.close();
    return true;
}

std::string ConfigReader::getPasswd() {
    return passwd;
}
