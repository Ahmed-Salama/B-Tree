/*
 * InternalNode.cpp
 *
 *  Created on: May 24, 2011
 *      Author: ahmed
 */
#ifndef INTERNALNODE_CPP_
#define INTERNALNODE_CPP_

#include <vector>
#include "Node.cpp"
#include "DataEntry.cpp"

using namespace std;

class InternalNode: public Node{
public:
	InternalNode(Node* p, int mi, Node* left):Node(p, mi){
		DataEntry* gnd = new DataEntry(-(1<<30), left);
		dataEntries.push_back(gnd);
	}
	void insert(int x){
		int f = 1;
		while(f < dataEntries.size() && x >= dataEntries[f]->getDir())f++;
		dataEntries[f-1]->getPtr()->insert(x);
	}
	void push(DataEntry* de){
		int f = 0;
		//or equal.
		while(f < dataEntries.size() && de->getDir() > dataEntries[f]->getDir())f++;
		addDataEntry(f, de);

		if(size == limit+1){//split.
			//Move half of the data entries to the next sibiling.
			InternalNode* sib = new InternalNode(parent, limit, 0);
			while(size > (limit+1)/2){
				sib->push(dataEntries[dataEntries.size()-1]);
				dataEntries[dataEntries.size()-1]->getPtr()->setParent(sib);
				removeDataEntry(dataEntries.size()-1);
			}

			//connect the siblings.
			sib->setNext(getNext());
			setNext(sib);
			sib->setPrev(this);

			//because I know that it will be remove to move to the above level.
			sib->getDataEntries()[0]->setPtr(sib->getDataEntries()[1]->getPtr());
			sib->getDataEntries()[0]->setDir(sib->getDataEntries()[1]->getDir());

			if(parent == 0){//make a new root.
				InternalNode* newParent = new InternalNode(0, limit, this);

				//set parent.
				parent = newParent;
				sib->setParent(newParent);

				DataEntry* de = new DataEntry(sib->getDataEntries()[1]->getDir(), sib);
				sib->removeDataEntry(1);

				newParent->push(de);
			}else{//add the middle value to the above internal node.
				InternalNode* aboveNode = (InternalNode* )parent;

				DataEntry* de = new DataEntry(sib->getDataEntries()[1]->getDir(), sib);
				sib->removeDataEntry(1);

				aboveNode->push(de);
			}
		}
	}
	void update(){
		if(size >= (limit+1)/2)return;


		//move some to this node from next sibling.
		if(next != 0 && parent == next->getParent() && (size+next->getSize())/2 >= (limit+1)/2){
			//move some of the dataEntries to the next sibling.
			while(size < (limit+1)/2){
				addDataEntry(dataEntries.size(), next->getDataEntries().front());
				next->removeDataEntry(0);
			}

			//Modify the above internal node.
			InternalNode* _parent = (InternalNode*) parent;
			for(int i = 0; i < _parent->getDataEntries().size(); i++){
//				cout << "tries: " << _parent->getDataEntries()[i]->getDir() << endl;
				if(_parent->getDataEntries()[i]->getPtr() == next){
//					cout << "Modified: " << next->getDataEntries()[0] << endl;
					_parent->getDataEntries()[i]->setDir(next->getDataEntries()[0]->getDir());
					break;
				}
			}
			return;
		}

		//move some to this node from prev sibling.
		if(prev != 0 && parent == prev->getParent() && (size+prev->getSize())/2 >= (limit+1)/2){
			//move some of the dataEntries to the next sibling.
			while(size < (limit+1)/2){
				addDataEntry(0, (prev->getDataEntries()[prev->getDataEntries().size()-1]));
				prev->removeDataEntry(prev->getDataEntries().size()-1);
			}

			//Modify the above internal node.
			InternalNode* _parent = (InternalNode*) parent;
			for(int i = 0; i < _parent->getDataEntries().size(); i++){
				if(_parent->getDataEntries()[i]->getPtr() == this){
					_parent->getDataEntries()[i]->setDir(dataEntries[0]->getDir());
					break;
				}
			}
			return;
		}

		//Merge with the next.
		if(next != 0 && parent == next->getParent() && (size+next->getSize()) <= limit){
			//move all the files from the sibling to this node.
			while(next->getDataEntries().size() > 0 ){
				addDataEntry(dataEntries.size(), next->getDataEntries().front());
				next->removeDataEntry(0);
			}

			//Modify the above internal node.
			InternalNode* _parent = (InternalNode*) parent;
			for(int i = 0; i < _parent->getDataEntries().size(); i++){
				if(_parent->getDataEntries()[i]->getPtr() == next){
					_parent->removeDataEntry(i);
					break;
				}
			}

			//Modify my next.
			next = next->getNext();

			parent->update();
			return;
		}

		//Merge with the prev.
		if(prev != 0 && parent == prev->getParent() && (size+prev->getSize()) <= limit){
			//move all the files from this node to the prev node.
			while(size > 0){
				prev->addDataEntry(prev->getDataEntries().size(), dataEntries.front());
				removeDataEntry(0);
			}

			//Modify the above internal node.
			InternalNode* _parent = (InternalNode*) parent;
			for(int i = 0; i < _parent->getDataEntries().size(); i++){
				if(_parent->getDataEntries()[i]->getPtr() == this){
					_parent->removeDataEntry(i);
					break;
				}
			}

			//Modify PREV.
			prev->setNext(next);

			parent->update();
			return;
		}

		//Nothing worked.
		if(parent != 0)
			parent->update();
	}

	void print(int l){
		for(int i = 0; i < dataEntries.size(); i++){
			for(int j = 0; j < l; j++)
				cout << "  ";
			cout << dataEntries[i]->getDir() << endl;

			dataEntries[i]->getPtr()->print(l+1);
		}
	}
	vector<DataEntry* > getDataEntries(){
		return dataEntries;
	}
	void removeDataEntry(int index){
		dataEntries.erase(dataEntries.begin()+index);
		size--;
	}
	void addDataEntry(int index, DataEntry* dataEntry){
		dataEntries.insert(dataEntries.begin()+index, dataEntry);
		size++;
	}
	bool search(int x, Node** container){
		int i = 1;
		while(i < dataEntries.size() && x >= dataEntries[i]->getDir())i++;
		return dataEntries[i-1]->getPtr()->search(x, container);
	}
	void rangeSearch(int f, int t){
		int i = 1;
		while(i < dataEntries.size() && f >= dataEntries[i]->getDir())i++;
		return dataEntries[i-1]->getPtr()->rangeSearch(f, t);
	}
	void remove(int x){
		int i = 1;
		while(i < dataEntries.size() && x >= dataEntries[i]->getDir())i++;
		dataEntries[i-1]->getPtr()->remove(x);
	}

	InternalNode* getNext(){return next;}
	InternalNode* getPrev(){return prev;}
	void setNext(InternalNode* ni){next = ni;}
	void setPrev(InternalNode* pi){prev = pi;}

private:
	vector<DataEntry*> dataEntries;
	InternalNode* next;
	InternalNode* prev;
};

#endif
