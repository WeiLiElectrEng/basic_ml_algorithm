/**
用遗传算法解决旅行商问题
博客地址：http://www.blogbus.com/myjuno-logs/237740875.html
**/

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
#include "utility.h"
#include "test.h"
#include <windows.h>

using namespace std;

class TSP{
public:
	TSP(const vector< vector<double> > &dist,int s,int ml=10000,int p=40,double p1=0.95,double p2=0.1);
	void train(const vector< vector<double> > &dist);
	void get_fitness(const vector< vector<double> > &);
	vector<double> get_path();
private:
	int start;
	int max_loop;
	int generation;
	int city_num;		//城市总个数
	int pop_num;		//种群大小(解决方案（路径）的个数)
	vector< vector<int> > population;		//种群
	vector<double> fitness;						//个体适应度
	vector<double> prob;						//个体被选中产生下一代的概率
	double best_value;
	vector<int> best_path;
	double pc;			//交叉率
	double pm;			//变异率
	int select_path();
	void mutation(vector<int> &vec);
	void crossover(vector<int> &vec1,vector<int> &vec2);
	
};


TSP::TSP(const vector< vector<double> > &dist,int s,int ml,int p,double p1,double p2)
	:max_loop(ml),start(s),city_num(dist.size()),pop_num(p),pc(p1),pm(p2){
	best_value=DBL_MAX;
	for(int i=0;i!=pop_num;++i) fitness.push_back(0);
	for(int i=0;i!=pop_num;++i) prob.push_back(0);
	for(int i=0;i!=city_num-1;++i) best_path.push_back(0);
	srand((unsigned)time(NULL));
	train(dist);
}
void TSP::train(const vector< vector<double> > &dist){
	//随机生成初始种群: 1~city_num 如果id等于start则直接跳过
	for(int i=0;i<pop_num;++i){
		vector<int> vec;	
		for(int j=0;j<city_num;){ 
			if(j!=(start-1)){
				vec.push_back(j+1);
			}
			++j;
		}
		std::random_shuffle(vec.begin(),vec.end());
		population.push_back(vec);
	}
	for(int loop=0;loop!=this->max_loop;++loop){
		if(loop%100==0) cout<<loop<<endl;
		//计算个体适应度
		get_fitness(dist);
		vector< vector<int> > temp;	//构造临时种群
		int i=0;
		while(true){
			//选择
			int p1=select_path();
			int p2(p1);		//选择两条不同的路径
			while(p2==p1)  p2=select_path();
			vector<int> vec1=population.at(p1);
			vector<int> vec2=population.at(p2);
			//交叉 
			if(rand()/(double)RAND_MAX<pc) crossover(vec1,vec2); 
			//变异
			if(rand()/(double)RAND_MAX<pm)	mutation(vec1);	
			if(rand()/(double)RAND_MAX<pm)	mutation(vec2);
			
			temp.push_back(vec1);	if(temp.size()==population.size()) break;
			temp.push_back(vec2);   if(temp.size()==population.size()) break;
		}
		population=temp;
	}
	cout<<"最优路径:(起始节点为：)"<<start<<endl<<"中间节点为： ";
	print_vector(best_path);
	cout<<"最优值为"<<best_value<<endl;
	cout<<start<<" "<<best_path.at(0)<<" "<<dist[start-1][best_path.at(0)-1]<<endl;
	for(int i=0;i<best_path.size()-1;++i){
		cout<<best_path.at(i)<<" "<<best_path.at(i+1)<<" "<<dist[best_path.at(i)-1][best_path.at(i+1)-1]<<endl;
	}
	cout<<best_path.at(best_path.size()-1)<<" "<<start<<" "<<dist[best_path.at(best_path.size()-1)-1][start-1]<<endl;
}
//适应度函数： n/length(Path) 节点总数除以路径长度
//同时根据个体适应度计算被选中产生下一代的概率
//同时记录下长度最小（适应度最高）的那条最优路线的id;
//正确性未测试
void TSP::get_fitness(const vector< vector<double> > &dist){
	//cout<<"个体适应度"<<endl;
	double total_fit=0;
	double bv=DBL_MAX;
	int bp=0;
	for(int i=0;i!=pop_num;++i){
		double length=0;
		length+=(dist[start-1][population[i][0]-1]+dist[population[i][city_num-2]-1][start-1]);
		for(int j=0;j!=city_num-2;++j) length+=dist[population[i][j]-1][population[i][j+1]-1];
		fitness.at(i)=(city_num/length);
		total_fit+=fitness.at(i);
		prob.at(i)=fitness.at(i);
		if(length<bv){ bv=length;bp=i;}
		if(length<best_value){ best_value=length; best_path=population.at(i); }
	}
	//cout<<"本种群最优个体为"<<bp<<" "<<"路径长度为： "<<bv<<endl;
	for(int i=0;i!=pop_num;++i){
		prob.at(i)/=total_fit;
	}

}

