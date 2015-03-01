/* Ozgur Pekcagliyan 2014-11-16
 * Network Handler class definition to build a messenger application server
 * Edit: 2015-03-01, it is a generic class for any type of server
 */

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
#include <cstdio> //for sprintf
#include <csignal> //for signal handling
#include <vector>
//#include <atomic>
#include <thread>
#include <mutex>
#include <ctime>
#include <cstring>
#include <string>
#include <memory>

using std::unique_ptr;
using std::vector;
using std::thread;
using std::mutex;
using std::string;

#include "NetworkHandler.h"
#include "Logger.h"

mutex mtxfd;
mutex mtxthr;

//FIXME: Need to add SSL support
//FIXME: Use unique and shared pointers
//FIXME: RECIEVE_SEND_TIME_OUT would be better if it is dynamic
//FIXME: reinterpret_cast is too expensive think again...
#define RECIEVE_SEND_TIME_OUT 10 //data should be recieved or sent in 10 seconds

bool threadStarted;

void listener(NetworkHandler *);
void connHandler(NetworkHandler*, const int&);

NetworkHandler::NetworkHandler()
{
    logger = Logger::getInstance(); // initialize logger
    char buffer[1024];
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET = IPv4, SOCK_STREAM = TCP, 0 = protocol which is default
    if(sockfd == -1)
    {
        sprintf(buffer, "%s - %d: Can't open socket. Raising SIGTERM immediately.", __FILE__, __LINE__);
        logger -> printErrorLog(buffer);
        raise(SIGTERM);
        isUp = false;
    }
    else
    {
        TIME_OUT = 30; //this is in seconds
        tv.tv_sec = RECIEVE_SEND_TIME_OUT; //This is in seconds
        tv.tv_usec = 0;
        FD_ZERO(&fdset); //initialize fdset
        isUp = true;
        ipv6 = false;
        raw = true;
        sprintf(buffer, "%s - %d: A new server trying to wake up with sokcet %d..", __FILE__, __LINE__, sockfd);
        logger -> printDebugLog(buffer);
    }
}

NetworkHandler::~NetworkHandler()
{
    shutdown();
}

/*NetworkHandler::NetworkHandler(const int &type) // Constructor to select connection type
{
    int sockType;
    if(type == TCP)
    {
        sockType = SOCK_STREAM;
    }
    else if(type == UDP)
    {
        sockType = SOCK_DGRAM;
    }
    else
    {
        char buffer[1024];
        sprintf(buffer, "%s - %d: Unsupported protocol reques. Raising SIGTERM immediately.", __FILE__, __LINE__);
        logger -> printErrorLog(buffer);
        raise(SIGTERM);
        isUp = false;
    }
    sockfd = socket(AF_INET, sockType, 0); //AF_INET = IPv4, SOCK_STREAM = TCP, 0 = protocol which is default
    if(sockfd == -1)
    {
        char buffer[1024];
        sprintf(buffer, "%s - %d: Can't open socket. Raising SIGTERM immediately.", __FILE__, __LINE__);
        logger -> printErrorLog(buffer);
        raise(SIGTERM);
        isUp = false;
    }
    else
    {
        FD_ZERO(&fdset); //initialize fdset
        isUp = true;
        ipv6 = false;
        TIME_OUT = 600;
    }
}*/

/*NetworkHandler::NetworkHandler(const int &type, const int &domain) // Constructor to select connection type and ipv6
{
    int sockType;
    int IPVer;
    if(type == TCP)
    {
        sockType = SOCK_STREAM;
    }
    else if(type == UDP)
    {
        sockType = SOCK_DGRAM;
    }
    else
    {
        char buffer[1024];
        sprintf(buffer, "%s - %d: Unsupported protocol reques. Raising SIGTERM immediately.", __FILE__, __LINE__);
        logger -> printErrorLog(buffer);
        raise(SIGTERM);
        isUp = false;
    }

    if(domain == IPV4)
    {
        IPVer = AF_INET;
        ipv6 = false;
    }
    else if(domain == IPV6)
    {
        IPVer = AF_INET6;
        ipv6 = true;
    }
    else
    {
        char buffer[1024];
        sprintf(buffer, "%s - %d: Unsuppoerted domain request. Raising SIGTERM immediately.", __FILE__, __LINE__);
        logger -> printErrorLog(buffer);
        raise(SIGTERM);
        isUp = false;
    }

    sockfd = socket(AF_INET, sockType, 0); //AF_INET = IPv4, SOCK_STREAM = TCP, 0 = protocol which is default
    if(sockfd == -1)
    {
        char buffer[1024];
        sprintf(buffer, "%s - %d: Can't open socket. Raising SIGTERM immediately.", __FILE__, __LINE__);
        logger -> printErrorLog(buffer);
        raise(SIGTERM);
        isUp = false;
    }
    else
    {
        FD_ZERO(&fdset); //initialize fdset
        isUp = true;
        TIME_OUT = 600;
    }
}*/

