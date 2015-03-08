/*
 * XmlParser.h
 * Main Class to manage XML operations
 * Created by: Ozgur Pekcagliyan - 2014-10-04 01:17:32 PM EEST
 * Last edited by: Ozgur Pekcagliyan - 2015-03-07
 * Notes: 
 * FIXME:This implementation always expects a tree hierarchy, there should be always 2 head nodes but it should be flexible to accept multiple trees.
*/

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <vector>
#include <stack>
#include "Logger.h"
// #include <map> //this might be used to build a tree


namespace XmlSpace
{
	typedef struct XmlAttribute
	{
		std::string attrName;
		std::string attrVal;
	}XmlAttribute;

	typedef struct XmlNode
	{
		std::string nodeName;
		std::vector <XmlAttribute *> attributes;
		std::string nodeVal;
		std::vector <struct XmlNode *> subNodes;
	}XmlNode;

// 	template <class T>
	class XmlParser
	{
	public:
		XmlParser();
		~XmlParser();
		
		bool loadXmlTree(const std::string &data); //This will generate a tree and load it into memeory.
		bool getNodeValue(std::string &result); //This will nodes attributes value.
		std::string operator[](const std::string &attr); //This will return attributes value.
		std::string operator[](int num); //This will return attributes value.
		bool getAttributeValue(std::string &result, const std::string &attr); //This will return attributes value.
		bool getAttributeValue(std::string &result, const int num = 1); //This will return attributes value.
		void printTree(XmlNode *node = NULL); //This will print the tree // TODO:This will be romeved in working copy
// 		T getAttributeValue(const string &attr); //This will call one of the traverse functions and return attributes value.
		bool findNode (const std::string &nodeName); // This is the function which we will show to user
	private:
		bool findNode (XmlNode *head, const std::string &nodeName); //This is the real traverse function and every other function will call it.
		void attributeFinder(std::string tempToken, std::stack<XmlNode *> &nodeStack, int &nodePtr, size_t &posSpace, std::string delim = ""); //this will find attributes from token and put them inside node stack
		void deleteAll(XmlNode *node = NULL);
		XmlNode * createNode (const std::string &nodeName, int nodePtr = 0, const std::string nodeVal = ""); // This function will create and return a node back to caller.
		void vectorizeString (const std::string &str); // This will tokenize a string and fill a vector.
		std::vector<XmlNode *> head;
		std::stack<XmlNode *> currNode; //will hold the next node in tree at the top of stack in order to be able to search
		int nodePtr; //will hold ptr value for head vector;
                
                Logger *logger;
	};

}

#endif