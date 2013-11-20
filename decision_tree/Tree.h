#ifndef _Classify_Tree_
#define _Classify_Tree_

#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std;

class Tree{
public:
	Tree():is_leaf(false),class_label(-1){}
	Tree* build_tree(vector< int > record_ids,vector<int> attr_remain,vector< vector<int> > *records,vector< int > *labels);
	int classify_record(Tree *tree,vector<int> &record);
//protected:
	vector< map<string,int> > *attr_table;
	map<string,int> *label_table;
	bool is_leaf;
	int class_label;
	int split_attr;
	vector<Tree *> sub_trees;
	int get_frequent(vector<int> &record_ids,vector< int > *labels);
	int  get_split_attr(vector<int> &record_ids,vector<int> &attr_remain,vector< vector<int> > *records,vector< int > *labels);
	double entropy(vector<int> &record_ids,vector< int > *labels);
	void divede_dataset(vector<int> &record_ids,int attr_id,vector< vector<int> > &sub_set,vector< vector<int> > *records);
	bool same_classes(vector<int> &record_ids,vector< int > *labels);
	vector<Tree *> get_sub_trees(){return this->sub_trees;}
	int get_split_attr(){return this->split_attr;}
	int get_class_label(){return this->class_label;}
	void p(){cout<<"P"<<endl;}
};


#endif