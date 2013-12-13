#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <cmath>
#include <ctime>
#include "SMO.h"

using namespace std;
double eps = 0.0000001;


double SMO::kernel(vector<double> &v1,vector<double> &v2){
	//��˹��
	//����Ҳ�֪����ôȷ��,����ٶ�Ϊ1
	/*double result(0);
	for(int i=0;i<dim;++i){
		result+=pow(v1[i]-v2[i],2);
	}
	return exp(result/-2);*/
	//����ʽ��
	double result(0);
	for(int i=0;i<dim;++i){
		result+=(v1[i]*v2[i]);
	}
	result +=1 ;
	return std::pow(result,2);
}

SMO::SMO(string train_data):threshold(0),tol(0.001),C(2){
	readData(train_data);
	dim = x.at(0).size();
	N = y.size();
	alpha.assign(N,0);
	error.assign(N,0);
	//����Ҫ��ʼ��a����ʼʱ��a��Ϊ0��ȫ���Ǳ߽��
	//��ʼ��error
	for(int i=0;i<N;++i){
		error[i]+=(threshold-y[i]);
		for(int j=0;j<N;++j){
			if(alpha[j]) error[i] += (alpha[i]*alpha[j]*kernel(x[i],x[j]));	//aj����0ʱ������
		}
	}
	srand((unsigned)time(NULL));
}

bool SMO::satisfy_KKT(int id){
	double r = y[id]*error[id];
	if( (alpha[id]<C && r<-tol) || (alpha[id]>0 && r>tol) ) return false;
	return true;
}

inline double max(double a,double b){ return a>b?a:b;}
inline double min(double a,double b){ return a<b?a:b;}

int SMO::update(int x1,int x2){
	double low;
	double high;
	double error1 = get_error(x1);
	double error2 = get_error(x2);
	if(y[x1]==y[x2]){
		low = max(0,alpha[x1]+alpha[x2]-C);
		high = min(C,alpha[x2]+alpha[x1]);
	}else{
		low = max(0,alpha[x2]-alpha[x1]);
		high = min(C,C+alpha[x2]-alpha[x1]);
	}
	double new_alpha2 = alpha[x2]+y[x2]*(error1-error2)/eta;
	double new_alpha1;
	if(new_alpha2>high) new_alpha2 = high;
	else if(new_alpha2<low) new_alpha2 = low;
	//�仯̫С�Ļ�������û�з����仯
	if(abs(new_alpha2-alpha[x2])<eps) 
		return 0;
	new_alpha1 = alpha[x1] +y[x1]*y[x2]*(alpha[x2]-new_alpha2);
	//ע�⣬����Ҫ��alpha1Ҳ�������½���޶�����Ϊalpha2�Ѿ��޶����ˣ���alpha2��alpha1�ĺ��Ƕ�ֵ��
	//��������double����������ܳ��ַǳ��ӽ�0�ĸ���
	if(new_alpha1<0 && new_alpha1>-eps){
		new_alpha1 = 0;	
	}
	//������ֵb
	double b1 = -error1-y[x1]*kernel(x[x1],x[x1])*(new_alpha1-alpha[x1])-y[x2]*kernel(x[x2],x[x1])*(new_alpha2-alpha[x2])+threshold;
	if(alpha[x1]>0 && alpha[x1]<C) {
		threshold = b1;
	}else{
		double b2 = -error2-y[x1]*kernel(x[x1],x[x2])*(new_alpha1-alpha[x1])-y[x2]*kernel(x[x2],x[x2])*(new_alpha2-alpha[x2])+threshold;
		if(alpha[x2]>0 && alpha[x2]<C)  threshold = b2;
		else	threshold = 0.5*(b1+b2);
	}
	alpha[x2] = new_alpha2;
	alpha[x1] = new_alpha1;
	return 1;
}

