/* 
 * File:   main.cpp
 * Author: zgrw
 *
 * Created on February 12, 2015, 11:07 PM
 */


#include "ServerMain.h"
#include "Logger.h"
#include "ConfigReader.h"
#include <csignal>


Logger *logger;
ServerMain server;

void sigHandler(int sig)
{
    char buffer[128];
    if(sig == SIGTERM)
    {
        sprintf(buffer, "%s - %d: SIGTERM caught", __FILE__, __LINE__);
    }
    else
    {
        sprintf(buffer, "%s - %d: SIGINT caught", __FILE__, __LINE__);
    }
    logger -> printInfoLog(buffer);
    server.shutdown();
    logger -> finalize();
    signal(sig, SIG_DFL);
    raise(sig);
}


int main(int argc, char** argv)
{
    signal(SIGTERM, sigHandler);
    signal(SIGINT, sigHandler);
    logger = Logger::getInstance();
    logger -> setLogConfig("./","pchoneremote.log", Logger::INFO, true);
    ConfigReader *cfReader = ConfigReader::getInstance();
    if(argc > 1)
    {
        cfReader -> setConfig(argv[1]);
    }
    cfReader -> parseConfig();
    int port = cfReader->getPort();
    
    server.listen(port, 10); // don't need many clients. Only a few is enough
    server.startSever();
    //server.shutdown(); //no need to call this, if start server quits, this means this function is already called
    logger -> finalize();
    
    return 0;
}

