#ifndef _C45_CLASSIFIER_
#define _C45_CLASSIFIER_

#include "Tree.h"
#include <vector>
#include <string>
using namespace std;

class Tree;

class C45 : public Tree{
public:
	C45(string f_name);
	void build_classify();
	void display();
	int classify(vector<int> &record);
	
private:
	Tree *root;
	vector< vector<int> > *records;
	vector< int > *labels;
	void print(Tree *t);
};

#endif