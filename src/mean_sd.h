#ifndef _MEAN_SD_H_
#define _MEAN_SD_H_
#include<stdio.h>
#include<stdlib.h>
double SampleMean(const double*,const int&);
double SampleMoment(const double*,const int&,const double&,const double&);
void SampleNormalization(double*,const int&);
void Weibull(double*,const int&,const double& scale=1.087,const double& shape=5);
#endif
