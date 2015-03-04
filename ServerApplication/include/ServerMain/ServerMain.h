/* 
 * File:   ServerMain.h
 * Author: zgrw
 * This class will do all the job
 * Created on March 1, 2015, 5:52 PM
 * Last edited on: 2015-03-02
 */

#ifndef SERVERMAIN_H
#define	SERVERMAIN_H

#include <map>
#include <utility>

#include "NetworkHandler.h"
#include "Logger.h"
#include "SoundController.h"

typedef std::map< int, std::pair< sockaddr_in, int > > CONN_HANDLE_TYPE; //it is like this: <fd, <address, state>>
/*State
 * 0: initial (default value)
 * 1: Init Request recieved
 * 2: Request approved
*/
class ServerMain : public NetworkHandler
{
public:
    enum fd_state {NEW = 0, INIT = 1, APPRVED = 2, WAIT_DATA = 3};
    enum server_messages {ERROR_MSG = 0, HELLO_MSG = 1, INIT_MSG = 2, DENY_MSG = 3, APPROVE_MSG = 4, HANDSHK_REQ_MSG = 5, OK_MSG = 6};
    enum request_messages {MUTE_REQ = 10, INCR_REQ = 11, DECR_REQ = 12, GET_VOL_REQ = 13, SET_VOL_REQ = 14, PLAY_REQ = 15, STOP_REQ = 16, FWD_REQ = 17, BCKWD_REQ = 18};
    
    ServerMain();
    ~ServerMain();
    virtual void onRecieve(const int &fd, unsigned char *data, const int &size);
    virtual void onConnect(const int &fd, const struct sockaddr_in &remote);
    virtual void onClose(const int &fd, const struct sockaddr_in &remote);
    //bool sendResponse(const int &fd, const void *data, const int &size);
private:
    Logger *logger;
    SoundController sc;
    bool inControl; //will determine if any client is currently controlling app or not
    CONN_HANDLE_TYPE connTracker; //This will track status of connections (like new connection, established connection), and will used to confirm handshakes
};

#endif	/* SERVERMAIN_H */

