/* 
 * File:   ServerMain.h
 * Author: zgrw
 * This class will do all the job
 * Created on March 1, 2015, 5:52 PM
 * Last edited on 2015-03-02
 */

#include <utility>
#include <cstdio>
#include <cstring>

using std::pair;
using std::make_pair;

#include "NetworkHandler.h"
#include "Logger.h"
#include "SoundController.h"
#include "ServerMain.h"

ServerMain::ServerMain()
:inControl(false)
{
    logger = Logger::getInstance();
}

void ServerMain::onClose(const int& fd, sockaddr_in& remote)
{
    connTracker.erase(fd); //FIXME: need to check if this guy was in charge or not
    char buffer[512];
    sprintf(buffer, "%s - %d: Connection closed from IP: %s", __FILE__, __LINE__, inet_ntoa(remote.sin_addr));
    logger -> printDebugLog(buffer);
}

void ServerMain::onConnect(const int& fd, sockaddr_in& remote)
{
    pair<sockaddr_in, bool> stat = make_pair(remote, NEW);
    pair<int, pair<sockaddr_in, bool> > conn = make_pair(fd, stat);
    connTracker.insert(conn);
    char buffer[512];
    sprintf(buffer, "%s - %d: New connection from IP: %s", __FILE__, __LINE__, inet_ntoa(remote.sin_addr));
    logger -> printDebugLog(buffer);
}

void ServerMain::onRecieve(const int& fd, unsigned char* data, const int& size)
{
    char buffer[512];
    //CONN_HANDLE_TYPE::iterator it = connTracker.find(fd);
    pair<sockaddr_in, int> &sec = connTracker[fd].second;
    if(sec.second == NEW) //if this is a new connection
    {
        if(strncmp(data, HELLO_MSG, size)) //if just searching for a server
        {
            sprintf(buffer, "%s - %d: %s said \"HELLO\"", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
            logger -> printDebugLog(buffer);
            send(fd, (unsigned char*)HELLO_MSG, 2);
            sprintf(buffer, "%s - %d: Responded to IP %s as \"HELLO\"", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
            logger -> printDebugLog(buffer);
            closeConnection(fd);
        }
        else if(strncmp(data, INIT_MSG, size)) //if tries to initialize a connection
        {
            sec.second = INIT;
            sprintf(buffer, "%s - %d: Initialization request from IP %s", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
            logger -> printInfoLog(buffer);
            if(inControl == false)
            {
                sprintf(buffer, "%s - %d: Handshake request sent to IP %s", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
                logger -> printInfoLog(buffer);
                send(fd, (unsigned char *)HANDSHK_REQ_MSG, 2);
            }
            else
            {
                sprintf(buffer, "%s - %d: Server is already being controlled. Illegal request from IP %s", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
                logger -> printWarnLog(buffer);
                send(fd, (unsigned char*)DENY_MSG, 2); //can't initialize, already in control
                closeConnection(fd);
            }
        }
    }
    else if(sec.second == INIT)
    {
        //FIXME: Read config file and wait for right password
    }
    else
    {
        
    }
}
