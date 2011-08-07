/*
 * BufferPool_RANK.cpp
 *
 *  Created on: May 30, 2011
 *      Author: ahmed
 */

#ifndef BUFFERPOOL_RANK_CPP_
#define BUFFERPOOL_RANK_CPP_

#include "BufferPool.cpp"
#include <vector>
#include <iostream>


class BufferPool_RANK: public BufferPool{
public :
	BufferPool_RANK(int size){
		for(int i = 0; i < size; i++){
			array.push_back(0);
			rank.push_back(0);
		}
	}
	void addBuffer(LeafNode* leafNode){
		int minIndex = 0;
		for(int i = 1; i < array.size(); i++)
			if(rank[minIndex] > rank[i])
				minIndex = i;

		array[minIndex] = leafNode;
	}
	bool contain(LeafNode* leafNode){
		for(int i = 0; i < array.size(); i++)
			if(array[i] == leafNode)return true;
		return false;
	}
	bool searchBuffer(int x){
		Node* containerLeafNode = 0;
		for(int i = 0; i < array.size(); i++){
			if(array[i] == 0)continue;
			cout << i << endl;

			array[i]->search(x, &containerLeafNode);

			if(containerLeafNode == 0)continue;
			else{
				LeafNode* _containerLeafNode = (LeafNode*)containerLeafNode;
				for(int i = 0 ; i < array.size(); i++)
					if(array[i] == _containerLeafNode){
						rank[i]++;
						return true;
					}
			}
		}
		return false;
	}
private :
	vector<LeafNode*> array;
	vector<int> rank;
};

#endif
