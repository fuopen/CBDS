#include<math.h>
#include<string.h>
#include"fast_median.h"
#include"mean_sd.h"
#include<iostream>
//#include<gsl/gsl_sf_gamma.h>

using namespace std;

double SampleMean(const double* array,const int& len){
	double sum=0;
	int i;
	for(i=0;i!=len;i++) sum+=array[i];
	return sum/((double) len);
}

double SampleMoment(const double* array,const int& len,const double& order,const double& central){
	double sum=0;
	int i;
	for(i=0;i!=len;i++) sum+=pow((array[i]-central),order);
	return sqrt(sum/((double) len));
}

void SampleNormalization(double* array,const int& len,const double& scale){
	double* array_tmp=(double*) malloc(sizeof(double)*len);
	memcpy(array_tmp,array,sizeof(double)*len);
	double central=fast_median(array_tmp,len);
	free(array_tmp);
	double mean_sd=SampleMoment(array,len,2,central);
	int i;
	for(i=0;i!=len;i++) array[i]=central+scale*(array[i]-central)/mean_sd;
}

void Weibull(double* array,const int& len,const double& scale,const double& shape){
	int i;
	for(i=0;i!=len;i++) array[i]=scale*pow(array[i],1.0/shape);
	//double weibullvar = scale*scale*(gsl_sf_gamma(1.0+2.0/shape)-gsl_sf_gamma(1.0+1.0/shape)*gsl_sf_gamma(1.0+1.0/shape));
	double weibullvar=scale*scale*0.052;
	SampleNormalization(array,len,sqrt(weibullvar));
	for(i=0;i!=len;i++) array[i]=pow(array[i]/scale,shape);
}

/*int main(){
	double a[10]={0.3,2.3,1.2,0.3,0.9,1.2,0.8,0.4,0.5,4.1};
	cout<<"sm: "<<SampleMean(a,10)<<endl;
	cout<<"sd: "<<SampleMoment(a,10,2,SampleMean(a,10))<<endl;
	Weibull(a,10);
	int i;
	for(i=0;i<10;i++)cout<<"a["<<i<<"]="<<a[i]<<endl;
	return 0;
}*/	