bool NetworkHandler::listen(const int &port, const int &clientCount)//This will open port and start listening
{
    threadStarted = false;
    int yes = 1;
    char buffer[1024];
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, static_cast<void*>(&yes), sizeof(int)) == -1) //This line configures socket to reuse same port for different sockets.
    {
        sprintf(buffer, "%s - %d: Can't configure socket. Informed caller.", __FILE__, __LINE__);
        logger -> printWarnLog(buffer);
        isUp = false;
        return false;
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, static_cast<void*>(&tv), sizeof(struct timeval)) == -1) //This line configures socket to reuse same port for different sockets.
    {
        sprintf(buffer, "%s - %d: Can't configure socket. Informed caller.", __FILE__, __LINE__);
        logger -> printWarnLog(buffer);
        isUp = false;
        return false;
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, static_cast<void*>(&tv), sizeof(struct timeval)) == -1) //This line configures socket to reuse same port for different sockets.
    {
        sprintf(buffer, "%s - %d: Can't configure socket. Informed caller.", __FILE__, __LINE__);
        logger -> printWarnLog(buffer);
        isUp = false;
        return false;
    }

    if(ipv6 == true)
    {
        sprintf(buffer, "%s - %d: IPV6 is not implemented currently. Sorry :(", __FILE__, __LINE__);
        logger -> printWarnLog(buffer);
        isUp = false;
        return false;
//        serverSock.sin_family = AF_INET6;           //host byte order
//        serverSock.sin_port = htons(port);          //short, network byte order
//        serverSock.sin_addr.s_addr = in6addr_any;	//use my IP address
//        memset(&(serverSock.sin_zero), '\0', 8);    //set zero to the rest of the struct
//        if(bind(sockfd, static_cast<struct sockadd *>(&serverSock6), sizeof(serverSock6)) == -1)
//        {
//            char buffer[1024];
//            sprintf(buffer, "%s - %d: Can't bind socket. Informed caller.", __FILE__, __LINE__);
//            logger -> printWarnLog(buffer);
//            isUp = false;
//            return false;
//        }
    }
    else
    {
        serverSock.sin_family = AF_INET;                //host byte order
        serverSock.sin_port = htons(port);          	//short, network byte order
        serverSock.sin_addr.s_addr = htonl(INADDR_ANY);	//use my IP address
        memset(&(serverSock.sin_zero), '\0', 8);		//set zero to the rest of the struct

        if(bind(sockfd, reinterpret_cast<struct sockaddr *>(&serverSock), sizeof(serverSock)) == -1)
        {
            sprintf(buffer, "%s - %d: Can't bind socket. Informed caller.", __FILE__, __LINE__);
            logger -> printWarnLog(buffer);
            isUp = false;
            return false;
        }
    }

    if(::listen(sockfd, clientCount) == -1) //starting to listen port
    {
        sprintf(buffer, "%s - %d: Can't listen. Informed caller.", __FILE__, __LINE__);
        logger -> printWarnLog(buffer);
        isUp = false;
        return false;
    }

    FD_SET(sockfd, &fdset); //add sockfd to fdset as first input
    fdmax = sockfd;
    threadHndlr.push_back(thread(listener, this));
    sprintf(buffer, "%s - %d: Server started to listen port %d on %s.", __FILE__, __LINE__, ntohs(serverSock.sin_port), inet_ntoa(serverSock.sin_addr));
    logger -> printInfoLog(buffer);
//    while(!threadStarted);
//    mtxthr.lock();
//    threadStarted = false;//prepare for next thread
//    mtxthr.unlock();
    return true; //caller of this function should execute an infinite loop!!!!!
}

