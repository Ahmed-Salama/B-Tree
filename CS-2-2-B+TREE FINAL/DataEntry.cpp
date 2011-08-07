/*
 * DataEntry.cpp
 *
 *  Created on: May 25, 2011
 *      Author: ahmed
 */
#include "Node.cpp"

#ifndef DATAENTRY_CPP_
#define DATAENTRY_CPP_

class DataEntry{
public:
	DataEntry(int diri, Node* ptri){
		dir = diri;
		ptr = ptri;
	}
	int getDir(){return dir;}
	Node* getPtr(){return ptr;}
	void setDir(int diri){dir = diri;}
	void setPtr(Node* ptri){ptr = ptri;}
private:
	int dir;
	Node* ptr;
};

#endif
