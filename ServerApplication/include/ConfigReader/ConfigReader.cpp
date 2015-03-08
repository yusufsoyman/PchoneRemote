/* 
 * File:   ConfigReader.h
 * Author: zgrw
 *
 * Created on March 2, 2015, 8:54 PM
 * Last Editr: March 3, 2015
 */
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <exception>
//#include "rapidxml.hpp"
#include "XmlParser.h"
#include "ConfigReader.h"
#include "Logger.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::exception;
using std::ios;
//using namespace rapidxml;
using namespace XmlSpace;

ConfigReader::ConfigReader()
:configPath("./pchone.cfg"), port(DEFAULT_PORT)
{
    char buffer[5];
    int pass = rand() % 9000 + 1000;
    sprintf(buffer, "%d", pass);
    passwd = buffer;
    logger = Logger::getInstance();
}

ConfigReader::~ConfigReader()
{
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


ConfigReader* ConfigReader::getInstance()
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
        sprintf(buffer, "%s - %d: Config file %s is corrupted\n\t\t\t\tRewriting it with default configuration.", __FILE__, __LINE__, configPath.c_str());
        logger -> printErrorLog(buffer);
        in.close();
        createConfig();
    }
    else
    {
        char data[256];
        //bool invalid = false;
        in.read(data, size);
        in.close();
        XmlParser myParser;
        myParser.loadXmlTree(data);
        bool rVal;
        rVal = myParser.findNode("port");
        if(rVal == false)
        {
            //port = DEFAULT_PORT;
            //Do nothing, already set the value in constructor
        }
        else
        {
            string res;
            myParser.getNodeValue(res);
            port = atoi(res.c_str());
        }
        
        rVal = myParser.findNode("passwd");
        if(rVal == false)
        {
            //Do nothing, already set the value in constructor
        }
        else
        {
            myParser.getNodeValue(passwd);
        }
        /*xml_document<> conf;
        try
        {
            conf.parse<0>(data);
        }
        catch (exception &e) //
        {
            sprintf(buffer, "%s - %d: Can't parse config file %s, might be corrupted\n\t\t\t\tRewriting it with default configuration\n\t\t\t\tError message: %s", __FILE__, __LINE__, configPath.c_str(), e.what());
            logger -> printErrorLog(buffer);
            createConfig();
            invalid = true;
        }
        if(!invalid)
        {
            xml_node<>* head = conf.first_node("pchone");
            if(head == NULL)
            {
                sprintf(buffer, "%s - %d: Config file %s is corrupted\n\t\t\t\tRewriting it with default configuration.", __FILE__, __LINE__, configPath.c_str());
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
                    if(strncmp(childeren->name(), "port", 4))
                    {
                        port = atoi(childeren->value());
                    }
                    else if(strncmp(childeren->name(), "passwd", 4))
                    {
                        passwd = childeren->value();
                    }
                    else
                    {
                        sprintf(buffer, "%s - %d: Unrecognized config %s in config file %s.", __FILE__, __LINE__, childeren->name(), configPath.c_str());
                        logger -> printWarnLog(buffer);
                    }
                    childeren = childeren -> next_sibling();
                }
                if(c == 0)
                {
                    sprintf(buffer, "%s - %d: Config file %s is corrupted\n\t\t\t\tRewriting it with default configuration.", __FILE__, __LINE__, configPath.c_str());
                    logger -> printErrorLog(buffer);
                    createConfig();
                }
            }
        }*/
    }
}

/*bool ConfigReader::validateConfig()
{

}*/

bool ConfigReader::createConfig()
{
    char buffer[1024];
    sprintf(buffer,"<pchone>\n\t<port>%d</port>\n\t<passwd>%s</passwd>\n</pchone>", port, passwd.c_str());
    ofstream out(configPath, ios::out);
    out<<buffer;
    out.flush();
    out.close();
    return true;
}

const string & ConfigReader::getPasswd()
{
    return passwd;
}
