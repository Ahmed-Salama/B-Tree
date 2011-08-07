/*
 * BufferPool_FIFO.cpp
 *
 *  Created on: May 30, 2011
 *      Author: ahmed
 */

#ifndef BUFFERPOOL_FIFO_CPP_
#define BUFFERPOOL_FIFO_CPP_

#include "BufferPool.cpp"
#include <vector>
#include <iostream>


class BufferPool_FIFO: public BufferPool{
public :
	BufferPool_FIFO(int size){
		for(int i = 0; i < size; i++)
			q.insert(q.begin(), 0);
	}
	void addBuffer(LeafNode* leafNode){
		q.insert(q.begin(), leafNode);
		q.pop_back();
	}
	bool contain(LeafNode* leafNode){
		for(int i = 0; i < q.size(); i++)
			if(q[i] == leafNode)return true;
		return false;
	}
	bool searchBuffer(int x){
		Node* containerLeafNode = 0;
		for(int i = 0; i < q.size(); i++){
			if(q[i] == 0)continue;

			q[i]->search(x, &containerLeafNode);

			if(containerLeafNode == 0)continue;
			else{

				//Add the adjacent leafNodes.
//				if(_containerLeafNode->getNext() != 0 && !contain(_containerLeafNode->getNext()))
//					addBuffer(_containerLeafNode->getNext());
//				if(_containerLeafNode->getPrev() != 0 && !contain(_containerLeafNode->getPrev()))
//					addBuffer(_containerLeafNode->getPrev());
				return true;
			}
		}
		return false;
	}
private :
	vector<LeafNode*> q;
};

#endif
