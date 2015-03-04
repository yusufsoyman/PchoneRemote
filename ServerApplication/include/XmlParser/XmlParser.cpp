/*
 * XmlParser.cpp
 * Main Class to manage XML operations
 * Created by: Ozgur Pekcagliyan - 2014-10-05 12:28:32 PM EEST
 * Last edited by: Ozgur Pekcagliyan - 2014-10-25
 * Notes:
 *
*/

#include <string>
// #include <map>
#include <stack>
#include <iostream> // TODO:This will be romeved in working copy
#include "XmlParser.h"
#include "Logger.h"

using std::cout;
using std::endl;
using std::string;
using std::stack;
using std::vector;
using namespace XmlSpace;


void XmlParser::vectorizeString (const string &str) // This will tokenize a string and fill a vector
{
	Logger::printInfoLog("Started parsing XML data");
	deleteAll();//clean tree just in case
	int nodePtr = -1;
	string tempStr(str);
	string tempToken;
	size_t pos, posBeg, posSpace;
	stack<XmlNode *> nodeStack; //This will hold nodes and 
	bool comment = false;
	while((pos = tempStr.find(">")) != string::npos) //find a node
	{
		Logger::printDebugLog("Found next end of node indicator >");
		tempToken = tempStr.substr(0, pos);
		if(nodeStack.empty())
		{
			Logger::printDebugLog("Requested starting next tree");
			++nodePtr;
		}
		if(tempToken.substr(0, 4) == "<!--")
		{
			Logger::printDebugLog("Found comment in XML, will ignore next data");
			tempToken.erase(0, 4); //remove head part for not to find it twice
			Logger::printDebugLog("Looking for end part for comment section");
			if( (posBeg = tempToken.find("--")) != string::npos)
			{
				Logger::printDebugLog("Found comment end, data till here will be deleted");
				tempStr.erase(0, 4 + posBeg + 3);
				if(nodeStack.empty())
				{
					Logger::printDebugLog("This comment were itself a new tree, so, tree requested for this one has taken back");
					--nodePtr;
				}
			}
			else
			{
				Logger::printDebugLog("Removing entire block, since end of comment couldn't be found yet");
				comment = true;
				tempStr.erase(0, pos + 1);
			}
		}
		else if(tempToken.substr(pos - 2, pos) == "--" && comment)
		{
			Logger::printDebugLog("End of comment section found, removing data till here");
			tempStr.erase(0, pos + 1);
			comment = false;
			if(nodeStack.empty())
			{
				Logger::printDebugLog("This comment were itself a new tree, so, tree requested for this one has taken back");
				--nodePtr;
			}
		}
		else 
		{
			if(tempToken[pos - 1] == '/' && !comment) //<br/>
			{
				Logger::printDebugLog("Found a node looking like <br/>");
				string nodeName;
				string tempSlashToken = tempToken;
				size_t c = 0;
				while ((posBeg = tempSlashToken.find("<")) != string::npos) //what if it is inside of another node, we don't want to mess with parrent node, so, we will try to find exact position which should be the last node
				{
					c += posBeg;
					tempSlashToken.erase(0, posBeg + 1);
				}
				Logger::printDebugLog("Found exact position of node");
				tempSlashToken = tempToken.substr(c, string::npos);
				Logger::printDebugLog("Looking for potential attributes");
				if ((posSpace = tempSlashToken.find(" ")) != string::npos)
				{
					Logger::printDebugLog("There are spaces inside the node, this generally means attributes, calling attribute finder");
					nodeName = tempSlashToken.substr(0, posSpace);
					attributeFinder(tempSlashToken, nodeStack, nodePtr, posSpace, "/"); //this deletes first < char
				}
				else
				{
					nodeName = tempSlashToken.substr(1, tempSlashToken.size() - 2); // -2 because of />
					XmlNode *temp = createNode(nodeName, nodePtr);
					nodeStack.top() -> subNodes.push_back(temp);
					Logger::printDebugLog("Node as added as a child node to the top node in stack");
				}
				tempStr.erase(c, pos - c + 1); // just <asdasd/> part not the whole body
				Logger::printDebugLog("Ndoe data removed from temp memeory");
			}
			
			else if ((posSpace = tempToken.find(" ")) != string::npos && tempToken.find("</") == string::npos && !comment) //<a href="sf">
			{
				Logger::printDebugLog("Found a node which includes attributes");
				posBeg = tempToken.find("<");
				string nodeName = tempToken.substr(posBeg + 1, posSpace - posBeg - 1);
				size_t tempPos = posSpace - posBeg;
				Logger::printDebugLog("Calling attribute finder");
				attributeFinder(tempToken, nodeStack, nodePtr, posSpace);
				//Lets find its value and clear tags ;)
				Logger::printDebugLog("Clearing tages to find its entire node value");
				XmlNode *node = nodeStack.top();
				node -> nodeName = nodeName; //fixing nodeName
				string endNode = "</";
				endNode += nodeName + ">";
				if((posSpace = tempStr.find(endNode.c_str())) == string::npos) //like <body color=#ffffff>.....................</body>
				{
					string log = "Node doesn't have an end: ";
					log += nodeName;
					Logger::printErrorLog("log");
					//FIXME: houston we got a probllem
				}
				string nodeVal = tempStr.substr(pos + 1, posSpace - pos - 1);
				nodeStack.top() -> nodeVal = nodeVal; //This is added into stack by attribute finder
				Logger::printDebugLog("Node value updated inside of stack");
				tempStr.erase(0, pos + 1); //erase read part
				Logger::printDebugLog("Ndoe data removed from temp memeory");
			}
			else if(!comment)//<asd> qwe </asd>
			{
				if(tempToken.find("</") != string::npos) // asd</asd>
				{
					Logger::printDebugLog("Found end of a node and node removed from stack");
					//FIXME: Be sure stack is not empty and this XML is valid
					nodeStack.pop();
				}
				else // <asd>
				{
					Logger::printDebugLog("Found a new node");
					Logger::printDebugLog("Clearing tages to find its entire node value");
					posBeg = tempToken.find("<");
					string nodeName = tempToken.substr(posBeg + 1, string::npos); //0th element should be <
					string endNode = "</";
					endNode += nodeName + ">";
					if((posSpace = tempStr.find(endNode.c_str())) == string::npos)//like <body>.....................</body>
					{
						string log = "Node doesn't have an end: ";
						log += nodeName;
						Logger::printErrorLog("log");
						//FIXME: houston we got a probllem
					}
					string nodeVal = tempStr.substr(pos + 1, posSpace - pos - 1);
					XmlNode *temp = createNode(nodeName, nodePtr, nodeVal); //create an empty node
					Logger::printDebugLog("New node created by create node function");
					if(temp == NULL) //we have just modified and this mmeans stack should also be empty
					{
						nodeStack.push(head[nodePtr]); //insert firs element to stack
						Logger::printDebugLog("Stack is empty, so found is the head of a new tree and tree head added to stack");
					}
					else
					{
						nodeStack.top() -> subNodes.push_back(temp);
						Logger::printDebugLog("Node added as a subnode of the top node in stack");
						nodeStack.push(temp);
						Logger::printDebugLog("Node added as the top node into stack");
					}
				}
				tempStr.erase(0, pos + 1); //erase read part
	// 			tempStr.erase(posBeg, endNode.size()); //erase read part
			}
		}
	}
	if(nodeStack.empty() == false)
	{
		Logger::printErrorLog("Invalid XML data!");
		//Oh no we have an invalid XML and couldn't complete XML
	}
}

