#include <algorithm>
#include <set>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include "SMO.h"

using namespace std;


int main(){
	//可以将数据放进a.txt中，然后自己重写SMO的	void readData(string fileName);
	//本实验没有找到合适的数据集，自己人工生成了训练样本(输入变量三维x,y,z)：
	//0.01~1之间的随机数，且当x^2+y^2+z^2>1时属于类别1，否则属于类别-1(以球面为分割平面)
	SMO model;
	model.train();
	model.test();	//训练个数100 测试个数100 错误预测个数5，准确率97%
	return 0;
}
