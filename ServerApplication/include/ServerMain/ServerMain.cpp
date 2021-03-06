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

#ifdef WIN32
#include "winsock2.h"
#else
#include <sys/socket.h> //for sockek operations
#include <netinet/in.h> //sockaddr_in members
#include <sys/select.h>
#include <arpa/inet.h> //for inet_ntoa function
#include <unistd.h> //for close
//#include <sys/types.h> // older OS's might require this header too. If can't compile, uncomment this line
#endif


using std::pair;
using std::make_pair;

#include "NetworkHandler.h"
#include "Logger.h"
#include "SoundController.h"
#include "ServerMain.h"
#include "ConfigReader.h"

ServerMain::ServerMain()
:inControl(false)
{
    logger = Logger::getInstance();
}

ServerMain::~ServerMain()
{
    
}

void ServerMain::onClose(const int &fd, const struct sockaddr_in &remote)
{
    pair<sockaddr_in, int> &sec = connTracker[fd];
    if(sec.second == APPRVED)
    {
        inControl = false;
    }
    connTracker.erase(fd);
    char buffer[512];
    sprintf(buffer, "%s - %d: Connection closed from IP: %s", __FILE__, __LINE__, inet_ntoa(remote.sin_addr));
    logger -> printDebugLog(buffer);
}

void ServerMain::onConnect(const int &fd, const struct sockaddr_in &remote)
{
    pair<sockaddr_in, bool> stat = make_pair(remote, NEW);
    pair<int, pair<sockaddr_in, bool> > conn = make_pair(fd, stat);
    connTracker.insert(conn);
    char buffer[512];
    sprintf(buffer, "%s - %d: New connection from IP: %s", __FILE__, __LINE__, inet_ntoa(remote.sin_addr));
    logger -> printDebugLog(buffer);
}

void ServerMain::onRecieve(const int &fd, unsigned char *data, const int &size)
{
    char buffer[512];
    //CONN_HANDLE_TYPE::iterator it = connTracker.find(fd);
    pair<sockaddr_in, int> &sec = connTracker[fd];
    if(sec.second == NEW) //if this is a new connection
    {
        if(atoi(reinterpret_cast<char*>(data)) == HELLO_MSG) //if just searching for a server
        //if(strncmp((char*)data, (char *)HELLO_MSG, size)) //if just searching for a server
        {
            sprintf(buffer, "%s - %d: %s said \"HELLO\"", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
            logger -> printDebugLog(buffer);
            sprintf(buffer, "%d", HELLO_MSG);
            send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
            sprintf(buffer, "%s - %d: Responded to IP %s as \"HELLO\"", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
            logger -> printDebugLog(buffer);
            closeConnection(fd);
        }
        else if(atoi((char*)data) == INIT_MSG) //if tries to initialize a connection
        //else if(strncmp((char*)data, (char*)INIT_MSG, size)) //if tries to initialize a connection
        {
            sec.second = INIT;
            sprintf(buffer, "%s - %d: Initialization request from IP %s", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
            logger -> printInfoLog(buffer);
            if(inControl == false)
            {
                sprintf(buffer, "%s - %d: Handshake request sent to IP %s", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
                logger -> printInfoLog(buffer);
                sprintf(buffer, "%d", HANDSHK_REQ_MSG);
                send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
            }
            else
            {
                sprintf(buffer, "%s - %d: Server is already being controlled. Illegal request from IP %s", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
                logger -> printWarnLog(buffer);
                sprintf(buffer, "%d", DENY_MSG);
                send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer)); //can't initialize, already in control
                closeConnection(fd);
            }
        }
    }
    else if(sec.second == INIT)
    {
        ConfigReader *cfReader = ConfigReader::getInstance();
        if(strncmp(reinterpret_cast<char*>(data), cfReader -> getPasswd().c_str(), cfReader -> getPasswd().size()) == 0)
        {
            sec.second = APPRVED;
            sprintf(buffer, "%d", APPROVE_MSG);
            send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
            sprintf(buffer, "%s - %d: Client with IP %s is approved.", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
            logger -> printInfoLog(buffer);
            inControl = true;
        }
        else
        {
            sprintf(buffer, "%s - %d: Client with IP %s is denied.", __FILE__, __LINE__, inet_ntoa(sec.first.sin_addr));
            logger -> printWarnLog(buffer);
            sprintf(buffer, "%d", DENY_MSG);
            send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer)); 
            closeConnection(fd);
        }
    }
    else if (sec.second = APPRVED)
    {
        int input = atoi(reinterpret_cast<char*>(data));
        bool rVal;
        switch(input)
        {
            case MUTE_REQ:
                rVal = sc.muteUnmute();
                if(rVal == false)
                {
                    sprintf(buffer, "%s - %d: Can't execute request.", __FILE__, __LINE__);
                    logger -> printErrorLog(buffer);
                    sprintf(buffer, "%d", ERROR_MSG);
                    send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
                }
                else
                {
                    sprintf(buffer, "%d", OK_MSG);
                    send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
                }
                break;
            case INCR_REQ:
                rVal = sc.increaseVolume();
                if(rVal == false)
                {
                    sprintf(buffer, "%s - %d: Can't execute request.", __FILE__, __LINE__);
                    logger -> printErrorLog(buffer);
                    sprintf(buffer, "%d", ERROR_MSG);
                    send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
                }
                else
                {
                    sprintf(buffer, "%d", OK_MSG);
                    send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
                }
                break;
            case DECR_REQ:
                rVal = sc.decreaseVolume();
                if(rVal == false)
                {
                    sprintf(buffer, "%s - %d: Can't execute request.", __FILE__, __LINE__);
                    logger -> printErrorLog(buffer);
                    sprintf(buffer, "%d", ERROR_MSG);
                    send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
                }
                else
                {
                    sprintf(buffer, "%d", OK_MSG);
                    send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
                }
                break;
            case GET_VOL_REQ:
            {
                float vol = sc.getCurrentVolumeLevel();
                sprintf(buffer, "%d", OK_MSG);
                send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
                sprintf(buffer, "%f", vol);
                send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
            }
                break;
            /*case SET_VOL_REQ: // no need
                sec.second = WAIT_DATA; //waiting for data
                send(fd, (unsigned char*)OK_MSG, 2);
                break;*/
            case PLAY_REQ:
                /*if(rVal == false)
                {
                    sprintf(buffer, "%s - %d: Can't execute request.", __FILE__, __LINE__);
                    logger -> printErrorLog(buffer);
                }*/
                break;
            case STOP_REQ:
                /*if(rVal == false)
                {
                    sprintf(buffer, "%s - %d: Can't execute request.", __FILE__, __LINE__);
                    logger -> printErrorLog(buffer);
                }*/
                break;
            case FWD_REQ:
                /*if(rVal == false)
                {
                    sprintf(buffer, "%s - %d: Can't execute request.", __FILE__, __LINE__);
                    logger -> printErrorLog(buffer);
                }*/
                break;
            case BCKWD_REQ:
                /*if(rVal == false)
                {
                    sprintf(buffer, "%s - %d: Can't execute request.", __FILE__, __LINE__);
                    logger -> printErrorLog(buffer);
                }*/
                break;
            default:
                sprintf(buffer, "%s - %d: Undefined request received.", __FILE__, __LINE__);
                logger -> printWarnLog(buffer);
                sprintf(buffer, "%d", DENY_MSG);
                send(fd, reinterpret_cast<unsigned char*>(buffer), strlen(buffer));
                break;
        }
    }
    else
    {
        //Currently not waiting for any data but reserved for this purpose
    }
}
