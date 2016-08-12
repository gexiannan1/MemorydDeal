#ifndef _ZXMLPARSE_H_
#define _ZXMLPARSE_H_
#include <boost/lexical_cast.hpp>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>


class zXMLParser
{
	public:
		zXMLParser();
		~zXMLParser();

		bool initFile(const std::string &xmlFile);
		bool initFile(const char *xmlFile);
		bool initStr(const std::string &xmlStr);
		bool initStr(const char *xmlStr);
		bool init();
		void final();
		std::string & dump(std::string & s, bool format=false);
		std::string & dump(xmlNodePtr dumpNode,std::string & s,bool head=true);
		xmlNodePtr getRootNode(const char *rootName);
		xmlNodePtr getChildNode(const xmlNodePtr parent, const char *childName);
		xmlNodePtr getNextNode(const xmlNodePtr node,const char *nextName);
		unsigned int getChildNodeNum(const xmlNodePtr parent, const char *childName);

		xmlNodePtr newRootNode(const char *rootName);
		xmlNodePtr newChildNode(const xmlNodePtr parent, const char *childName, const char *content);
		bool newNodeProp(const xmlNodePtr node,const char *propName,const char *prop);

		///
		/// NOTE: getNodePropNum2() interprets bytes differently than getNodePropNum().
		/// Version 2 interpretes bytes an integers, while version 1 interprets them as
		/// binary strings, as "char * foo = '\0';"
		///
		template<typename Numeric>
		void getNodePropNum2(const xmlNodePtr node,const char *propName,Numeric *prop);
		/// Like getNodePropNum2(), but falls back on a default value if the parse failes.
		template<typename Numeric>
		bool tryGetNodePropNum2(const xmlNodePtr node,const char *propName,Numeric *prop, Numeric fallback);
		bool getNodePropNum(const xmlNodePtr node,const char *propName,void *prop,int propSize);
		bool getNodePropFloat(const xmlNodePtr node,const char *propName,void *prop,int propSize);
		bool getNodePropStr(const xmlNodePtr node,const char *propName,void *prop,int propSize);
		bool getNodePropStr(const xmlNodePtr node,const char *propName,std::string &prop);
		bool getNodeContentNum(const xmlNodePtr node,void *content, int contentSize);
		bool getNodeContentStr(const xmlNodePtr node,void *content, int contentSize);
		bool getNodeContentStr(const xmlNodePtr node,std::string &content);
	private:
		unsigned char *charConv(unsigned char *in, const char *fromEncoding, const char *toEncoding);
		xmlDocPtr doc;
};

template<typename Numeric>
void zXMLParser::getNodePropNum2(const xmlNodePtr node,const char *propName,Numeric *prop)
{
	char *temp=NULL;
	temp = (char *)xmlGetProp(node, (const xmlChar *) propName);

	*prop = boost::lexical_cast<Numeric>(temp);

	if(temp) xmlFree(temp);
}

template<typename Numeric>
bool zXMLParser::tryGetNodePropNum2(const xmlNodePtr node,const char *propName,Numeric *prop,Numeric fallback)
{
	char *temp=NULL;
	temp = (char *)xmlGetProp(node, (const xmlChar *) propName);

	try
	{
		*prop = boost::lexical_cast<Numeric>(temp);
		if(temp) xmlFree(temp);
		return true;
	}
	catch (boost::bad_lexical_cast &ex)
	{
		*prop = fallback;
		if(temp) xmlFree(temp);
		return false;
	}
}

#endif

