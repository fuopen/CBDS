#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include"random.h"

void swap(double* a,double* b){
	double tmp=*a;
	*a=*b;
	*b=tmp;
}

void swap(int* a,int* b){
	int tmp=*a;
	*a=*b;
	*b=tmp;
}

int rsample(int begin,int end){
	if(begin>end){
		perror("invaild argument in function:sample, begin is greater than end!");
		exit(4);
	}
	if(begin==end) return begin;
	int len=end-begin;
	return rand()%len+begin;
}

void random_permutf(double *array,int n){
	srand(time(NULL));
	int i,j;
	for(i=0;i<n;i++){
		j=rsample(i,n);
		swap(&array[i],&array[j]);
	}
}

void random_permuti(int *array,int n){
	srand(time(NULL));
	int i,j;
	for(i=0;i<n;i++){
		j=rsample(i,n);
		swap(&array[i],&array[j]);
	}
}
/*int main(int argc,char** argv){
	double test[10]={0.3,-1.3,0.9,1.5,-0.2,0.3,0.5,1.8,0.2,1.7};
	int tmp_info[10];
	int n_l0=0,n_g0=9;
	for(int k=0;k<10;k++){
		test[k]>=0?tmp_info[n_l0++]=k:tmp_info[n_g0--]=k;
	}
	for(int j=0;j<10;j++){
		printf("test[%d]=%.1f ",tmp_info[j],test[tmp_info[j]]);
	}

	int* tmp_info2=(int*)malloc(sizeof(int)*n_l0);
	int n_l1=0,n_g1=n_l0;
	for(int k=0;k<n_l0;k++){
		test[tmp_info[k]]>1?tmp_info2[n_l1++]=tmp_info[k]:tmp_info2[n_g1--]=tmp_info[k];
	}
	for(int k=0;k<n_l0;k++){
		printf("%d -----%d\n",tmp_info[k],tmp_info2[k]);
	}
	memcpy(tmp_info,tmp_info2,sizeof(int)*n_l0);
	free(tmp_info2);
	printf("\nnl0=%d,ng0=%d,nl1=%d,ng1=%d\n",n_l0,n_g0,n_l1,n_g1);
	for(int k=0;k<10;k++){
		printf("\n%d\n",tmp_info[k]);
	}
	random_permuti(tmp_info,n_l1);
	random_permuti(tmp_info+n_l1,n_l0-n_l1);
	random_permuti(tmp_info+n_l0,10-n_l0);
	printf("\nafter permutation\n");
	
	for(int i=0;i<10;i++) {
		printf("test[%d]=%.1f ",tmp_info[i],test[tmp_info[i]]);
	}
	printf("\n");
	return 0;
}*/