bool NetworkHandler::isRunning()
{
    mtxfd.lock();
    bool rVal = isUp;
    mtxfd.unlock();
    return rVal;
}

bool NetworkHandler::send(const int &fd, const unsigned char *data, const int &s) //data and its size
{
    int send_size = 0;
    int total = 0;
    char buffer[1024];
    if(raw == false)
    {
        int size = s;
        send_size = ::send(fd, reinterpret_cast<void*>(&size), sizeof(size), 0);
        if(send_size <= 0)
        {
            sprintf(buffer, "%s - %d: Couldn't send size of the data which was %d", __FILE__, __LINE__, size);
            logger -> printErrorLog(buffer);
            return false;
        }
        int sStartTime, sEndTime;
        while(total < size)
        {
            sStartTime = time(0);
            send_size = ::send(fd, data + total, size - total, 0);
            sEndTime = time(0);
            if(sEndTime - sStartTime >= RECIEVE_SEND_TIME_OUT)
            {
                sprintf(buffer, "%s - %d: Time out for send operation: %d", __FILE__, __LINE__, fd);
                logger -> printInfoLog(buffer);
                break;
            }
            total += send_size;
            if(total <= 0)
            {
                sprintf(buffer, "%s - %d: Couldn't send size the data.", __FILE__, __LINE__);
                logger -> printErrorLog(buffer);
                sprintf(buffer, "%s - %d: The data was: '%s'.", __FILE__, __LINE__, data);
                logger -> printDebugLog(buffer);
                return false;
            }
        }
    }
    else
    {
        while(total < s)
        {
            send_size = ::send(fd, data + total, s - total, 0);
            total += send_size;
            if(total <= 0)
            {
                sprintf(buffer, "%s - %d: Couldn't send size the data.", __FILE__, __LINE__);
                logger -> printErrorLog(buffer);
                sprintf(buffer, "%s - %d: The data was: '%s'.", __FILE__, __LINE__, data);
                logger -> printDebugLog(buffer);
                return false;
            }
        }
    }
    return true;
}

void NetworkHandler::setConnType(const int &c)
{
    //FIXME: Need to close all connections then set
}

void NetworkHandler::setProtType(const int &p)
{
    //FIXME: Need to close all connections then set
}

void NetworkHandler::setRaw(const bool &r)
{
    raw = r;
}

void NetworkHandler::setTimeOut(const long &t)
{
    TIME_OUT = t;
}

int NetworkHandler::getTimeOut()
{
    return TIME_OUT;
}

bool NetworkHandler::closeConnection(const int &fd)
{
    if(fd != sockfd && isInSet(fd))
    {
        char buffer[1024];
        mtxfd.lock();
        FD_CLR(fd, &fdset); // remove from master set
        mtxfd.unlock();
#ifdef WIN32
        closesocket(fd);
#else
        close(fd);
#endif
        sprintf(buffer, "%s - %d: Connection closed for IP: %s.", __FILE__, __LINE__,  inet_ntoa(sockIPMap[fd].sin_addr));
        logger -> printInfoLog(buffer);
        onClose(fd, sockIPMap[fd]);
        sockIPMap.erase(fd);
        sprintf(buffer, "%s - %d: Client's socket closed: %d.", __FILE__, __LINE__, fd);
        logger -> printDebugLog(buffer);
    }
}

bool NetworkHandler::shutdown()
{
    char buffer[1024];
    mtxfd.lock();
    isUp = false;
    mtxfd.unlock();
    for(int i = 0; i <= fdmax; ++i)
    {
        closeConnection(i);
    }
    int max = (RECIEVE_SEND_TIME_OUT*1000000 > TIME_OUT*1000000 ? RECIEVE_SEND_TIME_OUT*1000000 : TIME_OUT*1000000); //turn everything into microseconds
    sprintf(buffer, "%s - %d: Shutting down the server. Waiting %f seconds for remaining threads.", __FILE__, __LINE__,  (double)max/1000000);
    logger -> printInfoLog(buffer);
    usleep(max + 500000); //give all threads a chance to close themselves
    mtxfd.lock();
    FD_CLR(sockfd, &fdset); // remove from master set
    mtxfd.unlock();
#ifdef WIN32
    closesocket(sockfd);
#else
    close(sockfd);
#endif
    logger -> printInfoLog("Server shutdown complete");
    return true;
}

