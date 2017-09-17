#ifndef _COALESENT_H_
#define _COALESENT_H_
#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<vector>
#include<cmath>
#include<string>
#include<cctype>
using namespace std;

typedef struct swap_data{
	double value;
	unsigned int a_pos;
	unsigned int p_pos;
	bool type;
}sd;

void read_data(const char*, double&,vector<string>&);

class psmc{
	private:
		double inpop_size;
		int intv_size;
		int t_size;
		int coal_size;
		double* c_gen;
		double* lambda;
		double* t_gen;
		sd* total_time;
		unsigned* tau;
		//unsigned int* tau_bak;
		unsigned* t;
		double neu_minus_logp;
		double sel_minus_logp;
		double diff;
		double alpha;
		//***********
		double Alp1,Alp2,Alp3,alp3_minus_logp;
		unsigned change_time1,change_time2;
		//***********
	public:
		~psmc();
		void initiate(const vector<string>&);
		psmc(const double&,const vector<string>&,const double*,const int&);
		void insert_time(const double*,const int&);
		void calculate();
		void swap(sd&,sd&);
		void sort(sd*,unsigned int*,unsigned int*,const unsigned int&);
		double get_n_logp()const;
		double get_s_logp()const;
		void cal_stat();
		double get_diff()const;
		double get_alpha()const;
		void clear();
		void print_info();
		double* alphas;
		inline int get_coal_size()const{
			return coal_size;
		}
		void cal_alp3(double&,double&,double&,double&,const unsigned&,const unsigned&);
		void get_max_alp3();
		double get_Alp1()const{return Alp1;}
		double get_Alp2()const{return Alp2;}
		double get_Alp3()const{return Alp3;}
		double get_diff3()const{ return neu_minus_logp-alp3_minus_logp;}
		//double get_cht1(const double* t_time)const{return t_time[change_time1];}
		//double get_cht2(const double* t_time)const{return t_time[change_time2];}
		unsigned get_ansnum1()const{ return coal_size-change_time1+1;}
		unsigned get_ansnum2()const{ return coal_size-change_time2+1;}
		unsigned get_index1()const{ return change_time1;}
		unsigned get_index2()const{ return change_time2;}
		//void reset(const double*);
		void recover();
		void initialize();
		double* recover_tree;
		double* spop_time;
		double* recover_delta;
		double get_cht1()const{return recover_tree[change_time1];}
		double get_cht2()const{return recover_tree[change_time2];}
};

#endif
