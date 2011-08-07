/*
 * LeafNode.cpp
 *
 *  Created on: May 24, 2011
 *      Author: ahmed
 */
#ifndef LEAFNODE_CPP_
#define LEAFNODE_CPP_

#include "Node.cpp"
#include "InternalNode.cpp"
#include "DataEntry.cpp"

class LeafNode: public Node{
public:
	LeafNode(Node* p, int mi):Node(p, mi){

	}
	void insert(int x){
		int f = 0;
		while(f < files.size() && x >= files[f])f++;
		addFile(f, x);

		if(size == limit+1){//split.
			//move half of the contents to a new sibling.
			LeafNode* sib = new LeafNode(parent, limit);
			while(size > (limit+1)/2){
				sib->insert(files[files.size()-1]);
				removeFile(files.size()-1);
			}

			//connect the siblings.
			sib->setNext(getNext());
			setNext(sib);
			sib->setPrev(this);

			if(parent == 0){//make a new root.
				InternalNode* newParent = new InternalNode(0, limit, this);

				parent = newParent;
				sib->setParent(newParent);

				DataEntry* de = new DataEntry(sib->getFiles()[0], sib);
				newParent->push(de);
			}else{//add the middle value to the above internal node.
				InternalNode* aboveNode = (InternalNode* )parent;

				DataEntry* de = new DataEntry(sib->getFiles()[0], sib);
				aboveNode->push(de);
			}
		}
	}
	void print(int l){
		for(int i = 0; i < files.size(); i++){
			for(int j = 0; j < l; j++)
				cout << "  ";
			cout << files[i] << endl;
		}
	}
	bool search(int x, Node** container){
		for(int i = 0; i < files.size(); i++)
			if(files[i] == x){
				*container = this;
				return true;
			}

		container = 0;
		return false;
	}
	void rangeSearch(int f, int t){
		for(int i = 0; i < files.size(); i++)
			if(f <= files[i] && files[i] <= t)
				cout << files[i] << "  ";
		if(next != 0 && t > files[files.size()-1])next->rangeSearch(f, t);
	}
	void remove(int x){
		for(int i = 0; i < files.size(); i++)
			if(files[i] == x){
				removeFile(i);
				break;
			}

		update();
	}

	void update(){
		if(size >= (limit+1)/2)return;

//		cout << "problem" << endl;
		//move some to this node from next sibling.
		if(next != 0 && parent == next->getParent() && (size+next->getSize())/2 >= (limit+1)/2){
			//move some of the files to the next sibling.
			while(size < (limit+1)/2){
				addFile(files.size(), next->getFiles().front());
				next->removeFile(0);
			}

			//Modify the above internal node.
			InternalNode* _parent = (InternalNode*) parent;
			for(int i = 0; i < _parent->getDataEntries().size(); i++){
				cout << "tries: " << _parent->getDataEntries()[i]->getDir() << endl;
				if(_parent->getDataEntries()[i]->getPtr() == next){
//					cout << "Modified: " << next->getFiles()[0] << endl;
					_parent->getDataEntries()[i]->setDir(next->getFiles()[0]);
					break;
				}
			}

			return;
		}

		//move some to this node from prev sibling.
		if(prev != 0 && parent == prev->getParent() && (size+prev->getSize())/2 >= (limit+1)/2){
			//move some of the files to the next sibling.
			while(size < (limit+1)/2){
				addFile(0, (prev->getFiles()[prev->getFiles().size()-1]));
				prev->removeFile(prev->getFiles().size()-1);
			}

			//Modify the above internal node.
			InternalNode* _parent = (InternalNode*) parent;
			for(int i = 0; i < _parent->getDataEntries().size(); i++){
				if(_parent->getDataEntries()[i]->getPtr() == this){
					_parent->getDataEntries()[i]->setDir(files[0]);
					break;
				}
			}
			return;
		}

		//Merge with the next.
		if(next != 0 && parent == next->getParent() && (size+next->getSize()) <= limit){
			//move all the files from the sibling to this node.
			while(next->getFiles().size() > 0 ){
				addFile(files.size(), next->getFiles().front());
				next->removeFile(0);
			}

			//Modify the above internal node.
			InternalNode* _parent = (InternalNode*) parent;
			for(int i = 0; i < _parent->getDataEntries().size(); i++){
				if(_parent->getDataEntries()[i]->getPtr() == next){
					_parent->removeDataEntry(i);
					break;
				}
			}

			//modify my next.
			next = next->getNext();

			parent->update();
			return;
		}

		//Merge with the prev.
		if(prev != 0 && parent == prev->getParent() && (size+prev->getSize()) <= limit){
			//move all the files from this node to the prev node.
			while(size > 0){
				prev->addFile(prev->getFiles().size(), files.front());
				removeFile(0);
			}


			//Modify the above internal node.
			InternalNode* _parent = (InternalNode*) parent;
			for(int i = 0; i < _parent->getDataEntries().size(); i++){
				if(_parent->getDataEntries()[i]->getPtr() == this){
					_parent->removeDataEntry(i);
					break;
				}
			}

			//modify PREV.
			prev->setNext(next);

			parent->update();
			return;
		}

		//Nothing worked.
		if(parent != 0)
			parent->update();

	}

	void addFile(int index, int file){
		files.insert(files.begin()+index, file);
		size++;
	}
	void removeFile(int index){
		files.erase(files.begin()+index);
		size--;
	}
	vector<int> getFiles(){return files;}
	LeafNode* getNext(){return next;}
	LeafNode* getPrev(){return prev;}
	void setNext(LeafNode* ni){next = ni;}
	void setPrev(LeafNode* pi){prev = pi;}
private:
	vector<int> files;
	LeafNode* next;
	LeafNode* prev;
};

#endif
