#include <iostream>
#include<algorithm>
#include<map>
#include "23tree.h"
#include <bits\stdc++.h>
using namespace std;
node* Tree:: searchLeaf(node* t, int key )
{
	if (t->isleaf) return t;
	if (key < t->key[0])
		return searchLeaf(t->child[0], key);
	else if ((key < t->key[1] && t->num == 2) || t->num == 1)
		return searchLeaf(t->child[1], key);
	else
		return searchLeaf(t->child[2], key);
}

node* Tree:: searchNode(node* t, int key)
{
	if (!t) return NULL;
	for (int i = 0; i < t->num; i++)
		if (t->key[i] == key)
			return t;

	if (t->num == 1) {
		if (key > t->key[0])
			return searchNode(t->child[1], key);
		else
			return searchNode(t->child[0], key);
	} else {
		if (key < t->key[0])
			return searchNode(t->child[0], key);
		else if (key > t->key[1])
			return searchNode(t->child[2], key);
		else
			return searchNode(t->child[1], key);
	}
}

void Tree:: insertLeaf(node* t, int key, int val)
{
	if (key < t->key[0]) {
		t->key[2] = t->key[1];
		t->data[2] = t->data[1];
		t->key[1] = t->key[0];
		t->data[1] = t->data[0];
		t->key[0] = key;
		t->data[0] = val;
	}
	else if (key < t->key[1] || t->num == 1) {
		t->key[2] = t->key[1];
		t->data[2] = t->data[1];
		t->key[1] = key;
		t->data[1] = val;
	}
	else {
		t->key[2] = key;
		t->data[2] = val;
	}
	t->num++;
	if (t->num > 2) divdeNode( t );
	return;
}

void Tree:: divdeNode(node* t)
{
	node* p = t->parent;
	node* sib = new node();
	int k = t->key[1];
	int v = t->data[1];
	sib->key[0] = t->key[2];
	sib->data[0] = t->data[2];
	sib->num = 1;
	sib->parent = p;
	sib->isleaf = t->isleaf;
	if (!t->isleaf){
		sib->child[0] = t->child[2];
		sib->child[1] = t->child[3];
		t->child[2]->parent = sib;
		t->child[3]->parent = sib;
	}
	t->child[2] = NULL; t->child[3] = NULL;
	t->num = 1;

	if (p == NULL) {
		root = new node();
		root->isleaf = 0;
		root->key[0] = k;
		root->data[0] = v;
		root->num = 1;
		root->child[0] = t;
		root->child[1] = sib;
		t->parent = root;
		sib->parent = root;
		return;
	}
	if (k < p->key[0]) {
		p->key[2] = p->key[1];
		p->data[2] = p->data[1];
		p->key[1] = p->key[0];
		p->data[1] = p->data[0];
		p->key[0] = k;
		p->data[0] = v;
		p->child[3] = p->child[2];
		p->child[2] = p->child[1];
		p->child[1] = sib;
	}
	else if (k < p->key[1] || p->num == 1) {
		p->key[2] = p->key[1];
		p->data[2] = p->data[1];
		p->key[1] = k;
		p->data[1] = v;
		p->child[3] = p->child[2];
		p->child[2] = sib;
	}
	else {
		p->key[2] = k;
		p->data[2] = v;
		p->child[3] = sib;
	}
	p->num++;
	if (p->num > 2) {
		divdeNode( p );
	}
	return;
}

void Tree:: insertkey( int key, int val)
{
	if (root == NULL) {
		root = new node();
		root->key[0] = key;
		root->data[0] = val;
		root->num=1;
		root->isleaf = 1;
		return;
	}
	node* t = searchLeaf(this->root, key);
	insertLeaf(t, key, val);
	return;
}

void Tree:: print()
{
	print_help(root, 0);
}

