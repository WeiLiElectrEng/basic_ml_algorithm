#include "Tree.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;


Tree* Tree::build_tree(vector< int > record_ids,vector<int> attr_remain,vector< vector<int> > *records,vector< int > *labels){
	Tree *father = new Tree;
	//把公共信息复制下来
	father->attr_table = this->attr_table;
	father->label_table = this->label_table;
	if(record_ids.size()<=1){		//只剩下1个样本
		father->is_leaf=true;
		father->class_label = labels->at(record_ids.at(0));
		cout<<"只剩下1个样本"<<labels->at(record_ids.at(0))<<endl;
		return father;
	}
	if(attr_remain.empty()){		//已经没有剩余的属性来切分样本
		father->is_leaf = true;
		father->class_label = get_frequent(record_ids,labels);
		cout<<"已经没有剩余的属性来切分样本"<<father->class_label<<endl;
		return father;
	}
	if(same_classes(record_ids,labels)){	//所有样本的类别标号相同
		father->is_leaf = true;
		father->class_label = labels->at(record_ids.at(0));
		cout<<"所有样本的类别标号相同"<<father->class_label<<endl;
		cout<<"样本id为： ";
		for(int i=0;i<record_ids.size();++i){
			cout<<record_ids.at(i)<<" ";
		}
		cout<<endl;
		return father;		
	}
	//选择最佳属性切分样本集
	father->split_attr = get_split_attr(record_ids,attr_remain,records,labels);
	vector< vector<int> > sub_set;
	divede_dataset(record_ids,father->split_attr,sub_set,records);
	vector<int> new_attr_remain;
	for(vector<int>::iterator itr=attr_remain.begin();itr!=attr_remain.end();++itr) {
		if(*itr!=father->split_attr) new_attr_remain.push_back(*itr);
	}
	for(int i=0;i<sub_set.size();++i){
		if(sub_set.at(i).empty()){
			father->sub_trees.push_back(NULL);
			continue;
		}
		father->sub_trees.push_back(build_tree(sub_set.at(i),new_attr_remain,records,labels));
	}
	return father;
}

int Tree::get_frequent(vector<int> &record_ids,vector< int > *labels){
	vector<int> vec;
	for(int i=0;i<record_ids.size();++i){
		int lbl = labels->at(record_ids.at(i));
		if(vec.size()<lbl+1) vec.resize(lbl+1);
		++vec.at(lbl-1);
	}
	return *max_element(vec.begin(),vec.end());
}

//遍历每个属性，计算信息增益和信息增益比，选择增益比最大的一个属性作为分割样本集的属性
int  Tree::get_split_attr(vector<int> &record_ids,vector<int> &attr_remain,vector< vector<int> > *records,vector< int > *labels){
	if(attr_remain.size()==1) return attr_remain.at(0);
	//计算样本切分前各类别的分布情况
	double E_D = entropy(record_ids,labels);	//entropy(D)
	double MAX_GR = -DBL_MAX;			//最大的增益比
	int best_attr = -1;					//最大的增益比对于的属性id
	for(int i=0;i<attr_remain.size();++i){
		int id = attr_remain.at(i);
		vector< vector<int> > sub_set;
		divede_dataset(record_ids,id,sub_set,records);		//根据属性划分子集
		//vector<double> E_D_IS;			//entropy(Di)
		double E_D_IS=0.0;						//各个自己的entropy之和
		double split_info(0.0);
		for(int j=0;j!=sub_set.size();++j){
			if(sub_set.at(j).empty()){
				cout<<"该sub_set为空"<<endl;
				continue;
			}else if(sub_set.at(j).size()==record_ids.size()){
				cout<<"该recored_ids全部属于同一类"<<endl;
				return -1;
			}
			double d = sub_set.at(j).size()/(double)record_ids.size();
			E_D_IS+=d*entropy(sub_set.at(j),labels);
			split_info-=(d*log(d)/log(2.0));
			cout<<d<<" "<<split_info<<endl;
		}
		if(split_info<0.000000001) 
			cout<<"error"<<endl;
		double d = (E_D-E_D_IS)/split_info;
		if(d>MAX_GR){
			MAX_GR = d;
			best_attr = id;
		}
		cout<<"第"<<i<<"个属性的信息增益比为："<<d<<endl;
	}
	return best_attr;
}

double Tree::entropy(vector<int> &record_ids,vector< int > *labels){
	double e=0.0;
	vector<double> dist(this->label_table->size());
	for(int i=0;i<record_ids.size();++i){
		int id = record_ids.at(i);
		int lbl_id = labels->at(id);
		dist[lbl_id]++;
	}
	for(int i=0;i<dist.size();i++){
		dist[i]/=record_ids.size();
	}
	for(int i=0;i<dist.size();i++){
		if(dist[i]<0.00000001) continue;	//此样本集中不含该类别的样本
		e-=(dist[i]*log(dist[i])/log(2.0));
	}
	return e;
}


void Tree::divede_dataset(vector<int> &record_ids,int attr_id,vector< vector<int> > &sub_set,vector< vector<int> > *records){		//根据属性的取值划分数据集
	for(int i=0;i<this->attr_table->at(attr_id).size();++i){
		vector<int> vec;
		sub_set.push_back(vec);
	}
	for( int i=0;i<record_ids.size();++i){
		//判断record属于哪个subset
		int id = record_ids.at(i);
		int val = records->at(id).at(attr_id);
		sub_set.at(val).push_back(id);
	}
}

int Tree::classify_record(Tree *tree,vector<int> &record){
	if(tree->is_leaf) return tree->class_label;
	//找到该record对应的分支
	int which_sub = record.at(tree->split_attr);
	return classify_record(tree->sub_trees.at(which_sub),record);
}

bool Tree::same_classes(vector<int> &record_ids,vector< int > *labels){
	int lbl(labels->at(record_ids.at(0)));
	for(int i=1;i<record_ids.size();++i){
		if(labels->at(record_ids.at(i)) !=lbl ) return false;
	}
	return true;
}