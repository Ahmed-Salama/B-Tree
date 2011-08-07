/*
 * Main.cpp
 *
 *  Created on: May 24, 2011
 *      Author: ahmed
 */

#include <iostream>
#include <sstream>
#include "BTree.cpp"
#include <sstream>
#include <stdio.h>

using namespace std;

//int main(){
//	BTree* bt = new BTree(4);
//	bt->insert(10);
//	bt->insert(20);
//	bt->insert(30);
//	bt->insert(40);
//	bt->insert(50);
//	bt->insert(60);
//	bt->insert(70);
//	bt->insert(80);
//	bt->insert(90);
//	bt->insert(100);
//	bt->insert(110);
//	bt->insert(120);
//	bt->insert(130);
//
////	bt->insert(75);
////	bt->insert(77);
////	bt->insert(76);
////	bt->insert(73);
//
////	bt->insert(35);
//
////	bt->remove(50);
//
////	bt->remove(60);
////	bt->remove(77);
////	bt->remove(80);
////	bt->remove(90);
//
////	bt->remove(100);
//
////	bt->remove(20);
////	bt->remove(30);
//
//
//	bt->remove(90);
////	bt->remove(110);
////	bt->remove(70);
////	bt->remove(100);
//
//	bt->insert(35);
//	bt->insert(37);
//	bt->insert(33);
//
//	bt->remove(100);
//
//	bt->remove(70);
////	bt->remove(80);
//
//	bt->remove(10);
//
//	bt->print(0);
//
//
//	for(int i = 9; i <= 20; i++)
//		cout << i << ": " << bt->search(i) << endl;
//
//	bt->rangeSearch(12, 17);
//
//	cout << endl << "Done" << endl;
//	return 0;
//}

int main(){
	int limit;
	cout << "Enter the limit of the tree: " << endl;
	cin >> limit;

	BTree* bt = new BTree(limit);

	string fileName;
	cout << "Enter the file name to load from: ['*' will not load any.]" << endl;
	cin >> fileName;

	if(fileName != "*"){
		bt->bulkLoad(fileName, limit);
		bt->print(0);
	}

	while(true){
		string command;
		cin >> command;

		if(command[0] == 'i'){
			command.erase(0, 1);
			int x;
			sscanf(command.c_str(), "%d", &x);
			bt->insert(x);
		}else if(command[0] == 'r'){
			command.erase(0, 1);
			int x;
			sscanf(command.c_str(), "%d", &x);
			bt->remove(x);
		}else if(command[0] == 's'){
			command.erase(0, 1);
			int x;
			sscanf(command.c_str(), "%d", &x);
			cout << "Found: " << (bt->search(x)?"True":"False") << endl;
		}else if(command[0] == '['){
			command.erase(0, 1);
			command.erase(command.size()-1, 1);

			string x;
			stringstream ss(command);

			vector<string> region;
			while(getline(ss, x, ',')) {
				region.push_back(x);
			}

			int f = atoi(region[0].c_str());
			int t = atoi(region[1].c_str());

			cout << "Result in range: " << endl;
			bt->rangeSearch(f, t);
		}

		bt->print(0);
	}

	return 0;
}