void Tree::fixup( node* np)
{
	node* p = np->parent;
	node* sp = NULL;
	if ( p == NULL ) {
		if (np->child[0]) {
			root = np->child[0];
			root->parent = 0;
		} else {
			root = 0;
		}
		delete np;
	} else if (np->isleaf) {
		if (p->num == 1) {
			if (np == p->child[0]) {
				sp = p->child[1];
				if (sp->num==2) {
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					p->key[0] = sp->key[0];
					p->data[0] = sp->data[0];
					sp->key[0] = sp->key[1];
					sp->data[0] = sp->data[1];
					sp->num = 1;
					np->num = 1;
				} else {
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					np->key[1] = sp->key[0];
					np->data[1] = sp->data[0];
					np->num = 2;
					p->num = 0;
					sp->num = 0;
					delete sp;
					p->child[0] = np;
					p->child[1] = 0;
					fixup(p);
				}
			} else if (np == p->child[1]) {
				sp = p->child[0];
			//		    p(c)              p(b)
			//        /   \      -->     /    \
			//    sp(a,b)  np(X)     sp(a)  np(c)
				if (sp->num == 2) {
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					p->key[0] = sp->key[1];
					p->data[0] = sp->data[1];
					np->num = 1;
					sp->num = 1;
				} else if (sp->num == 1) {
			//      p(c)              p(X)
			//     /    \      -->    /   \
			//  sp(a)   np(X)      sp(a,c)  np(--)
					sp->key[1] = p->key[0];
					sp->data[1] = p->data[0];
					sp->num = 2;
					delete np;
					np = NULL;
					p->child[1] = NULL;
					p->num = 0;
					fixup(p);
				}
			}
		} else if (p->num == 2) {
			if (np == p->child[0]) {
				sp = p->child[1];
				if (sp->num == 2) {
		//        p(a,b)          p(c,b)
		//        /  \      -->   /  \   \
		//     np(X) sp(c,d)    np(a) sp(d)
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					p->key[0] = sp->key[0];
					p->data[0] = sp->data[0];
					sp->key[0] = sp->key[1];
					sp->data[0] = sp->data[1];
					np->num = 1;
					sp->num = 1;
				} else {
		//      p(a,b)            p(b)
		//     /   \    \   -->   /   \
		//   np(x) sp(c) D     np(a,c) D
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					np->key[1] = sp->key[0];
					np->data[1] = sp->data[0];
					p->key[0] = p->key[1];
					p->data[0] = p->data[1];
					p->child[1] = p->child[2];
					delete sp;
					p->child[2] = NULL;
					np->num = 2;
					p->num = 1;
				}
			} else if (np == p->child[1]) {
				if (p->child[2]->num ==2) {
			//    p(a, b)
			//   /   \    \
			//  D   np(X) sp(c,d)

			//   p(a ,c)
			//  /   \    \
			// D    np(b) sp(d)
					sp = p->child[2];
					np->key[0] = p->key[1];
					np->data[0] = p->data[1];
					p->key[1] = sp->key[0];
					p->data[1] = sp->data[0];
					np->num = 1;
					sp->key[0] = sp->key[1];
					sp->data[0] = sp->data[1];
					sp->num = 1;
				} else if (p->child[0]->num == 2){
			//    p(a, b)
			//   /   \     \
			//sp(c,d) np(X) D

			//   p(d ,b)
			//  /   \    \
			//sp(c) np(a) D
					sp = p->child[0];
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					p->key[0] = sp->key[1];
					p->data[0] = sp->data[1];
					sp->num = 1;
					np->num = 1;
				} else {
			//    p(a,b)              p(b)
			//  /    \    \    -->  /     \
			// sp(c) np(X) D(d)   sp(c,a) np(d)
					sp = p->child[0];
					sp->key[1] = p->key[0];
					sp->data[1] = p->data[0];
					p->key[0] = p->key[1];
					p->data[0] = p->data[1];
					p->num = 1;
					sp->num = 2;
					np->key[0] = p->child[2]->key[0];
					np->data[0] = p->child[2]->data[0];
					delete p->child[2];
					p->child[2] = NULL;
				}
			}
			else {
				sp = p->child[1];
				if (sp->num ==2) {
			//      p(a,b)             p(a,d)
			//    /    |    \    -->   /  \   \
			//   D   sp(c,d) np(X)   D   sp(c) np(b)
					np->key[0] = p->key[1];
					np->data[0] = p->data[1];
					p->key[1] = sp->key[1];
					p->data[1] = sp->data[1];
					sp->num = 1;
					np->num = 1;
				} else {
			//      p(a,b)           p(a)
			//    /     \   \  -->  /    \
 			//  D     sp(c)  np(X) D     sp(c,b)
					sp->key[1] = p->key[1];
					sp->data[1] = p->data[1];
					sp->num = 2;
					p->num = 1;
					delete np;
					p->child[2] = NULL;
				}
			}
		}
	} else if (np->num == 0 && np->isleaf == 0) {
		if ( p->num == 1) {
			if (p->child[0] == np) {
				sp = p->child[1];
				if (sp->num == 2) {
			//       p(a)                          p(b)
			//     /     \                       /        \
			//   np(X)   sp(b,c)       ---->   np(a)      sp(c)
			//   |        /  \   \           /    \       /   \
			//   A        B  C    D          A    B      C     D
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					np->child[1] = sp->child[0];
					np->child[1]->parent = np;
					np->num = 1;
					p->key[0] = sp->key[0];
					p->data[0] = sp->data[0];
					sp->key[0] = sp->key[1];
					sp->data[0] = sp->data[1];
					sp->child[0] = sp->child[1];
					sp->child[1] = sp->child[2];
					sp->child[2] = NULL;
					sp->num = 1;
				} else {
			//       p(a)                       p(X)
			//     /     \                     /    \
			//   np(X)   sp(b)     --->     np(a,b) sp(--)
			//    |       / \              /   \   \
			//    A       B  C            A    B    C
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					np->key[1] = sp->key[0];
					np->data[1] = sp->data[0];
					np->child[1] = sp->child[0];
					np->child[2] = sp->child[1];
					np->child[1]->parent = np;
					np->child[2]->parent = np;
					np->num = 2;
					sp->child[0] = NULL;
					sp->child[1] = NULL;
					sp->num = 0;
					p->child[1] = NULL;
					delete sp;
					sp = NULL;
					p->num = 0;
					fixup( p );
				}
			} else if (p->child[1] == np){
				sp = p->child[0];
				if (sp->num == 2) {
		//      p(a)                     p(c)
		//     /    \                   /    \
		//  sp(b,c)   np(X)  --->   sp(b)    np(a)
		//  /   \  \    /          /   \     /   \
		//  A   B   C  D          A    B     C    D
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					np->child[1] = np->child[0];
					np->child[0] = sp->child[2];
					sp->child[2]->parent = np;
					sp->child[2] = NULL;
					np->num = 1;
					p->key[0] = sp->key[1];
					p->data[0] = sp->data[1];
					sp->num = 1;
				} else if(sp->num == 1) {
		//      p(a)                   p(X)
		//    /     \       ---->    /      \
		//  sp(b)   np(X)         sp(b,a)   np(--)
		// /    \   /              /  \  \
		// A     B  C             A   B   C
					sp->key[1] = p->key[0];
					sp->data[1] = p->data[0];
					sp->child[2] = np->child[0];
					sp->child[2]->parent = sp;
					sp->num = 2;
					np->child[0] = NULL;
					np->num = 0;
					p->child[1] = NULL;
					delete np;
					np = NULL;
					p->num = 0;
					fixup( p );
				}

			}
		} else if (p->num == 2) {
			if (p->child[0] == np) {
				sp = p->child[1];
				if (sp->num == 2) {
			//                p(a, b)                       p(c,b)
			//               /    \    \         ---->      /   \    \
			//           np(X)    sp(c,d)                np(a)  sp(d)
			//           /         /  \  \                / \   / \
			//          A         B   C   D              A  B   C  D
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					np->num = 1;
					np->child[1] = sp->child[0];
					np->child[1]->parent = np;
					p->key[0] = sp->key[0];
					p->data[0] = sp->data[0];
					sp->key[0] = sp->key[1];
					sp->data[0] = sp->data[1];
					sp->child[0] = sp->child[1];
					sp->child[1] = sp->child[2];
					sp->child[2] = NULL;
					sp->num = 1;
				} else if (sp->num == 1) {
			//                p(a, b)                       p(b)
			//               /    \   \         ---->      /    \
			//           np(X)   sp(c)                np(a,c)  sub
			//           /         /  \                 / \  \
			//          A         B   C                A  B   C
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					np->key[1] = sp->key[0];
					np->data[1] = sp->data[0];
					np->child[1] = sp->child[0];
					np->child[2] = sp->child[1];
					np->child[1]->parent = np;
					np->child[2]->parent = np;
					np->num = 2;
					p->key[0] = p->key[1];
					p->data[0] = p->data[1];
					p->child[1] = p->child[2];
					p->child[2] = NULL;
					p->num = 1;
					delete sp;
					sp = NULL;
				}
			} else if (p->child[1] == np) {
				if (p->child[2]->num == 2) {
			//       p(a,b)                               p(a,c)
			//      /   \  \                            /    \   \
			//     s    np(X)  sp(c,d)      --->        s    np(b) sp(d)
			//           /     /   \  \                       /\   / \
			//          A     B    C   D                     A B   C D
					sp = p->child[2];
					np->key[0] = p->key[1];
					np->data[0] = p->data[1];
					p->key[1] = sp->key[0];
					p->data[1] = sp->data[0];
					sp->key[0] = sp->key[1];
					sp->data[0] = sp->data[1];
					np->child[1] = sp->child[0];
					np->child[1]->parent = np;
					sp->child[0] = sp->child[1];
					sp->child[1] = sp->child[2];
					sp->child[2] = NULL;
					np->num = 1;
					sp->num = 1;
				} else if (p->child[0]->num == 2) {
			//             p(a,b)                p(d,b)
			//             /  \   \             /   \  \
			//        sp(c,d)np(X) s  ---->   sp(c) np(a) s
			//         /  \ \   /              / \  /  \
			//        A   B  C  D             A  B  C   D
					np->key[0] = p->key[0];
					np->data[0] = p->data[0];
					p->key[0] = sp->key[1];
					p->data[0] = sp->data[0];
					sp->num = 1;
					np->child[1] = np->child[0];
					np->child[0] = sp->child[2];
					np->child[0]->parent = np;
					sp->child[2] = NULL;
					np->num = 1;
				} else {
			//          p(a ,b)                        p(b)
			//         /    \   \                     /    \
			//        sp(c) np(X) s  ---->         sp(c,a) s
			//        / \    /                      /  \  \
			//       A  B    C                    A    B   C
					sp->key[1] = p->key[0];
					sp->data[1] = p->data[0];
					p->key[0] = p->key[1];
					p->data[0] = p->data[1];
					p->child[1] = p->child[2];
					p->child[2] = NULL;
					p->num = 1;
					sp->child[2] = np->child[0];
					sp->child[2]->parent = sp;
					delete np;
					np = NULL;
				}
			} else if (p->child[2] == np) {
				sp = p->child[1];
				if (sp->num == 2) {
			//               p(a,b)                       p(a,d)
			//               /  \   \                    /   \   \
			//              s  sp(c,d)np(X)   ----->    s  sp(c) np(b)
			//                   / \ \  /                    / \  / \
			//                  A  B C  D                   A  B C   D
					np->key[0] = p->key[1];
					np->data[0] = p->data[1];
					p->key[1] = sp->key[1];
					p->data[1] = sp->data[1];
					np->child[1] = np->child[0];
					np->child[0] = sp->child[2];
					np->child[0]->parent = np;
					np->num = 1;
					sp->child[2] = NULL;
					sp->num = 1;
				} else if (sp->num == 1) {
			//                 p(a,b)                          p(a)
			//               /    \    \                       /  \     \
			//              s    sp(c) np(X)  ------>         s sp(c,b) np(--)
			//                  /  \    /                       /   \  \
			//                  A  B   C                        A   B  C
					sp->key[1] = p->key[1];
					sp->data[1] = p->data[1];
					sp->child[2] = np->child[0];
					sp->child[2]->parent = sp;
					sp->num = 2;
					np->child[0] = NULL;
					delete np;
					np = NULL;
					p->child[2] = NULL;
					p->num = 1;

					}
				}

			}
		}
		return;
	}