//轮盘赌方法选择个体
//要注意由于double变量的不确定性所有方案的概率之和相加可能略小于1（0.999999999999999XXX）
//而rand()/(double)RAND_MAX完全有可能大于这个值，所以(d1<=m)可能对于所有方案都不成立，因此
//此时最后的返回值应当返回最后一个（population.size()-1）
inline int TSP::select_path(){
	double d1=rand()/(double)RAND_MAX;
	double m=0;
	int i=0;
	for(;i!=population.size();++i){
		m+=prob.at(i);
		if(d1<=m) return i;
	}
	return population.size()-1;
}

void TSP::mutation(vector<int> &vec){
	int pos1=rand()%(city_num-1);
	int pos2(pos1);
	while(pos2==pos1) pos2=rand()%(city_num-1);
	int tmp=vec.at(pos1);
	vec.at(pos1)=vec.at(pos2);
	vec.at(pos2)=tmp;
}

void TSP::crossover(vector<int> &vec1,vector<int> &vec2){
	int pos=rand()%(city_num-2)+1;	//单点交叉,pos不应该取首尾位置，因为那样等于是没有交叉
	//需要检查交叉后是否合法:vec2在pos之后出现的节点要和vec1在pos之后出现的节点相同，仅仅是顺序不同。
	for(vector<int>::iterator itr=vec1.begin()+pos;itr!=vec1.end();++itr){
		if(count(vec2.begin()+pos,vec2.end(),*itr)!=1)  return; 
	}
	vector<int> tmp(vec2.begin(),vec2.end());
	copy(vec1.begin()+pos,vec1.end(),vec2.begin()+pos);
	copy(tmp.begin()+pos,tmp.end(),vec1.begin()+pos);
	//cout<<endl;
}


//用来从文件中读取数据的
void read_data(string filename,int num,vector< vector<double> > &dist){
	ifstream in(filename);
	double d;
	for(int i=0;i!=num;++i){
		vector<double> vec(num);
		for(int j=0;j!=num;++j){
			in>>d;
			vec.at(j)=d;
		}
		dist.at(i)=vec;
	}
}

int main(){
	 system("mode con:cols=100 lines=1000");  //调整控制台输出行数 
	//读取数据
	string filename="data.txt";
	int num=30;
	int start=1;
	cout<<"请输入节点个数"<<endl;
	cin>>num;cin.get();
	cout<<"数据文件名"<<endl;
	getline(cin,filename);
	cout<<"请输入起点城市（也是终点城市）"<<endl;
	cin>>start;/**/
	
	vector< vector<double> > dist(num);
	read_data(filename,num,dist);
	
	TSP model(dist,start);

	system("pause");
	return 0;
}

//生成人工数据的代码
//void  generator_data(){
//	int num=30;
//	ofstream out("data.txt");
//	for(int i=0;i<num;++i){
//		for(int j=0;j<num;++j){
//			out<<10*((double)rand()/RAND_MAX)<<"	";
//		}
//		out<<endl;
//	}
//	out.close();
//}