#include "quantile.h"

void quantile25(double* array,const int& len,double& q25,double& q50,double& q75){
	q50=fast_median(array,len);
	if(len%2==0){
		q25=fast_median(array,len/2);
		q75=fast_median(array+len/2,len/2);
	}
	else{
		q25=fast_median(array,(len+1)/2);
		q75=fast_median(array+(len+1)/2-1,(len+1)/2);
	}
}
