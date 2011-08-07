/*
 * BufferPool.cpp
 *
 *  Created on: May 29, 2011
 *      Author: ahmed
 */

#ifndef BUFFERPOOL_CPP_
#define BUFFERPOOL_CPP_

#include "LeafNode.cpp"

class BufferPool{
public:
	void virtual addBuffer(LeafNode* leafNode){

	}
	bool virtual searchBuffer(int x){
		return false;
	}
};

#endif
