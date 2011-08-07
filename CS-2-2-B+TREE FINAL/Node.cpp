/*
 * Node.cpp
 *
 *  Created on: May 24, 2011
 *      Author: ahmed
 */


#ifndef NODE_CPP_
#define NODE_CPP_

#include <vector>
#include <iostream>

using namespace std;

class Node{
public:
	Node(Node* p, int mi){
		size = 0;
		parent = p;
		limit = mi;
	}

	Node* getParent(){return parent;}
	void setParent(Node* newParent){parent = newParent;}
	int getLimit(){return limit;}
	void setLimit(int newLimit){limit = newLimit;}
	int getSize(){return size;}
	void setSize(int newSize){size = newSize;}

	bool virtual search(int x, Node** container){return 0;}

	void virtual print(int l){}
	void virtual insert(int x){}
	void virtual rangeSearch(int f, int t){}
	void virtual remove(int x){}
	void virtual update(){}
protected:
	int limit, size;
	Node* parent;
};

#endif /* NODE_CPP_ */
