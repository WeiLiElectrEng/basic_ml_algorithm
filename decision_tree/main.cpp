/**
address: http://www.blogbus.com/myjuno-logs/239066336.html
**/
#include "C45.h"
#include <vector>
#include <iostream>
#include <string>
#include "C45.h"

using namespace std;


int main(){

	C45 model("data.txt");

	model.build_classify();
	model.display();
	
	//Ҳ���Զ�ȡ�ı��ļ�Ȼ�����attr_table��label_table���ֶ��Զ�ת����int,
	vector<int> vec;
	vec.push_back(1);vec.push_back(1);vec.push_back(2);vec.push_back(0);
	cout<<"�������ࣺ "<<model.classify(vec)<<endl;

	return 0;
}
