#ifndef _SM0_
#define _SMO_

#include <vector>
#include <string>
#include <set>
#include <map>

using std::vector;
using std::string;

class SMO{
public:
	SMO(string train_data="ʵ���˹�����");
	void train();
	void test();
	int classify(vector<double> &input);
	void readData(string fileName);
private:
	double tol;	//toleranceΥ��KKT�����ľ�ȷ��
	int dim;	//�Ա���x��ά��
	int N;		//��������
	double C;	//�ͷ�����
	double eta;	//����a2ʱ�õ���eta=K11+K22-2K12
	vector<double> alpha;	//�������ճ���
	double threshold;	//��ֵb
	vector< vector<double> > x;	//�������
	vector<int> y;				//�����
	vector<double> error;		//Ԥ��ֵ��ʵ��ֵ֮��
	std::set<int> bound_id;	//�߽���id,����Ϊ0��Cʱ,���ڱ߽��ϡ��Ǳ߽�㣺0<��<C��ʵ���ϴ󲿷�����aΪ0����Ҫ������
	int select_alpha2(int);
	double kernel(vector<double> &v1,vector<double> &v2);
	int update(int x1,int x2);
	bool satisfy_KKT(int id);
	double get_error(int x);
};

#endif