double SMO::get_error(int id){
	double rst = threshold;
	for(int i=0;i<N;++i){
		if(alpha[i]!=0){
			rst += (alpha[i]*y[i]*kernel(x[i],x[id]));
		}
	}
	return rst-y[id];
}

int SMO::select_alpha2(int x1){
	double max_dev = -1;		//ѡ����|E1-E2|����a2
	double dev = 0;
	int x2 = -1;
	//���ȴӷǱ߽��Ѱ��
	for(int i=0;i<N;++i){
		if(alpha[i]==0 || alpha[i]==C) continue;
		dev = abs(get_error(x1)-get_error(i));
		if(dev>max_dev){
			max_dev = dev;
			x2 = i;
		}
	}
	if(x2!=-1){
		eta = kernel(x[x1],x[x1]) +kernel(x[x2],x[x2]) - 2*kernel(x[x1],x[x2]);
		if(eta) return x2;			//����eta��0
	}
	//����Ҳ�������ʼ��non_bound�������ѡһ��eta����0��a2
	int new_begin = rand()%N;
	if(new_begin==N-1) new_begin = 0;
	int new_end = new_begin++;
	while(new_begin!=new_end){
		x2 = new_begin;
		if(new_begin==N-1) new_begin=0;
		else			  ++new_begin;
		if(alpha[x2]==0 || alpha[x2]==C || x2==x1) continue;
		eta = kernel(x[x1],x[x1]) +kernel(x[x2],x[x2]) - 2*kernel(x[x1],x[x2]);
		if(eta) return x2;
	}
	//������Ҳ������������㼯��Ѱ��
	new_begin = rand()%N;
	if(new_begin==N-1) new_begin = 0;
	new_end = new_begin++;
	while(new_begin!=new_end){
		x2 = new_begin;
		if(new_begin==N-1) new_begin=0;
		else			  ++new_begin;
		eta = kernel(x[x1],x[x1]) +kernel(x[x2],x[x2]) - 2*kernel(x[x1],x[x2]);
		if(eta) return x2;
	}
	//ע������������û�д������ĵ�һ�������һ������˳��ѡȡ�����������ѡ��ʼλ�ã�
	//����Ϊ�˷�ֹJhon C.Platt��������ν������ѡȡ��ʼ�ü��������Ӷ�����ƫ�����
	//�����˹����ɵļ�ģ�ͣ�ֱ�������µ�˳��ѡȡ׼ȷ��ûɶ�仯��
	/*for(int x2=0;x2<N;++x2){
		if(alpha[x2]==0 || alpha[x2]==C || x2==x1) continue;
		eta = kernel(x[x1],x[x1]) +kernel(x[x2],x[x2]) - 2*kernel(x[x1],x[x2]);
		if(eta) return x2;
	}
	for(int x2=0;x2<N;++x2){
		if(x2==x1) continue;
		eta = kernel(x[x1],x[x1]) +kernel(x[x2],x[x2]) - 2*kernel(x[x1],x[x2]);
		if(eta) return x2;
	}*/
	return -1;
}

void SMO::train(){
	bool loop_on_bounds(false);
	int num_changed;		//û��ֱ����num_violate_KKT����Ϊ�п�����ȻΥ����KKT������Υ���̶Ⱥ�С��a1 a2�ı仯�ǳ�ϸ΢��
	while(true){
		num_changed = 0;
		for(int i=0;i<N;++i){
			if((alpha[i]==0 || alpha[i]==C) && loop_on_bounds) continue;	//ֻ�ڱ߽�����Ѱ��a
			if(!satisfy_KKT(i)){
				int x2 = select_alpha2(i);
				if(x2==-1) continue;
				num_changed+=update(i,x2);
			}
		}
		if(!loop_on_bounds && num_changed==0) break;	//���������ݼ��϶��Ҳ���Υ��KKT������a�������㷨
		if(num_changed) loop_on_bounds=true;			//���������ݼ��ϱ���һ�κ����̻ص��߽���ϱ�����ֱ���߽�㶼����KKT
		else			loop_on_bounds=false;
		//������ֵ
		/*double er=0;	
		for(int i=0;i<N;++i){
			er+=abs(get_error(i));
		}
		cout<<er<<endl;*/
	}
}