void XmlParser::attributeFinder(string tempToken, stack<XmlNode *> &nodeStack, int &nodePtr, size_t &posSpace, string delim)
{
	Logger::printDebugLog("Strated looking for attributes");
	string nodeName = tempToken.substr(1, posSpace - 1); //0th element should be <
	size_t posTemp;
	XmlNode *temp = createNode(nodeName, nodePtr);
	if(temp == NULL)
	{
		//TODO: be sure stack is empty!
		Logger::printDebugLog("Added head of tree into nodeStack");
		nodeStack.push(head[nodePtr]); //insert firs element to stack
		temp = nodeStack.top();
	}
	else
	{
		nodeStack.top() -> subNodes.push_back(temp);
		Logger::printDebugLog("Node added as a subnode of the top node in stack");
		nodeStack.push(temp);
		Logger::printDebugLog("Node added to stack");
	}
	if (delim == "/")
	{
		Logger::printDebugLog("This is just a single node, so it is removed from stack ex: <br/> or <br test=1/>");
		nodeStack.pop();//node cloased
	}
	tempToken.erase(0, posSpace + 1);
	string attrName, attrValue;
	bool hasQuote;
	Logger::printDebugLog("Strated scanning for attributes");
	while((posTemp = tempToken.find("=")) != string::npos)
	{
		hasQuote = false;
		attrName = tempToken.substr(0, posTemp); //range between space and = sign like //<a href=
		Logger::printDebugLog("Found attribute name");
		if(tempToken[posTemp + 1] == '\"')
		{
			Logger::printDebugLog("Attribute has quotes so moved posSpace pointer accordingly");
			hasQuote = true;
			++posTemp; //Move to other side of quote
			posSpace = tempToken.substr(posTemp + 1, string::npos).find("\"") + posTemp + 1; // FIXME:So expensive
		}
		else
		{
			Logger::printDebugLog("Attribute doesn't have any quotes so moved posSpace pointer accordingly");
			if((posSpace = tempToken.find(" ")) == string::npos)
			{
				if(delim.empty() == false)
				{
					posSpace = tempToken.find(delim);
				}
				else
				{
					posSpace = tempToken.size();
				}
			}
		}
		attrValue = tempToken.substr(posTemp + 1, posSpace - posTemp - 1);
		Logger::printDebugLog("Found attribute value");
		if(hasQuote)
		{
			tempToken.erase(0, posSpace + 2);
		}
		else
		{
			tempToken.erase(0, posSpace + 1);
		}
		Logger::printDebugLog("Removed attribute from temp memory");
		XmlAttribute *tempAttr = new XmlAttribute();
		tempAttr -> attrName = attrName;
		tempAttr -> attrVal = attrValue;
		temp -> attributes.push_back(tempAttr);
		Logger::printDebugLog("Saved attribute into node memory");
	}
}

