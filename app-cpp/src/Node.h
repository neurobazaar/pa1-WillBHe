#pragma once

class DataStringNode
{
public:
	char buffer[100];
	DataStringNode(char* buffer);
	~DataStringNode();
};

class PointerNode
{
public:
	DataStringNode* pData;
	PointerNode* pNext;
	PointerNode* pPrev;
	int count; 
public:
	PointerNode(char* buffer, int count);
	PointerNode(char* buffer, int count, PointerNode* pPrev);
	~PointerNode();

	static void First();

private:

};
