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
	SMO(string train_data="实验人工数据");
	void train();
	void test();
	int classify(vector<double> &input);
	void readData(string fileName);
private:
	double tol;	//tolerance违反KKT条件的精确度
	int dim;	//自变量x的维度
	int N;		//样本个数
	double C;	//惩罚因子
	double eta;	//更新a2时用到的eta=K11+K22-2K12
	vector<double> alpha;	//拉格朗日乘子
	double threshold;	//阈值b
	vector< vector<double> > x;	//输入变量
	vector<int> y;				//类别标号
	vector<double> error;		//预测值与实际值之差
	std::set<int> bound_id;	//边界点的id,当α为0或C时,即在边界上。非边界点：0<α<C（实际上大部分样本a为0不需要调整）
	int select_alpha2(int);
	double kernel(vector<double> &v1,vector<double> &v2);
	int update(int x1,int x2);
	bool satisfy_KKT(int id);
	double get_error(int x);
};

#endif