void NetworkHandler::setSSL(const bool &s)
{
    isSSL = s;
}

bool NetworkHandler::isInSet(const int &fd)
{
    bool rVal;
    mtxfd.lock();
    rVal = FD_ISSET(fd, &fdset);
    mtxfd.unlock();
    return rVal;
}

void listener(NetworkHandler *obj)
{
    int i;
    int addrlen;
    int newfd;
    struct sockaddr_in remote;
    char buffer[1024];
    Logger * logger = Logger::getInstance();
//    mtxthr.lock();
//    threadStarted = true;
//    mtxthr.unlock();
    while(obj->isRunning())
    {
        for(i = 0; i <= obj->fdmax; ++i)
        {
            /*if(select(obj->fdmax + 1, &obj->fdset, nullptr, nullptr, &obj->tv) == -1)
            {
                sprintf(buffer, "%s - %d: Can't instentiate select.", __FILE__, __LINE__);
                logger -> printErrorLog(buffer);
            }
            else*/ if(obj->isInSet(i) && i == obj->sockfd)
            {
                addrlen = sizeof(remote);
#ifdef WIN32
                if((newfd = accept(obj->sockfd, reinterpret_cast<struct sockaddr*>(&remote), &addrlen))) == -1)
#else
                if((newfd = accept(obj->sockfd, reinterpret_cast<struct sockaddr*>(&remote), reinterpret_cast<socklen_t *>(&addrlen))) == -1)
#endif
                {
                    if(errno != ECONNABORTED && errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        sprintf(buffer, "%s - %d: Couldn't accept connection. Informed caller.", __FILE__, __LINE__);
                        logger -> printWarnLog(buffer);
                        sprintf(buffer, "%s - %d: Errorno: %d.", __FILE__, __LINE__, errno);
                        logger -> printDebugLog(buffer);
                    }
                }
                else
                {
                    mtxfd.lock();
                    FD_SET(newfd, &(obj->fdset));
                    mtxfd.unlock();
                    if(newfd > obj->fdmax)
                    {
                        obj->fdmax = newfd;
                    }
                    sprintf(buffer, "%s - %d: A new client connected to the server from %s.", __FILE__, __LINE__, inet_ntoa(remote.sin_addr));
                    logger -> printInfoLog(buffer);
                    sprintf(buffer, "%s - %d: Clients socket address: %d", __FILE__, __LINE__, newfd);
                    logger -> printDebugLog(buffer);
                    obj->sockIPMap[newfd] = remote;
                    obj->onConnect(newfd, remote);
                    obj->threadHndlr.push_back(thread(connHandler, obj, newfd));
//                    while(!threadStarted);
//                    mtxthr.lock();
//                    threadStarted = false; //prepare for next thread
//                    mtxthr.unlock();
                }
            }
        }
    }
}


