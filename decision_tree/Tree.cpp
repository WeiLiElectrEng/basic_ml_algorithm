#include "Tree.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;


Tree* Tree::build_tree(vector< int > record_ids,vector<int> attr_remain,vector< vector<int> > *records,vector< int > *labels){
	Tree *father = new Tree;
	//�ѹ�����Ϣ��������
	father->attr_table = this->attr_table;
	father->label_table = this->label_table;
	if(record_ids.size()<=1){		//ֻʣ��1������
		father->is_leaf=true;
		father->class_label = labels->at(record_ids.at(0));
		cout<<"ֻʣ��1������"<<labels->at(record_ids.at(0))<<endl;
		return father;
	}
	if(attr_remain.empty()){		//�Ѿ�û��ʣ����������з�����
		father->is_leaf = true;
		father->class_label = get_frequent(record_ids,labels);
		cout<<"�Ѿ�û��ʣ����������з�����"<<father->class_label<<endl;
		return father;
	}
	if(same_classes(record_ids,labels)){	//�����������������ͬ
		father->is_leaf = true;
		father->class_label = labels->at(record_ids.at(0));
		cout<<"�����������������ͬ"<<father->class_label<<endl;
		cout<<"����idΪ�� ";
		for(int i=0;i<record_ids.size();++i){
			cout<<record_ids.at(i)<<" ";
		}
		cout<<endl;
		return father;		
	}
	//ѡ����������з�������
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

//����ÿ�����ԣ�������Ϣ�������Ϣ����ȣ�ѡ�����������һ��������Ϊ�ָ�������������
int  Tree::get_split_attr(vector<int> &record_ids,vector<int> &attr_remain,vector< vector<int> > *records,vector< int > *labels){
	if(attr_remain.size()==1) return attr_remain.at(0);
	//���������з�ǰ�����ķֲ����
	double E_D = entropy(record_ids,labels);	//entropy(D)
	double MAX_GR = -DBL_MAX;			//���������
	int best_attr = -1;					//��������ȶ��ڵ�����id
	for(int i=0;i<attr_remain.size();++i){
		int id = attr_remain.at(i);
		vector< vector<int> > sub_set;
		divede_dataset(record_ids,id,sub_set,records);		//�������Ի����Ӽ�
		//vector<double> E_D_IS;			//entropy(Di)
		double E_D_IS=0.0;						//�����Լ���entropy֮��
		double split_info(0.0);
		for(int j=0;j!=sub_set.size();++j){
			if(sub_set.at(j).empty()){
				cout<<"��sub_setΪ��"<<endl;
				continue;
			}else if(sub_set.at(j).size()==record_ids.size()){
				cout<<"��recored_idsȫ������ͬһ��"<<endl;
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
		cout<<"��"<<i<<"�����Ե���Ϣ�����Ϊ��"<<d<<endl;
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
		if(dist[i]<0.00000001) continue;	//���������в�������������
		e-=(dist[i]*log(dist[i])/log(2.0));
	}
	return e;
}


void Tree::divede_dataset(vector<int> &record_ids,int attr_id,vector< vector<int> > &sub_set,vector< vector<int> > *records){		//�������Ե�ȡֵ�������ݼ�
	for(int i=0;i<this->attr_table->at(attr_id).size();++i){
		vector<int> vec;
		sub_set.push_back(vec);
	}
	for( int i=0;i<record_ids.size();++i){
		//�ж�record�����ĸ�subset
		int id = record_ids.at(i);
		int val = records->at(id).at(attr_id);
		sub_set.at(val).push_back(id);
	}
}

int Tree::classify_record(Tree *tree,vector<int> &record){
	if(tree->is_leaf) return tree->class_label;
	//�ҵ���record��Ӧ�ķ�֧
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