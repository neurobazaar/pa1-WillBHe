#include "Node.h"
#include <cstring>

DataStringNode::DataStringNode(char* buffer)
{
	strcpy(this->buffer, buffer);
}

DataStringNode::~DataStringNode()
{
}

PointerNode::PointerNode(char* buffer, int count)
	: pData(new DataStringNode(buffer)),
	pNext(nullptr),
	pPrev(nullptr),
	count(count)
{
}

PointerNode::PointerNode(char* buffer, int count, PointerNode* pPrev)
	: pData(new DataStringNode(buffer)),
	pNext(nullptr),
	pPrev(pPrev),
	count(count)
{
}

PointerNode::~PointerNode()
{
	delete pData;
}
