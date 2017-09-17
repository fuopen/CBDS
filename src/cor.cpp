#include "cor.h"
#include "fast_median.h"
#include "macro.h"
#include "gen_func.h"
#include "file.h"
#include "tree.h"
#include "list.h"
#include "mean_sd.h"
#include "quantile.h"
#include "random.h"

#if defined H975
	#if	defined CHB
		//double alp_h1=6.717548;
		double alp_threshold=0.4642;
	#elif defined CEU
		//double alp_h1=6.49632;
		double alp_threshold=0.470899;
	#elif defined YRI
		//double alp_h1=3.312256;
		double alp_threshold=0.594968
	#else 
		//double alp_h1=6.5;
		double alp_threshold=0.470786
	#endif
#elif defined H99
	#if defined CHB
		//double alp_h1=8.601544;
		double alp_threshold=0.527;//0.413495;
	#elif defined CEU
		//double alp_h1=8.434554;
		double alp_threshold=0.528;//0.417589;
	#elif defined YRI
		//double alp_h1=4.425076;
		double alp_threshold=0.50;//0.544259;
	#elif defined CORE
		double alp_threshold=0.5012;
	#elif defined CONS
		double alp_threshold=0.539;
	#elif defined MALAY
		double alp_threshold=0.3779;
	#else
		//double alp_h1=6.5;
		double alp_threshold=0.470786;
	#endif
#else
	//double alp_h1=6.5;
	double alp_threshold=0.470786;
#endif	
	
//static double alp_threshold=0.7;

cor::cor(const char* input1,const char* input2):inpop_size(0.0),store_trepos(NULL),tfs(NULL),ntf(NULL),ns(0),nt(0),total_info(NULL),my_psmc(NULL),ratio_median(NULL),n_h1(0){
	read_col_time(input1);
	read_pop_size(input2);
	double_cor();
}			

cor::cor(const char* input1,const char* input2,const char* input3):inpop_size(0.0),ns(0),nt(0),store_trepos(NULL),tfs(NULL),ntf(NULL),total_info(NULL),my_psmc(NULL),ratio_median(NULL){
	read_col_time(input1);
	read_pop_size(input2);
	read_alp_scale(input3);
	single_cor();
}

cor::~cor(){
	if(ratio_median) free(ratio_median);
	if(total_info){
		for(int k=0;k!=nt;k++){
			free(total_info[k]);
		}
		free(total_info);
	}
	if(my_psmc){
		for(int k=0;k!=nt;k++){
			delete my_psmc[k];
		}
		delete my_psmc;
	}
	if(tfs||ntf||store_trepos){
		for(int i=0;i!=nt;i++){
			if(tfs[i])free(tfs[i]);
			if(ntf[i])free(ntf[i]);
			if(store_trepos)free(store_trepos[i]);
		}
		if(tfs)free(tfs);
		if(ntf)free(ntf);
		if(store_trepos)free(store_trepos);
	}
}

void cor::read_col_time(const char* input1){
	MakeTreeFormat(input1,&store_trepos,&tfs,&nt);
	total_info=(FLOAT**) malloc(sizeof(FLOAT*)*nt);
	for(int i=0;i!=nt;i++){
		STRING s=tfs[i];
		FLOAT* f;
		int n;
		TreeIntvs(s,&f,&n);
		total_info[i]=f;
		if(i==0) ns=n;
	}
}

void cor::read_pop_size(const char* input2){
	ifstream ifs(input2);
	string data_tmp;
	bool find_n0=false;
	bool ifn_store=true;

	if(ifs.is_open()){
		while(true){
			ifs>>data_tmp;
			if(!ifs.eof()){
				if(ifn_store){
					if(find_n0){
						inpop_size = atof(data_tmp.data());
						find_n0 = false;
						continue;
					}
					if(data_tmp.compare("N0")==0){
						find_n0=true;
						continue;
					}
					if(data_tmp.compare("lambda")==0){
						ifn_store=false;
						continue;
					}
				}
				else store_data.push_back(data_tmp);
			}
			else break;
		}
	}
	else{
		cerr<<"Failed to open the pop_size file!"<<endl;
		exit(-1);
	}
	
	ifs.close();
}

void cor::read_alp_scale(const char* input3){
	ifstream ifs(input3);
	double restore_input;
	int count=0;
	ratio_median=(FLOAT*) malloc(sizeof(FLOAT)*ns);
	while(ifs>>restore_input){
		if(count==ns){
			alp_median=restore_input;
			break;
		}
		else{
			ratio_median[count++]=restore_input;
		}
	}
	ifs.close();
}	

