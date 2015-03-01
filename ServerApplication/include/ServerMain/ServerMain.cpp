/* 
 * File:   ServerMain.h
 * Author: zgrw
 * This class will do all the job
 * Created on March 1, 2015, 5:52 PM
 */
#include "NetworkHandler.h"
#include "Logger.h"
#include "SoundController.h"
#include "ServerMain.h"

ServerMain::ServerMain()
{
    logger = Logger::getInstance();
}

void ServerMain::onClose(const int& fd, sockaddr_in& remote)
{
    //don't do anything for now
}

void ServerMain::onConnect(const int& fd, sockaddr_in& remote)
{
    //don't do anything for now
}

void ServerMain::onRecieve(const int& fd, unsigned char* data, const int& size)
{
    
}