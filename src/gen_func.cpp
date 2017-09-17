#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#include"gen_func.h"

int ndec(int dec){
	int f=1;
	while(dec/10){
		f++;
		dec/=10;
	}
	return f;
}

int intpow(const int a,const int b){
	int power=1;
	int i;
	for(i=0;i<b;i++) power*=a;
	return power;
}

STRING i2a(int dec){
	STRING str=(STRING)malloc(sizeof(char)*INTBUF);
	int len=ndec(dec);
	int i;
	for(i=len;i>0;i--){
		char s = dec/intpow(10,i-1)+48;
		str[len-i]=s;
		dec = dec%intpow(10,i-1);
	}
	str[len]='\0';
	return str;
}

STRING StrTrunk(CSTRING str,int beg,int end){
	if(beg>end||(int)strlen(str)<end) return NULL;
	STRING substr=(STRING)malloc(SIZECHAR*(end-beg+2));
	memcpy(substr,str+beg,SIZECHAR*(end-beg+2));
	substr[end-beg+1]='\0';
	return substr;
}

STRING f2a(FLOAT f,int prec){
	FLOAT rf=(FLOAT) intpow(10,prec)*f;
	int irf=(int) rf;
	int quotient =irf/intpow(10,prec);
	int remainder=irf%intpow(10,prec);
	STRING squot=i2a(quotient);
	STRING srema=i2a(remainder);
	int len_quo=(int) strlen(squot);
	int len_rem=(int) strlen(srema);
	STRING fa=(STRING)malloc(SIZECHAR*(len_quo+prec+2));
	memcpy(fa,squot,SIZECHAR*len_quo);
	fa[len_quo]='.';
	if(len_rem>=prec){
		memcpy(fa+len_quo+1,srema,SIZECHAR*prec);
		fa[len_quo+1+prec]='\0';
	}
	else{
		memcpy(fa+len_quo+1,srema,SIZECHAR*len_rem);
		int i;
		for(i=len_quo+1+len_rem;i!=len_quo+prec+1;i++) fa[i]='0';
		fa[len_quo+1+prec]='\0';
	}
	free(squot);
	free(srema);
	return fa;
}

STRING c2a(char c){
	STRING sc=(STRING)malloc(SIZECHAR*2);
	sc[0]=c;
	sc[1]='\0';
	return sc;
}

FLOAT* cumsum(const FLOAT* farray,int n){
	FLOAT* cs=(FLOAT*)malloc(sizeof(FLOAT)*n);
	cs[0]=farray[0];
	int i;
	for(i=1;i!=n;i++) cs[i]=cs[i-1]+farray[i];
	return cs;
}

/*int main(int argc,char** argv){
	float fs=atof(argv[1]);
	STRING fuck1=f2a(fs,2);
	printf("ts:%s\n",fuck1);
	free(fuck1);
	return 0;
}*/