void cor::cor_ini(){
	my_psmc = new psmc*[nt];
	for(int i=0;i!=nt;i++){
		for(int j=0;j<ns;j++){
			if(total_info[i][j]<0.001) total_info[i][j]=0.01;
		}
		
		FLOAT* tmp_info=(FLOAT*) malloc(sizeof(FLOAT)*ns);
		memcpy(tmp_info,total_info[i],sizeof(double)*ns);
		for(int j=0;j<ns-1;j++)
			tmp_info[j+1] += tmp_info[j];
		
		my_psmc[i] = new psmc(inpop_size,store_data,tmp_info,ns);
		free(tmp_info);
		memcpy(total_info[i],my_psmc[i]->alphas,sizeof(double)*ns);
	}
}

void cor::each_cor(){
	ratio_median=(FLOAT*) malloc(sizeof(FLOAT)*ns);
	for(int j=0;j<ns;j++){
		FLOAT* tmp=(FLOAT*) malloc(sizeof(FLOAT)*nt);
		for(int i=0;i!=nt;i++){
			tmp[i]=total_info[i][j];
		}
		double md = fast_median(tmp,nt);
		ratio_median[j]=md;
		for(int i=0;i!=nt;i++){
			total_info[i][j] *= log(2)/md;
		}
		free(tmp);
	}
	for(int i=0;i<nt;i++){
		memcpy(my_psmc[i]->alphas,total_info[i],sizeof(double)*ns);
		my_psmc[i]->recover();
	}
}

void cor::all_cor(){
	int n_l1=0,n_g1=nt-1;
	FLOAT* alp_info = new FLOAT[nt];
	int* tmp_info=new int[nt];
	for(int i=0;i<nt;i++){
		alp_info[i] = my_psmc[i]->get_alpha();
	}	
	FLOAT* alp_info_cp=new FLOAT[nt];
	memcpy(alp_info_cp,alp_info,sizeof(FLOAT)*nt);
	alp_median=fast_median(alp_info_cp,nt);
	delete[] alp_info_cp;
	for(int i=0;i<nt;i++){
		alp_info[i]<alp_median?tmp_info[n_l1++]=i:tmp_info[n_g1--]=i;
	}
#ifdef H21
	//printf("exec h21\n");
	int n_l2=0,n_g2=n_l1-1;
	int* tmp_info2=new int[n_l1];
	for(int i=0;i<n_l1;i++){
		alp_info[tmp_info[i]]<alp_threshold*alp_median?tmp_info2[n_l2++]=tmp_info[i]:tmp_info2[n_g2--]=tmp_info[i];
	}
	memcpy(tmp_info,tmp_info2,sizeof(int)*n_l1);
	n_h1=n_l2;
	delete[] tmp_info2;
#endif

	for(int j=0;j<ns;j++){
#if !defined H21
		//printf("exec h1\n");
		random_permuti(tmp_info,n_l1);
		random_permuti(tmp_info+n_l1,nt-n_l1);
		//exit(10);
#else
		//printf("exec h21\n");
		random_permuti(tmp_info,n_l2);
		random_permuti(tmp_info+n_l2,n_l1-n_l2);
		random_permuti(tmp_info+n_l1,nt-n_l1);
		//exit(10);
#endif
		for(int i=0;i<nt;i++){
			my_psmc[i]->alphas[j]=total_info[tmp_info[i]][j]/alp_median;
		}
	}
	for(int i=0;i<nt;i++) my_psmc[i]->recover();
	
	delete[] alp_info;
	delete[] tmp_info;
}

void cor::rec_tree(){
	ntf=(STRING*) malloc(sizeof(STRING)*nt);
	for(int i=0;i<nt;i++){
		memcpy(total_info[i],my_psmc[i]->recover_delta,sizeof(double)*ns);
		FLOAT* f=cumsum(total_info[i],ns);
		CorrectTree(tfs[i],&ntf[i],f,ns);
	}
}

void cor::double_cor(){
	cor_ini();
	each_cor();
	all_cor();
	rec_tree();
}

void cor::s_each_cor(){
	for(int j=0;j<ns;j++){
		for(int i=0;i!=nt;i++){
			total_info[i][j] *= log(2)/ratio_median[j];
		}
	}
	for(int i=0;i<nt;i++){
		memcpy(my_psmc[i]->alphas,total_info[i],sizeof(double)*ns);
		my_psmc[i]->recover();
	}
}

void cor::s_all_cor(){
	for(int i=0;i<nt;i++){
		for(int j=0;j<ns;j++){
			my_psmc[i]->alphas[j] /= alp_median;
		}
		my_psmc[i]->recover();
	}
}

void cor::single_cor(){
	cor_ini();
	s_each_cor();
	s_all_cor();
	rec_tree();
}

