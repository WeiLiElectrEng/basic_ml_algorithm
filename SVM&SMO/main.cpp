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
	//���Խ����ݷŽ�a.txt�У�Ȼ���Լ���дSMO��	void readData(string fileName);
	//��ʵ��û���ҵ����ʵ����ݼ����Լ��˹�������ѵ������(���������άx,y,z)��
	//0.01~1֮�����������ҵ�x^2+y^2+z^2>1ʱ�������1�������������-1(������Ϊ�ָ�ƽ��)
	SMO model;
	model.train();
	model.test();	//ѵ������100 ���Ը���100 ����Ԥ�����5��׼ȷ��97%
	return 0;
}