XmlNode * XmlParser::createNode (const string &nodeName, int nodePtr, const string nodeVal)
{
	Logger::printDebugLog("Strated creating a new node");
	XmlNode *newNode = new XmlNode(); // TODO: This should be deleted by destructor/
	newNode -> nodeName = nodeName;
	newNode -> nodeVal = nodeVal;
	if(currNode.empty() == true)
	{
		currNode.push(newNode);
	}
	if(head.size() <= nodePtr)
	{
		Logger::printInfoLog("Started a new tree");
		head.push_back(newNode);
		return NULL; //indicates that we have initialized new tree and modified head.
	}
	return newNode;
}

void XmlParser::printTree(XmlNode *node)
{
	Logger::printWarnLog("This function is only for debug purposes and it won't be supported in the future");
	static int nodePtr = 0;
	if(head.size() > nodePtr)
	{
		if(node == NULL)
		{
			node = head[nodePtr];
		}
		int attrSize = node -> attributes.size();
		int subNodesize = node -> subNodes.size();
		int i;
		cout<<"Node name: "<<node -> nodeName<<endl;
		for( i = 0; i < attrSize; ++i)
		{
			cout<<node -> attributes[i] -> attrName<<" = "<<node -> attributes[i] -> attrVal<<endl;
		}
		cout<<"Node Value: "<<node -> nodeVal<<endl<<"-------------------"<<endl;
		for(i = 0; i < subNodesize; ++i)
		{
			printTree(node -> subNodes[i]);
		}
		if(node == head[nodePtr])
		{
			++nodePtr;
			printTree();
		}
	}
	else
	{
		cout<<"End of tree"<<endl;
	}
}

XmlParser::XmlParser()
{
	nodePtr = 0;
}

XmlParser::~XmlParser()
{
	deleteAll();
}

void XmlParser::deleteAll(XmlNode *node)
{
	static int nodePtr = 0;
	string log = "Strated destroyin all nodes with current tree ";
	log += nodePtr;
	Logger::printDebugLog(log);
	static stack<XmlNode *> nodeStack;
	if(head.size() > nodePtr)
	{
		if(node == NULL)
		{
			node = head[nodePtr];
		}
		int attrSize = node -> attributes.size();
		int subNodesize = node -> subNodes.size();
		int i;
		Logger::printDebugLog("Strated destroyin all attributes");
		for( i = attrSize - 1; i >= 0; --i)
		{
			delete node -> attributes[i];
// 			cout<<node -> attributes[i] -> attrName<<" = "<<node -> attributes[i] -> attrVal<<endl;
		}
// 		cout<<"Node Value: "<<node -> nodeVal<<endl<<"-------------------"<<endl;
		Logger::printDebugLog("Strated saving all subnodes to memeory to destroy them");
		for(i = 0; i < subNodesize; ++i)
		{
			nodeStack.push(node -> subNodes[i]);
			deleteAll(node -> subNodes[i]);
		}
		Logger::printDebugLog("Current tree stored in memory and starting actual delete operations");
		if(node == head[nodePtr])
		{
// 			head.clear();
			Logger::printDebugLog("Deleting all nodes in memory for current tree");
			while(!nodeStack.empty())
			{
				node = nodeStack.top();
				nodeStack.pop();
				delete node;
			}
			++nodePtr;
			string log = "Current tree destroyed, starting next tree ";
			log += nodePtr;
			Logger::printDebugLog(log);
			deleteAll();
		}
	}
	else
	{
		head.clear();
	}
}

