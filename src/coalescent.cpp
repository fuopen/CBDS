#include"coalescent.h"
psmc::psmc(const double& ip,const vector<string>& store_data,const double* t_time,const int&ct_size):inpop_size(ip),intv_size(0),t_size(0),coal_size(0),c_gen(NULL),lambda(NULL),t_gen(NULL),total_time(NULL),tau(NULL),t(NULL),neu_minus_logp(0.0),sel_minus_logp(0.0),diff(0.0),alpha(0.0),Alp1(0.0),Alp2(0.0),Alp3(0.0),alp3_minus_logp(0.0),change_time1(0),change_time2(0),alphas(NULL),recover_tree(NULL),spop_time(NULL),recover_delta(NULL){
	initiate(store_data);
	insert_time(t_time,ct_size);
	calculate();
}

psmc::~psmc(){
	if(t!=NULL){
		delete[] t;
		t=NULL;
	}
	if(total_time!=NULL){
		delete[] total_time;
		total_time=NULL;
	}
	if(c_gen!=NULL)delete[] c_gen;
	if(lambda!=NULL)delete[] lambda;
	if(t_gen!=NULL)delete[] t_gen;
	if(tau!=NULL)delete[] tau;
	if(alphas!=NULL)delete[] alphas;
	if(recover_tree)delete[] recover_tree;
	if(spop_time)delete[] spop_time;
	if(recover_delta)delete[] recover_delta;
}

void psmc::clear(){
	t_size = 0;
	coal_size = 0;
	if(total_time!=NULL){
		delete[] total_time;
		total_time=NULL;
	}
	if(t!=NULL){
		delete[] t;
		t = NULL;
	}
	if(alphas!=NULL){
		delete[] alphas;
		alphas = NULL;
	}
	neu_minus_logp = 0.0;
	sel_minus_logp = 0.0;
	diff = 0.0;
	alpha = 0.0;
}

void psmc::initiate(const vector<string>& store_data){
	intv_size = (int) store_data.size()/3;
	c_gen = new double[intv_size];
	lambda = new double[intv_size];
	t_gen = new double[intv_size];
	tau = new unsigned int[intv_size];
	for(unsigned int i=0;i<intv_size;i++){
		tau[i] = atoi(store_data[3*i].data());
		c_gen[i] = atof(store_data[3*i+1].data());
		lambda[i] = atof(store_data[3*i+2].data());
		//*********************************
		//	cout<<tau[i]<<" "<<c_gen[i]<<" "<<lambda[i]<<endl;
		if(i==0)t_gen[i] = c_gen[i];
		else t_gen[i] = t_gen[i-1]+(c_gen[i]-c_gen[i-1])*2*inpop_size;
	}
	//*********************8
	//exit(0);
	spop_time = new double[intv_size];
	spop_time[0]=c_gen[0];
	//cout<<spop_time[0]<<" ";
	for(unsigned i=1;i<intv_size;i++){
		spop_time[i]=spop_time[i-1]+(c_gen[i]-c_gen[i-1])/lambda[i-1];
		//cout<<spop_time[i]<<" ";
	}
	//cout<<endl;
	//exit(0);
	//***********************
}

void psmc::insert_time(const double*t_time,const int& ct_size){
	coal_size = ct_size;
	t_size = ct_size+intv_size;
	total_time = new sd[t_size];
	t = new unsigned int[ct_size];
	for(unsigned int i=0;i<ct_size;i++){
		t[i] = i+intv_size;
	}
	for(unsigned int i=0;i<intv_size;i++){
		total_time[i].value = t_gen[i];
		total_time[i].p_pos = tau[i];
		total_time[i].a_pos = i;
		total_time[i].type  = 0;
	}
	for(unsigned int i=intv_size;i<t_size;i++){
		total_time[i].value = t_time[i-intv_size];
		total_time[i].p_pos = i-intv_size;
		total_time[i].a_pos =i; 
		total_time[i].type  = 1;
	}
	sort(total_time,tau,t,t_size);
}
void psmc::print_info(){
	//****************************
	for(unsigned i=0;i<t_size;i++)
		cout<<total_time[i].value<<" ";
	cout<<endl;
	cout<<"intv_size: "<<intv_size<<endl;
	for(unsigned i=0;i<intv_size;i++)
		cout<<"t["<<tau[i]<<"]="<<total_time[tau[i]].value<<" ";
	cout<<endl;
	cout<<"coal_size: "<<coal_size<<endl;
	for(unsigned i=0;i<coal_size;i++)
		cout<<"t["<<t[i]<<"]="<<total_time[t[i]].value<<" ";
	cout<<endl;
	exit(0);
	//****************************
}

void psmc::sort(sd* array,unsigned int* index1,unsigned int* index2,const unsigned int& size){
	for(unsigned int i=0;i<size-1;i++){
		for(unsigned int j=size-1;j>i;j--){
			if(array[j].value<array[j-1].value){
				swap(array[j],array[j-1]);
				if(array[j].type==0&&array[j-1].type==1){
					tau[array[j].p_pos]=array[j].a_pos;
					t[array[j-1].p_pos]=array[j-1].a_pos;
				}
				if(array[j].type==1&&array[j-1].type==0){
					t[array[j].p_pos]=array[j].a_pos;
					tau[array[j-1].p_pos]=array[j-1].a_pos;
				}
			}
		}
	}
}

void psmc::swap(sd& a,sd& b){
	sd* sd_tmp = new(sd);
	sd_tmp->value = a.value;
	sd_tmp->p_pos = a.p_pos;
	sd_tmp->type  = a.type;
	a.value = b.value;
	a.p_pos = b.p_pos;
	a.type  = b.type;
	b.value = sd_tmp->value;
	b.p_pos = sd_tmp->p_pos;
	b.type  = sd_tmp->type;
	delete sd_tmp;
}

