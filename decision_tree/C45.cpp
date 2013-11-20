#include "C45.h"
#include "Tree.h"
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Classify_Tree;

C45::C45(string f_name){
	attr_table = new vector< map<string,int> >;
	label_table = new map<string,int>;
	records = new vector< vector<int> >;
	labels = new vector< int >;

	ifstream in(f_name);
	string word;
	while(true){
		in>>word;
		map<string,int> m;
		int n,i=0;
		in>>n;
		string tmp;
		while(n--){
			in>>tmp;
			m.insert(make_pair(tmp,i++));
		}
		if(word=="@attribute") attr_table->push_back(m);
		if(word=="@class") {
			*label_table=m;
			break;
		}
	}
	in>>word;
	int n;
	in>>n;
	while(n--){
		vector<int> vec;
		for(int i=0;i<attr_table->size();++i){
			in>>word;
			vec.push_back(attr_table->at(i)[word]);
		}
		
		this->records->push_back(vec);
		in>>word;
		this->labels->push_back((*(this->label_table))[word]);
	}
}

void C45::build_classify(){
	vector<int> record_ids;
	vector<int> label_ids;
	vector<int> attr_remain;
	for(int i=0;i<this->records->size();++i) record_ids.push_back(i);
	for(int i=0;i<this->attr_table->size();++i) attr_remain.push_back(i);
	root = build_tree(record_ids,attr_remain,this->records,this->labels);
	//训练数据在模型建成后可以删除
	delete this->records;
	delete this->labels;
}

int C45::classify(vector<int> &record){
	return classify_record(root,record);
}

void C45::display(){
	//打印树(深度优先)
	Tree *tree = root;
	print(tree);
}

void C45::print(Tree *t){
	//层次遍历
	queue<Tree*> q;
	q.push(root);
	while(!q.empty()){
		Tree *t = q.front();
		for(int i=0;i<t->get_sub_trees().size();++i) q.push(t->get_sub_trees().at(i));
		if(t->get_split_attr()<0) cout<<"叶节点，类标号为:"<<t->get_class_label()<<endl;
		else cout<<"非叶节点，切分属性为： "<<t->get_split_attr()<<" "<<endl;
		q.pop();
	}
}