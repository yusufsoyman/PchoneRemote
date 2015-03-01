/* 
 * File:   ServerMain.h
 * Author: zgrw
 * This class will do all the job
 * Created on March 1, 2015, 5:52 PM
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
    pair<sockaddr_in, bool> stat = make_pair(remote, false);
    pair<int, pair<sockaddr_in, bool> > conn = make_pair(fd, stat);
    connTracker.insert(conn);
    char buffer[512];
    sprintf(buffer, "%s - %d: New connection from IP: %s", __FILE__, __LINE__, inet_ntoa(remote.sin_addr));
    logger -> printDebugLog(buffer);
}

void ServerMain::onRecieve(const int& fd, unsigned char* data, const int& size)
{
    //CONN_HANDLE_TYPE::iterator it = connTracker.find(fd);
    pair<sockaddr_in, bool> sec = connTracker[fd].second;
    if(sec.second == false) //if this is a new connection
    {
        if(strncmp(data, HELLO_MSG, 2)) //if just searching for a server
        {
            send(fd, (unsigned char*)HELLO_MSG, 2);
            closeConnection(fd);
        }
        else if(strncmp(data, INIT_MSG, 2)) //if tries to initialize a connection
        {
            if(inControl == false)
            {
                
            }
            else
            {
                send(fd, (unsigned char*)DENY_MSG, 2); //can't initialize, already in control
                closeConnection(fd);
            }
        }
    }
    else
    {
        
    }
}