void connHandler(NetworkHandler* obj, const int &fd)
{
    Logger * logger = Logger::getInstance();
    char buffer[1024];
    //unsigned char *recvBuffer; //this will recieve data and will be dynamically allocated
//    mtxthr.lock();
//    threadStarted = true; //inform caller that thread has started
//    mtxthr.unlock();
    bool keepConAlive = true;
//    mtxthr.lock();
//    obj->timer.push_back(time(0));
//    int myTimePos = obj->timer.size() - 1; //I am the last element so, my position is one less from size
//    mtxthr.unlock();
    time_t startTime = time(0);
    time_t currentTime = startTime;
    int dataSize = 0;
    int recv_size = 0;
    int total = 0;
    while(keepConAlive && obj->isRunning() && obj->isInSet(fd))
    {
        currentTime = time(0);
        if(currentTime - startTime >= obj->TIME_OUT)
        {
            sprintf(buffer, "%s - %d: Client didn't communicate for a very long time: %s. Connection closed.", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
            logger -> printInfoLog(buffer);
//            obj->closeConnection(fd);
//            keepConAlive = false; //close connection
//            break;
        }
        else if(obj->raw == false)
        {
            /*
             * Here I declare communication protocol as simple as possible. First, recieve size of the data, then, start recieving data. This is the mid-level protocol to recieve any data
             */
            int size;
            recv_size = recv(fd, reinterpret_cast<void*>(&size), sizeof(size), 0);
            if(recv_size < 0)
            {
                sprintf(buffer, "%s - %d: Couldn't recieve size of the data, closing connection: %s", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
                logger -> printInfoLog(buffer);
                obj->closeConnection(fd);
                keepConAlive = false; //close connection
//                break;
            }
            else if (recv_size == 0)
            {
                sprintf(buffer, "%s - %d: Connection closed by client: %s", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
                logger -> printInfoLog(buffer);
                sprintf(buffer, "Socket id: %d", fd);
                logger -> printDebugLog(buffer);
                obj->closeConnection(fd);
                keepConAlive = false; //close connection
//                break;
            }
            else
            {
                total = 0;
                unique_ptr<unsigned char[]> recvBuffer (new unsigned char[size]); //guaranteed to be cleared :)
                //recvBuffer = new unsigned char[size]; // open size bytes in memory
                int rStartTime, rEndTime;
                while(total < size)
                {
                    rStartTime = time(0);
                    recv_size = recv(fd, recvBuffer.get() + total, size - total, 0);
                    rEndTime = time(0);
                    if(rEndTime - rStartTime >= RECIEVE_SEND_TIME_OUT)
                    {
                        sprintf(buffer, "%s - %d: Time out for recieve operation: %s", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
                        logger -> printInfoLog(buffer);
                        break;
                    }
                    total += recv_size;
                }

                if(total <= 0)
                {
                    if(total == 0)
                    {
                        sprintf(buffer, "%s - %d: Connection closed by client: %s", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
                        logger -> printInfoLog(buffer);
                        sprintf(buffer, "Socket id: %d", fd);
                        logger -> printDebugLog(buffer);
                    }
                    else
                    {
                        sprintf(buffer, "%s - %d: Client is down: %s", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
                        logger -> printInfoLog(buffer);
                    }
                    keepConAlive = false; //close connection
                    obj->closeConnection(fd);
                    keepConAlive = false; //close connection
        //            break;
                }
                else
                {
                    obj->onRecieve(fd, recvBuffer.get(), size);
                }
                //delete []recvBuffer; //cleanup memory
            }
        }
        else //raw recieve heyyy!!
//FIXME: Raw recieve still beta and dangerous to use it
        {
            const int RECV_BUF_SIZE = 1024;
            total = 0;
            int total2 = 0;
            unique_ptr<unsigned char[]> recvBuffer (new unsigned char[RECV_BUF_SIZE]);
            //recvBuffer = new unsigned char[RECV_BUF_SIZE]; // open size bytes in memory
            string data;
            while(total >= 0)
            {
                memset(recvBuffer.get(), '\0', RECV_BUF_SIZE);
                recv_size = recv(fd, recvBuffer.get() + total2, RECV_BUF_SIZE - total2, 0);
                total += recv_size;
                total2 += recv_size;
                if(total2 == RECV_BUF_SIZE)
                {
                    total2 = 0;
                    data.append(reinterpret_cast<char*>(recvBuffer.get()));
                }
                else if(recv_size <= 0)
                {
                    break;
                }
                else if((recvBuffer.get())[total2] == '\0')
                {
                    data.append(reinterpret_cast<char*>(recvBuffer.get()));
                    break;
                }
            }

            if(total <= 0)
            {
                if(total == 0)
                {
                    sprintf(buffer, "%s - %d: Connection closed by client: %s", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
                    logger -> printInfoLog(buffer);
                    sprintf(buffer, "Socket id: %d", fd);
                    logger -> printDebugLog(buffer);
                }
                else
                {
                    sprintf(buffer, "%s - %d: Client is down: %s", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
                    logger -> printInfoLog(buffer);
                }
                obj->closeConnection(fd);
                keepConAlive = false; //close connection
    //            break;
            }
            else
            {
                obj->onRecieve(fd, (unsigned char*)data.c_str(), data.size());
            }
            //delete []recvBuffer; //cleanup memory
        }
    }
    if(obj->isInSet(fd))
    {
        sprintf(buffer, "%s - %d: Connection closed for client: %s", __FILE__, __LINE__, inet_ntoa(obj->sockIPMap[fd].sin_addr));
        logger -> printInfoLog(buffer);
        sprintf(buffer, "Socket id: %d", fd);
        logger -> printDebugLog(buffer);
        obj->closeConnection(fd);
    }
}

