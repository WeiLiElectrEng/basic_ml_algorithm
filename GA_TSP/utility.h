#include <iostream>
#include <vector>
using namespace std;
#ifndef MY_UTILITY
#define MY_UTILITY

template<class T>
void print_vector(vector<T> &vec){
	for(vector<T>::iterator itr=vec.begin();itr!=vec.end();++itr){
		cout<<*itr<<" ";
	}
	cout<<endl;
}
#endif