void cor::print(const char* output_prefix,bool if_output_ratio){
	char res[]="_res.txt";
	char alp[]="_alp.txt";
	char nh1[]="_nh1.txt";
	//char tree[]="_tree.txt";
	//char Tree[]="_Tree.txt";
	int len_prefix=strlen(output_prefix);
	/*if(if_output_ratio){
		char rat[]="_rat.txt";
		char* output0=(char*)malloc(sizeof(char)*(len_prefix+9));
		memcpy(output0,output_prefix,sizeof(char)*len_prefix);
		memcpy(output0+len_prefix,rat,sizeof(char)*8);
		output0[len_prefix+8]='\0';
		ofstream ofs0(output0);
		free(output0);
		for(int i=0;i<ns;i++) ofs0<<ratio_median[i]<<" ";
		ofs0<<alp_median<<endl;
		ofs0.close();
	}*/
	char* output1=(char*)malloc(sizeof(char)*(len_prefix+9));
	char* output2=(char*)malloc(sizeof(char)*(len_prefix+9));
	char* output3=(char*)malloc(sizeof(char)*(len_prefix+9));
	//char* output3=(char*)malloc(sizeof(char)*(len_prefix+10));
	//char* output4=(char*)malloc(sizeof(char)*(len_prefix+10));
	memcpy(output1,output_prefix,sizeof(char)*len_prefix);
	memcpy(output2,output_prefix,sizeof(char)*len_prefix);
	memcpy(output3,output_prefix,sizeof(char)*len_prefix);
	//memcpy(output3,output_prefix,sizeof(char)*len_prefix);
	//memcpy(output4,output_prefix,sizeof(char)*len_prefix);

	memcpy(output1+len_prefix,res,sizeof(char)*8);
	memcpy(output2+len_prefix,alp,sizeof(char)*8);
	memcpy(output3+len_prefix,nh1,sizeof(char)*8);
	//memcpy(output3+len_prefix,tree,sizeof(char)*9);
	//memcpy(output4+len_prefix,Tree,sizeof(char)*9);

	output1[len_prefix+8]='\0';
	output2[len_prefix+8]='\0';
	output3[len_prefix+8]='\0';
	//output3[len_prefix+9]='\0';
	//output4[len_prefix+9]='\0';

	ofstream ofs1(output1);
	ofstream ofs2(output2);
	ofstream ofs3(output3);
	//ofstream ofs3(output3);
	//ofstream ofs4(output4);
	//cout<<"output1:"<<output1<<endl;
	//cout<<"output2:"<<output2<<endl;
	//cout<<"output3:"<<output3<<endl;
	free(output1);
	free(output2);
	free(output3);
	//free(output3);
	//free(output4);
	//exit(0);
	for(int i=0;i!=nt;i++){
		my_psmc[i]->get_max_alp3();
		//double ch_time1=my_psmc[i]->get_cht1();
		//double ch_time2=my_psmc[i]->get_cht2();
		
		ofs2<<store_trepos[i]<<" ";
		//ofs3<<store_trepos[i]<<" ";
		//ofs4<<store_trepos[i]<<" "<<ntf[i];
		for(int j=0;j<ns;j++){
			if(j!=ns-1){
				ofs2<< my_psmc[i]->alphas[j]<<" ";
				//total_info[i][j]>=2.0?ofs3<<total_info[i][j]<<" ":ofs3<<2.0<<" ";
			}
			else{
				ofs2<< my_psmc[i]->alphas[j];
				//total_info[i][j]>=2.0?ofs3<<total_info[i][j]<<" ":ofs3<<2.0;
			}	
		}
		ofs2<<endl;
		//ofs3<<endl;
		//ofs4<<endl;
		//ofs1<<store_trepos[i]<<" "<<my_psmc[i]->get_alpha()<<" "<<my_psmc[i]->get_diff()<<" "<<my_psmc[i]->get_Alp1()<<" "<<my_psmc[i]->get_Alp2()<<" "<<my_psmc[i]->get_Alp3()<<" "<<my_psmc[i]->get_diff3()<<" "<<ch_time1<<" "<<ch_time2<<" "<<my_psmc[i]->get_index1()<<" "<<my_psmc[i]->get_index2()<<endl;
		FLOAT alpha=my_psmc[i]->get_alpha();
		FLOAT h1=my_psmc[i]->get_diff();
		FLOAT h2=my_psmc[i]->get_diff3();
		
		ofs1<<store_trepos[i]<<" "<<alpha<<" "<<h1<<" "<<h2<<endl;
	}
	ofs1.close();
	ofs2.close();
	ofs3<<get_nh1()<<endl;
	ofs3.close();
	//ofs3.close();
	//ofs4.close();
}
