/* 
 * File:   ServerMain.h
 * Author: zgrw
 * This class will do all the job
 * Created on March 1, 2015, 5:52 PM
 */

#ifndef SERVERMAIN_H
#define	SERVERMAIN_H

#include <map>
#include <vector>

#include "NetworkHandler.h"
#include "Logger.h"
#include "SoundController.h"

class ServerMain : public NetworkHandler
{
public:
    enum server_messages {ERROR_MSG = 0, HELLO_MSG = 1, INIT_MSG = 2, DENY_MSG = 3, APPROVE_MSG = 4};
    
    ServerMain();
    ~ServerMain();
    virtual void onRecieve(const int &fd, unsigned char *data, const int &size);
    virtual void onConnect(const int &fd, const struct sockaddr_in &remote);
    virtual void onClose(const int &fd, const struct sockaddr_in &remote);
    //bool sendResponse(const int &fd, const void *data, const int &size);
private:
    Logger *logger;
    SoundController sc;
    std::vector<std::map<int, int>> connTracker; //This will track status of connections (like new connection, established connection), and will used to confirm handshakes
};

#endif	/* SERVERMAIN_H */

