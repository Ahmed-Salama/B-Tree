/*
 * BTree.cpp
 *
 *  Created on: May 24, 2011
 *      Author: ahmed
 */
#include <algorithm>

#ifndef BTREE_CPP_
#define BTREE_CPP_

#include "LeafNode.cpp"
#include "InternalNode.cpp"
#include "Node.cpp"
#include "BufferPool.cpp"
#include "BufferPool_FIFO.cpp"
#include "BufferPool_RANK.cpp"

#include <fstream>

class BTree{
public:
	BTree(int limiti){
		bufferpool = new BufferPool_RANK(4);
		limit = limiti;
		root = new LeafNode(0, limit);
	}
	void insert(int x){
		root->insert(x);
		updateRoot();
	}
	void print(int l){
		cout << "Printing" << endl;
		root->print(l);
	}
	bool search(int x){
		//check it in the bufferpool first;
		if(bufferpool->searchBuffer(x))return true;
		cout << "Not Found in the Buffer" << endl;

		//Search for it in the BTree.
		Node* containerLeafNode = 0;
		root->search(x, &containerLeafNode);

		if(containerLeafNode == 0)return false;
		else{//Add it to the bufferPool.
			LeafNode* _containerLeafNode = (LeafNode*)containerLeafNode;
			bufferpool->addBuffer(_containerLeafNode);
//			if(_containerLeafNode->getNext() != 0)	bufferpool->addBuffer(_containerLeafNode->getNext());
//			if(_containerLeafNode->getPrev() != 0)bufferpool->addBuffer(_containerLeafNode->getPrev());

			return true;
		}
	}
	void rangeSearch(int f, int t){
		if(f > t)swap(f, t);
		root->rangeSearch(f, t);
		cout << endl;
	}
	void remove(int x){
		root->remove(x);
		updateRoot();
	}
	void bulkLoad(string fileName, int limiti){
		limit = limiti;

		//Load the file.
		vector<int> a;
		readArray(fileName, a);

		//~~ Testing only ~~
//		for(int i = 1; i <= 30; i++)
//			a.push_back(i);

		sort(a.begin(), a.end());

		if(a.size() <= limit){//fit in one leaf node.
			root = new LeafNode(0, limit);
			for(int i = 0; i < a.size(); i++)
				root->insert(a[i]);
		}else{
			vector<DataEntry*> searchGuide;
			vector<LeafNode*> leafNodes;

			//Build the leafNodes and initial search guides.
			for(int i = 0; i < a.size(); i+=limit){
				LeafNode* curr = new LeafNode(0, limit);
				for(int j = i; j < i+limit && j < a.size(); j++)
					curr->insert(a[j]);
				leafNodes.push_back(curr);
				if(i+limit < a.size())
					searchGuide.push_back(new DataEntry(a[i+limit], 0));
			}

			//add ground.
			searchGuide.insert(searchGuide.begin()+0, new DataEntry(-(1<<30), 0));

			//fix the pointers.
			for(int i = 0; i < searchGuide.size(); i++)
				if(i < leafNodes.size())searchGuide[i]->setPtr(leafNodes[i]);

			//link all the leaf nodes together.
			for(int i = 0; i < leafNodes.size(); i++){
				if(i > 0)leafNodes[i]->setPrev(leafNodes[i-1]);
				if(i < leafNodes.size())leafNodes[i]->setNext(leafNodes[i+1]);
			}


			//Build the internal Nodes.
			while(searchGuide.size() > limit){
				vector<DataEntry*> newSearchGuide;
				vector<InternalNode*> internalNodes;

				//pack the data Entries to the internal nodes and take the data Entries of the following level.
				for(int i = 0; i < searchGuide.size(); i+=limit+1){
					InternalNode* curr = new InternalNode(0, limit, 0);
					if(i == 0){
						curr->removeDataEntry(0);//remove the ground node.
					}
					for(int j = i; j < i+limit && j < searchGuide.size(); j++){
						curr->push(searchGuide[j]);
						if(searchGuide[j]->getPtr() != 0)searchGuide[j]->getPtr()->setParent(curr);
					}

					internalNodes.push_back(curr);

					if(i+limit < searchGuide.size())
						newSearchGuide.push_back(searchGuide[i+limit]);
				}

				//add ground node.
				newSearchGuide.insert(newSearchGuide.begin(), new DataEntry(-(1<<30), 0));

				//fix the pointers.
				for(int i = 0; i < newSearchGuide.size(); i++){
					if(i < internalNodes.size()){
						if(i > 0){
							internalNodes[i]->getDataEntries()[0]->setDir(newSearchGuide[i]->getDir());
							internalNodes[i]->getDataEntries()[0]->setPtr(newSearchGuide[i]->getPtr());
						}
						newSearchGuide[i]->setPtr(internalNodes[i]);
					}
				}

				searchGuide = newSearchGuide;
			}

			//gather the remaining data Entries in one internal node.
			InternalNode* _root = new InternalNode(0, limit, 0);
			_root->removeDataEntry(0);
			for(int i = 0; i < searchGuide.size(); i++){
				_root->push(searchGuide[i]);
				searchGuide[i]->getPtr()->setParent(_root);
			}

			root = _root;
		}
	}
private:
	void readArray(string fileName, vector<int> &a) {
		ifstream infile(fileName.c_str());

		int x;
		while (infile) {
			infile >> x;
			if(!infile)break;

			a.push_back(x);
		}

		infile.close();
	}
	void updateRoot(){
		//up.
		while(root->getParent() != 0)
			root = root->getParent();

		//down.
		InternalNode* check;
		while((check = dynamic_cast<InternalNode*>(root)) && (root->getSize() == 0)){
			InternalNode* _root = (InternalNode*) root;
			root = _root->getDataEntries()[0]->getPtr();
		}
	}
	Node* root;
	int limit;
	BufferPool* bufferpool;
};

#endif
