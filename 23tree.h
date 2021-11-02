#ifndef TREE
#define TREE

#include <iostream>
#include<algorithm>
#include<map>
#include <bits/stdc++.h>
//node,tree class
using namespace std;

class node{
	public:
		int key[4];
		int data[4];
		node* child[4];
		node* parent;
		bool isleaf;
		int num;
		node() {
			for (int i = 0; i < 4; i ++) child[i] = NULL;
			parent = NULL;
		}
};
class Tree {
	public:
		node* root;
		Tree(): root(NULL) {}

		void divdeNode(node* t);

		void print_help(node*& t, int deep);

		void print();

		node* searchLeaf(node* t, int key);

		node* searchNode(node* t, int key);

		void insertLeaf(node* t, int key, int val);

		void fixup(node* np);
		
		void insertkey( int key, int val);

		bool deletekey(int key);
};

#endif