bool XmlParser::loadXmlTree(const string &data) //This will generate a tree and load it into memeory
{
	Logger::printInfoLog("Loading XML Data...");
	vectorizeString(data);
	return true;
}

string XmlParser::operator[](int num) //This will return attributes value.
{
	Logger::printDebugLog("Atrribute requested with [number]");
	string result;
	if(num > 0)
	{
		getAttributeValue(result, num);
	}
	return result;
}

string XmlParser::operator[](const string &attr) //This will return attributes value.
{
	Logger::printDebugLog("Attribute reequested with [string]");
	string result;
	getAttributeValue(result, attr);
	return result;
}

bool XmlParser::getNodeValue(std::string &result) //This will nodes attributes value.
{
	Logger::printDebugLog("Node value requested");
	XmlNode *node = currNode.top();
	if(node -> nodeName.size() > 0)
	{
		result = node -> nodeVal;
		return true;
	}
	Logger::printErrorLog("Error with current node, can not return its value. Size of the name of the node is <= 0");
	return false;
}

bool XmlParser::getAttributeValue(string &result, const string &attr) //This will return attributes value.
{
	string log  = "Looking for an attribute value... ";
	log += attr;
	Logger::printDebugLog(log);
	XmlNode *node = currNode.top();
	int attrSize = node -> attributes.size();
	for(int i = 0; i < attrSize; ++i)
	{
		if(node -> attributes[i] -> attrName == attr)
		{
			result = node -> attributes[i] -> attrVal;
			Logger::printDebugLog("Attribute found");
			return true;
		}
	}
	Logger::printDebugLog("Attribute can not be found");
	return false;
}

bool XmlParser::getAttributeValue(string &result, int num) //This will return attributes value.
{
	string log  = "Looking for an attribute value by number... ";
	log += num;
	Logger::printDebugLog(log);
	XmlNode *node = currNode.top();
	if(node -> attributes.size() >= num)
	{
		result = node -> attributes[num - 1] -> attrVal;
		Logger::printDebugLog("Attribute found");
		return true;
	}
	Logger::printDebugLog("Attribute can not be found");
	return false;
}

bool XmlParser::findNode (const string &nodeName) // This is the function which we will show to user
{
	string log  = "Looking for a node... ";
	log += nodeName;
	Logger::printDebugLog(log);
	stack <XmlNode *>tempHead = currNode;
	if( findNode (NULL, nodeName) == false)
	{
		Logger::printDebugLog("Node can not be found");
		currNode = tempHead;
		return false;
	}
	Logger::printDebugLog("Node found");
	return true;
}

bool XmlParser::findNode (XmlNode *head, const string &nodeName) //This is the real traverse function and every other function will call it
{
	//FIXME: Add an algorithm to keep searching from begining until reaches to start point
	Logger::printDebugLog("Started main node finding function...");
	static int nodePtr = 0;
	XmlNode *sNode = currNode.top();
	if(this -> head.size() > nodePtr)
	{
		if(head == NULL)
		{
			currNode.pop();
			if(currNode.empty() == true && this -> head.size() > nodePtr)
			{
				Logger::printDebugLog("Jumping next tree");
				currNode.push(this -> head[++nodePtr]);
			}
			else if(this -> head.size() <= nodePtr)
			{
				return false;
			}
			head = currNode.top();
// 			if(sNode != head[0])
// 			{
// 				printTree(node -> subNodes[i]);
// 			}
// 			node = head[nodePtr];
		}
// 		int attrSize = head -> attributes.size();
		int subNodesize = head -> subNodes.size();
		int i;
		Logger::printDebugLog("Checking current node");
		if(nodeName == head -> nodeName && head != sNode)
		{
			Logger::printDebugLog("Node found here");
			currNode.push(head);
			return true;
		}
		Logger::printDebugLog("Started looking for sub nodes");
		for(i = 0; i < subNodesize; ++i)
		{
			if(head != sNode)
			{
				currNode.push(head);
				if(findNode(head -> subNodes[i], nodeName) == true) //found it don't search any more
				{
					return true;
				}
			}
		}
		Logger::printDebugLog("Completed looking to sub nodes, will try next tree");
		if(head == sNode)
		{
// 			currHead.pop();
			if(findNode(NULL, nodeName) == true) //found it don't search any more
			{
				return true;
			}
		}
	}
	Logger::printDebugLog("Node can not be found");
// 	else
// 	{
		return false;
// 	}
}
