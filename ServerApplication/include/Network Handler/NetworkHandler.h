/* Ozgur Pekcagliyan 2014-11-16
 * Network Handler class definition to build a messenger application server
 * Edit: 2015-02-24, it is a generic class for any type of server
 */

#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <vector>
#include <map>
#include <thread>
#include <ctime>
#include <netinet/in.h>
#include <sys/select.h>


class NetworkHandler
{
public:
    enum CONN_TYPE {TCP = 1, UDP = 2};
    enum PROT_TYPE {IPV4 = 1, IPV6 = 2};
    NetworkHandler(); // Default constructor, TCP for default
    virtual ~NetworkHandler();
   // NetworkHandler(const int &); // Constructor to select connection type
   // NetworkHandler(const int &,const int&); // Constructor to select connection type and ipv6
    bool listen(const int &port, const int &clientCount = 100); //This will open port and start listening
    bool send(const int&, const unsigned char *, const int&); //data and its size
    virtual void onRecieve(const int &fd, unsigned char *data, const int &size) = 0; //makes class abstract and can't be instentiated
    virtual void onConnect(const int &fd, const struct sockaddr_in &remote) = 0;
    virtual void onClose(const int &fd, const struct sockaddr_in &remote) = 0;
    friend void listener(NetworkHandler *);
    friend void connHandler(NetworkHandler*, const int&);
    bool isRunning(); //return if server up or not
    void setTimeOut(const long&);
    void setConnType(const int &c);
    void setProtType(const int &p);
    void setRaw(const bool &r);
    int getTimeOut();
    bool closeConnection(const int &fd);
    bool shutdown();
protected:
    void setSSL(const bool &);
private:
    bool isInSet(const int &fd);
    struct sockaddr_in serverSock;
    struct sockaddr_in6 serverSock6;
    bool ipv6;
    bool isSSL;
//    std::vector<time_t> timer; //this will track connection time per connection
    bool isUp;
    int fdmax;
    std::vector<std::thread> threadHndlr;
    int sockfd; //socket handler
    fd_set fdset;
    long TIME_OUT;
    bool raw;
    struct timeval tv;
    std::map<int, struct sockaddr_in> sockIPMap;
};




#endif // NETWORKHANDLER_H