void psmc::calculate(){
	double sum1 = 0.0;
	double sum2 = 0.0;
	alphas = new double[coal_size];
	int c_event=coal_size+1;
	for(unsigned int i=0;i<coal_size;i++){
		double tmp=0.0;
		//double Tmp=0.0;
		if(i==0){
			if(t[i]==1){
			tmp=c_event*(c_event-1)*total_time[1].value/(2.0*lambda[0]*2*inpop_size);
			}
			else{
				for(unsigned int j=1;j<t[i];j++){
					tmp += (total_time[j].value-total_time[j-1].value)/lambda[j-1];
				}
				tmp += (total_time[t[i]].value-total_time[t[i]-1].value)/lambda[t[i]-i-1];
				tmp *= c_event*(c_event-1)/(2.0*2*inpop_size);
			}
		}
		else{
			if((t[i]-t[i-1])==1){
				tmp = c_event*(c_event-1)*(total_time[t[i]].value-total_time[t[i-1]].value)/(2.0*lambda[t[i]-i-1]*2.0*inpop_size);
			}
			else{
				for(unsigned int k2=t[i-1];k2<t[i]-1;k2++){
					tmp += (total_time[k2+1].value-total_time[k2].value)/lambda[k2-i];
				}
				tmp += (total_time[t[i]].value-total_time[t[i]-1].value)/lambda[t[i]-i-1];
				tmp *= c_event*(c_event-1)/(2.0*2*inpop_size);
			}
		}
		alphas[i] = tmp;
		c_event--;
	}
	cal_stat();
}

void psmc::initialize(){
	alpha=0.0;
	neu_minus_logp=0.0;
	sel_minus_logp=0.0;
	diff=0.0;
}

void psmc::cal_stat(){
	initialize();
	for(unsigned i=0;i<coal_size;i++) neu_minus_logp += alphas[i];
	alpha=neu_minus_logp/coal_size;
	sel_minus_logp =coal_size*(1.0+log(alpha)); 
	diff = neu_minus_logp-sel_minus_logp;
}
	
void psmc::recover(){
	if(recover_tree){
		delete recover_tree;
	}
	if(recover_delta){
		delete recover_delta;
	}
	recover_tree = new double[coal_size+1];
	recover_delta =new double[coal_size];
	unsigned p2=0;
	recover_tree[0]=0.0;
	for(unsigned i=1;i<=coal_size;i++){
		recover_tree[i]=recover_tree[i-1]+alphas[i-1]*2.0/((coal_size+2-i)*(coal_size+1-i));
	}

	for(unsigned i=1;i<=coal_size;i++){
		if(p2<intv_size){
			for(unsigned k=p2;k<intv_size;k++){
				if(recover_tree[i]<=spop_time[k]){
					p2=k;
					break;
				}
			}
		}
		if(recover_tree[i]<=spop_time[intv_size-1]){
			recover_tree[i-1]=c_gen[p2-1]*2.0*inpop_size+(recover_tree[i]-spop_time[p2-1])*2.0*inpop_size*lambda[p2-1];
		}
		else{
			recover_tree[i-1]=c_gen[intv_size-1]*2.0*inpop_size+(recover_tree[i]-spop_time[intv_size-1])*2*inpop_size*lambda[intv_size-1];
		}
	}
	recover_delta[0]=recover_tree[0];
	for(unsigned i=1;i<coal_size;i++){
		recover_delta[i]=recover_tree[i]-recover_tree[i-1];
	}
	cal_stat();
}

double psmc::get_n_logp()const{
	return neu_minus_logp;
}

double psmc::get_s_logp()const{
	return sel_minus_logp;
}

double psmc::get_diff()const{
	return diff;
}

double psmc::get_alpha()const{
	return alpha;
}

void psmc::get_max_alp3(){
	for(unsigned i=1;i<coal_size-1;i++){
		for(unsigned j=i+1;j<coal_size;j++){
			double tp1=0.0,tp2=0.0,tp3=0.0,tp4=0.0;
			cal_alp3(tp1,tp2,tp3,tp4,i,j);
			if(i==1&&j==2){
				Alp1=tp1;
				Alp2=tp2;
				Alp3=tp3;
				alp3_minus_logp=tp4;
				change_time1=i;
				change_time2=j;
			}
			else if(tp4<alp3_minus_logp){
				Alp1=tp1;
				Alp2=tp2;
				Alp3=tp3;
				alp3_minus_logp=tp4;
				change_time1=i;
				change_time2=j;
			}
			else continue;
		}
	}
}

void psmc::cal_alp3(double& tp1,double& tp2,double& tp3, double& tp4,const unsigned& u,const unsigned& v){
	double tmp1=0.0,tmp2=0.0,tmp3=0.0,tmp4=0.0;
	if(u==1&&alphas[0]<0.01){
		tp1=100000.0;
	}
	else{
		for(unsigned i=0;i<u;i++)
			tmp1 += alphas[i];
		tp1 = tmp1/(u+0.000001);
	}

	if((v==u+1)&&(alphas[u]<0.01)) tp2=100000.0;
	else{
		for(unsigned i=u;i<v;i++)
			tmp2 += alphas[i];
		tp2 = tmp2/(v-u+0.000001);
	}
	
	if((v==coal_size-1)&&alphas[coal_size-1]<0.01) tp3=100000.0;
	else{
		for(unsigned i=v;i<coal_size;i++)
			tmp3 += alphas[i];
		tp3 = tmp3/(coal_size-v+0.000001);
	}
	tp4 = u*(log(tp1)+1.0)+(v-u)*(log(tp2)+1.0)+(coal_size-v)*(log(tp3)+1.0);
}