int SMO::classify(vector<double> &input){
	double rst = threshold;
	for(int i=0;i<N;++i){
		if(alpha[i]==0) continue;
		rst += alpha[i]*y[i]*kernel(x[i],input);
	}
	return (rst<0)?-1:1;
}

void gen_data(vector< vector<double> > &x,vector<int> &y,int MAX_NUM){
	double x1 = 0;	double x2 = 0;	double x3 = 0;
	int num = 0;
	double eps = 1e-5;
	while(num<MAX_NUM){
		x1 = static_cast<double>(rand())/RAND_MAX;
		x2 = static_cast<double>(rand())/RAND_MAX;
		x3 = static_cast<double>(rand())/RAND_MAX;
		if(x1<0.01 || x2<0.01 || x3<0.01) continue;
		double v = x1*x1+x2*x2+x3*x3;
		if(abs(v-1)<eps) continue;
		else if(v>1) y.push_back(1);
		else		y.push_back(-1);
		vector<double> vec;vec.push_back(x1);vec.push_back(x2);vec.push_back(x3);
		x.push_back(vec);
		++num;
	}
}

void SMO::readData(string fileName){
	cout<<"ֱ�ӹ����˹�����"<<endl;
	const int TRAIN_NUM = 100;
	y.reserve(TRAIN_NUM);
	x.reserve(TRAIN_NUM);
	gen_data(x,y,TRAIN_NUM);
	return;
	//��ͬ���ı���ʽ���ò�ͬ�Ķ�ȡ����,ֻҪ�����������ȡ��x,��������뵽y����
	//ifstream fin(fileName);
	//string label;
	//double var;
	//while(fin>>var){
	//	vector<double> vec;
	//	vec.push_back(var);
	//	for(int i=0;i<31;++i){
	//		fin.get();
	//		fin>>var;
	//		vec.push_back(var);
	//	}
	//	fin.get();
	//	fin>>label;
	//	y.push_back((label=="Active")?1:(-1));
	//	x.push_back(vec);
	//}
	////�����ݱ�׼����0~1֮�䡣
	//double max[32];
	//double min[32];
	//fill(max,max+32,-1*(1<<10));
	//fill(min,min+32,1<<10);
	//for(size_t i=0;i<x.size();++i){
	//	for(size_t j=0;j<32;++j){
	//		if(max[j]<x[i][j]) max[j] = x[i][j];
	//		if(min[j]>x[i][j]) min[j] = x[i][j];
	//	}
	//}
	//double dev[32];
	//for(int i=0;i<32;++i) dev[i] = max[i]-min[i];
	//for(size_t i=0;i<x.size();++i){
	//	for(size_t j=0;j<32;++j){
	//		x[i][j] = (x[i][j]-min[j])/dev[j];
	//	}
	//}
}

void SMO::test(){
	const int TEST_NUM = 100;
	vector<int> y2;
	vector< vector<double> > x2;
	x2.reserve(TEST_NUM);
	y2.reserve(TEST_NUM);
	gen_data(x2,y2,TEST_NUM);
	int error_num = 0;
	for(int i=0;i<TEST_NUM;++i){
		int predict = classify(x2[i]);
		if( y2[i] != predict) error_num++;
		//���Կ��������ǵ���0.99~1.01֮��ĵ�ᱻ����
		/*if( y[i] != predict){
			double rst = pow(x2[i][0],2)+pow(x2[i][1],2)+pow(x2[i][2],2);
			cout<<rst<<endl;
		}*/
	}
	cout<<"�������"<<error_num<<" ��������"<<
		TEST_NUM<<" ������:"<<double(error_num)/TEST_NUM<<endl;
}