void Tree:: print_help(node*& t, int deep)
{
	if (!t) return;
	if (deep > 0) puts("");
	for (int i = 0; i < deep; i++) cout<<"   ";
	for (int i = 0; i < t->num; i ++) {
		printf("(%d,%d)  ",t->key[i], t->data[i]);
	}
	if (t->num == 1) {
		print_help(t->child[0], deep+1);
		print_help(t->child[1], deep+1);
	}
	else {
		print_help(t->child[0], deep+1);
		print_help(t->child[1], deep+1);
		print_help(t->child[2], deep+1);
	}
}

bool Tree:: deletekey( int key )
{
	node* p = searchNode(root, key);
	if (!p) return false;
	if (p->isleaf) {
		if (p->num == 2) {
			if (p->key[0] == key){
				p->key[0] = p->key[1];
				p->data[0] = p->data[1];
			}
			p->num = 1;
		} else {
			cout << "here" << p->isleaf << endl;
			p->num--;
			fixup( p );
		}
	} else if (p->num == 2) {
		node* lp = NULL;
		if (key == p->key[0]) {
			lp = searchLeaf(p->child[1], key);
			p->key[0] = lp->key[0];
			p->data[0] = lp->data[0];
		}
		else if (key == p->key[1]) {
			lp = searchLeaf(p->child[2], key);
			p->key[1] = lp->key[0];
			p->data[1] = lp->data[0];
		}
		lp->num --;
		if (lp->num == 0)
			fixup(lp);
		else {
			lp->key[0] = lp->key[1];
			lp->data[0] = lp->data[1];
		}
	} else {
		node* lp = NULL;
		lp = searchLeaf(p->child[1], key);
		p->key[0] = lp->key[0];
		p->data[0] = lp->data[0];
		lp->num --;
		if (lp->num == 0)
			fixup(lp);
		else{
			lp->key[0] = lp->key[1];
   		    lp->data[0] = lp->data[1];
		}
	}
	return true;
}

//text
int main()
{
	Tree tree;
	int n;
	//cin >> n;
	n = 10;
	for (int i = 1; i <= n; i++)
	{
		tree.insertkey(i, 0);
	}
	tree.deletekey( 2 );
	tree.deletekey( 10 );
	tree.deletekey( 8 );
	tree.deletekey( 5 );
	tree.deletekey( 7 );
	cout << "Print tree:" << endl;
	tree.print();
	
	return 0;
}
