#ifndef _FAST_MEDIAN_
#define _FAST_MEDIAN_

void rswap(double& a, double& b);
void iswap(int &a, int &b);
int sample(const int& begin,const int& end);
//void random_permut(int *array, const int& n);
int partition(double* sort_array,const int& begin, const int& end);
int ipartition(double* sort_array,int* indices,const int& begin, const int& end);
int  random_partition(double* sort_array, const int& begin, const int& end);
int  irandom_partition(double* sort_array, int* indices,const int& begin, const int& end);
double random_select(double* sort_array, const int& begin, const int& end,const int& i);
double irandom_select(double* sort_array, int* indices,const int& begin, const int& end,const int& i);
double fast_median(double* sort_array,const int& array_len);
double fast_quantile(double* sort_array,const int& array_len,const double& kq);
double fast_index(double* sort_array,const int&	array_len, const int& idx);
double ifast_median(double* sort_array,int* indices,const int& array_len);
double ifast_quantile(double* sort_array,int* indices,const int& array_len,const double& kq);
double ifast_index(double* sort_array,int* indices,const int& array_len, const int& idx);

#endif
