#include <string>
#include "XmlParser.h"
#include <iostream>
#include "Logger.h"
#include <signal.h>


using std::cout;
using std::endl;
using std::string;

using namespace XmlSpace;





void sigHandler(int sig)
{
	char buffer[128];
	sprintf(buffer, "%s - %d: SIGTERM caught", __FILE__, __LINE__);
	Logger::printInfoLog(buffer);
	Logger::finalize();
	signal(sig, SIG_DFL);
	raise(sig);
}






int main()
{
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);
// 	Logger::setLogConfig("./","test.log", Logger::DEBUG);
	Logger::setLogConfig("./","test.log", Logger::DEBUG, false);
// 	string data = "<test a=1/><html><head test1=1111 test2=\"2222\" test3=\"3333 aaaa\" test4=4444><title>deneme</title></head><body>asda\nsd<br/></body></html>";
	string data = "<test a=1/><!--This is a test--><html><head test1=1111 test2=\"2222\" test3=\"3333 aaaa\" test4=4444><title>deneme</title></head><body>asda\nsd<br/><a href=\"http://www.example.com\">test link</a></body></html>";
	XmlParser myP;
	myP.loadXmlTree(data);
	myP.printTree();
	cout<<endl<<endl;
	myP.findNode("head");
	string res;
	myP.getAttributeValue(res, "test1");
	cout<<"head "<<res<<endl;
	myP.getAttributeValue(res, 3);
	cout<<"head "<<res<<endl;
	myP.findNode("a");
	myP.getAttributeValue(res, "href");
	cout<<"a "<<res<<endl;
	cout<<"a[] "<<myP["href"]<<endl;
	cout<<"a[N] "<<myP[1]<<endl;
// 	myP.findNode("title");
// 	myP.getNodeValue(res);
// 	cout<<"title "<<res<<endl; //will print last found items value since it can't start from beginning
// 	
	return 0;
}