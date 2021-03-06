/* 
 * File:   newsimpletest.cpp
 * Author: zgrw
 *
 * Created on Feb 24, 2015, 8:21:33 PM
 * Last edited by: Ozgur Pekcagliyan - 2015-03-01
 */

#include <arpa/inet.h> //for inet_ntoa function
#include<signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <NetworkHandler.h>
#include <Logger.h>

using namespace std;

/*
 * Simple Echo Server
 */
class NetworkTest : public NetworkHandler
{
public:
    NetworkTest();
    virtual void onRecieve(const int &fd, unsigned char *data, const int &size); //makes class abstract and can't be instentiated
    virtual void onConnect(const int &fd, const struct sockaddr_in &remote);
    virtual void onClose(const int &fd, const struct sockaddr_in &remote);
};

NetworkTest::NetworkTest()
{
    
}                                                                                                                                                                                                                                                   

void NetworkTest::onRecieve(const int &fd, unsigned char *data, const int &size)
{
    cout<<data<<endl;
    send(fd, data, size);
}

void NetworkTest::onConnect(const int &fd, const struct sockaddr_in &remote)
{
    cout<<"New connection from ip:"<<inet_ntoa(remote.sin_addr)<<endl;
}

void NetworkTest::onClose(const int &fd, const struct sockaddr_in &remote)
{
    cout<<"Connection gone from ip:"<<inet_ntoa(remote.sin_addr)<<endl;
}

NetworkTest nt;
Logger *logger;

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
    nt.shutdown();
    logger -> finalize();
    signal(sig, SIG_DFL);
    raise(sig);
}


int main(int argc, char** argv) {
    logger = Logger::getInstance();
    signal(SIGTERM, sigHandler);
    signal(SIGINT, sigHandler);
    logger -> setLogConfig("./","test.log", Logger::DEBUG, true);

    nt.listen(1234);
    while(1);
    logger -> finalize();
    return 0;
}

