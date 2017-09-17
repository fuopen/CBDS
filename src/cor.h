#ifndef _COR_H_
#define _COR_H_
#include "coalescent.h"
#include "macro.h"
//#include "fast_median.h"

class cor{
	private:
		//***********************************************************
		//***********************************************************
		//data segment
		//vector<string> store_trepos;
		//vector<string> store_time_intv;
		//FLOAT** store_time_intv;
		STRING* store_trepos;
		STRING* tfs;
		STRING* ntf;
		FLOAT** total_info;
		FLOAT alp_median;
		FLOAT* ratio_median;

		double inpop_size;
		vector<string> store_data;
		
		int nt;
		int ns;
		//double alp_median;
		//double*  ratio_median;
		//double** total_info;
		int n_h1;
		psmc** my_psmc;	
		ofstream ofs;
		//enum dealtypes{NORM,SINGAL,NONE};
		//***********************************************************
		//***********************************************************
		//function segment
		void read_pop_size(const char*);
		void read_col_time(const char*);
		void read_alp_scale(const char*);
		void cor_ini();
		void each_cor();//for double_cor
		void all_cor(); //for double_cor
		void s_each_cor();//for single_cor
		void s_all_cor();//for single_cor
		void single_cor();
		void double_cor();
		void rec_tree();//reconstruct UPGMA tree interval
	public:
		cor(const char*,const char*);
		cor(const char*,const char*,const char*);
		~cor();
		void print(const char*,bool if_output_ratio=false);
		int get_nh1()const{return n_h1;}
};
#endif
