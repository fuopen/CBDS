#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<cstring>
#include"fast_median.h"
#include"random.h"
void rswap(double& a, double& b){
	double tmp = a;
	a = b;
	b = tmp;
}

void iswap(int& a, int& b){
	int tmp = a;
	a = b;
	b = tmp;
}

int sample(const int& begin,const int& end){
	if(begin>end){
		perror("error when the begin greater than end!\n");
		exit(2);
	}
	if(begin==end) return begin;
	int len = end-begin+1;
	//srand((unsigned)(time(NULL)+begin+end));
	int rand_index = rand() % len + begin;
	//int rand_index=(end-begin+1)%2==0? (begin+end-1)/2:(begin+end)/2;
	return rand_index;
}

/*void random_permut(int *array,const int& n){
	srand(time(NULL));	
}*/

int partition(double* sort_array,const int& begin, const int& end){
	double x = sort_array[end];
	int i = begin-1;
	for(int j=begin;j<end;j++){
		if(sort_array[j]<=x){
			i++;
			rswap(sort_array[i],sort_array[j]);
		}
	}
	rswap(sort_array[i+1],sort_array[end]);
	return i+1;
}

int ipartition(double* sort_array,int* indices,const int& begin, const int& end){
	double x = sort_array[end];
	int i = begin-1;
	for(int j=begin;j<end;j++){
		if(sort_array[j]<=x){
			i++;
			rswap(sort_array[i],sort_array[j]);
			iswap(indices[i],indices[j]);
		}
	}
	rswap(sort_array[i+1],sort_array[end]);
	iswap(indices[i+1],indices[end]);
	return i+1;
}
int  random_partition(double* sort_array, const int& begin, const int& end){
	int s = sample(begin,end);
	rswap(sort_array[end],sort_array[s]);
	return partition(sort_array,begin,end);
}

int  irandom_partition(double* sort_array, int* indices,const int& begin, const int& end){
	int s = sample(begin,end);
	rswap(sort_array[end],sort_array[s]);
	iswap(indices[end],indices[s]);
	return ipartition(sort_array,indices,begin,end);
}

double random_select(double* sort_array, const int& begin, const int& end,const int& i){
	if(begin==end){
		return sort_array[begin];
	}
	int q = random_partition(sort_array,begin,end);
	int k = q-begin+1;

	if(i==k){
		return sort_array[q];
	}
	else if(i<k){
		return random_select(sort_array,begin,q-1,i);
	}
	else return random_select(sort_array,q+1,end,i-k);
}

double irandom_select(double* sort_array, int* indices,const int& begin, const int& end,const int& i){
	if(begin==end){
		return sort_array[begin];
	}
	int q = irandom_partition(sort_array,indices,begin,end);
	int k = q-begin+1;

	if(i==k){
		return sort_array[q];
	}
	else if(i<k){
		return irandom_select(sort_array,indices,begin,q-1,i);
	}
	else return irandom_select(sort_array,indices,q+1,end,i-k);
}

double fast_median(double* sort_array,const int& array_len){
	if(array_len==0){
		perror("array length is invalid!\n");
		exit(2);
	}
	if(array_len==1){
		return sort_array[0];
	}
	srand(time(NULL));
	if(array_len%2==0){
		return random_select(sort_array,0,array_len-1,array_len/2);
	}
	else{
		return random_select(sort_array,0,array_len-1,(array_len+1)/2);
	}
}

double fast_quantile(double* sort_array,const int& array_len,const double& kq){
	if(array_len==0){
		perror("array length is invalid!\n");
		exit(2);
	}
	if(array_len==1){
		return sort_array[0];
	}
	srand(time(NULL));
	return random_select(sort_array,0,array_len-1,(int) round(array_len*kq));
}

double fast_index(double* sort_array,const int& array_len, const int& idx){
	if(array_len==0){
		perror("array length is invalid!\n");
		exit(2);
	}
	if(array_len==1){
		return sort_array[0];
	}
	if(idx>array_len-1){
		perror("index is over the range\n");
		exit(3);
	}
	srand(time(NULL));
	return random_select(sort_array,0,array_len-1,idx);
}
	
double ifast_median(double* sort_array,int* indices,const int& array_len){
	if(array_len==0){
		perror("array length is invalid!\n");
		exit(2);
	}
	if(array_len==1){
		return sort_array[0];
	}
	srand(time(NULL));
	if(array_len%2==0){
		return irandom_select(sort_array,indices,0,array_len-1,array_len/2);
	}
	else{
		return irandom_select(sort_array,indices,0,array_len-1,(array_len+1)/2);
	}
}

double ifast_quantile(double* sort_array,int* indices,const int& array_len,const double& kq){
	if(array_len==0){
		perror("array length is invalid!\n");
		exit(2);
	}
	if(array_len==1){
		return sort_array[0];
	}
	srand(time(NULL));
	return irandom_select(sort_array,indices,0,array_len-1,(int) round(array_len*kq));
}

double ifast_index(double* sort_array,int* indices,const int& array_len, const int& idx){
	if(array_len==0){
		perror("array length is invalid!\n");
		exit(2);
	}
	if(array_len==1){
		return sort_array[0];
	}
	if(idx>array_len-1){
		perror("index is over the range\n");
		exit(3);
	}
	srand(time(NULL));
	return irandom_select(sort_array,indices,0,array_len-1,idx);
}
/*int main(int argc,char** argv){
	double* db=(double *)malloc(sizeof(double)*(argc-1));
	int* id = (int*)malloc(sizeof(int)*(argc-1));
	for(int i=0;i<argc-1;i++){
		db[i] = atof(argv[i]);
		//id[i] = i;
	}
	double* db_cp=(double*)malloc(sizeof(double)*(argc-1));
	memcpy(db_cp,db,sizeof(double)*(argc-1));
	double am=fast_median(db_cp,argc-1);
	free(db_cp);
	int n_l0=0,n_g0=argc-2;
	for(int i=0;i<argc-1;i++){
		db[i]<am?id[n_l0++]=i:id[n_g0--]=i;
	}
	int *p = id;
	random_permuti(p,n_l0);
	random_permuti(p+n_l0,argc-1-n_l0);

	double pc[5]={0.01,0.02,0.03,0.04,0.05};
	double qt[5]={0,0,0,0,0};
	double am=ifast_median(db,id,argc-1);
	int* id_cp=(int*) malloc(sizeof(int)*(argc-1));
	int indx[5]={0};
	int half_len=(argc-1)%2==0?(argc-1)/2:argc/2;
	int array_len=half_len;
	for(int i=0;i<5;i++){
		indx[i]=(int) round(pc[4-i]*(argc-1));
		qt[i]=ifast_index(db,id,array_len,indx[i]);
		array_len=indx[i];
	}
	memcpy(id_cp,id,sizeof(int)*(argc-1));
	int* p=id;

	for(int i=0;i<5;i++){
		if(i==0){
			random_permuti(p,indx[4-i]);
		}
		else{
			random_permuti(p,indx[4-i]-indx[4-i+1]);
		}
		p = id+indx[4-i];
	}
	random_permuti(p,half_len-indx[0]);
	random_permuti(id+half_len,argc-1-half_len);
	//FILE* fp=fopen("c2_index.txt","w");
	//fprintf(fp,"n_l0=%d\n",n_l0);
	//fclose(fp);

	for(int i=0;i<argc-1;i++){
		printf("%.6f\t%d\n",db[i],id[i]);
	}
	free(id);
	//free(id_cp);
	free(db);
	return 0;
}*/
