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
	
	//也可以读取文本文件然后根据attr_table和label_table将字段自动转化成int,
	vector<int> vec;
	vec.push_back(1);vec.push_back(1);vec.push_back(2);vec.push_back(0);
	cout<<"所属分类： "<<model.classify(vec)<<endl;

	return 0;
}
