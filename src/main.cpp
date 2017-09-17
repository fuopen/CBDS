//#include "cor.h"
#include<stdio.h>
#include<stdlib.h>
#include<cctype>
#include<iostream>
#include<unistd.h>
#include"call.h"
using namespace std;
#define PRINTCHAR(pc) (isprint((unsigned char) pc) ? pc : '@')
#define ARGNUM 8

FLOAT VERSION_NUM=1.01;
static int ARG=0;
static void Usage(char* ProgName){
	cout<<"Usage: "<<ProgName<<" [-t arg] [-d arg] [-r arg] [-o arg] [-p arg] [-i arg] [-m arg] [-v ]"<<endl;
	cout<<"-t\ttree format file"<<endl;
	cout<<"-d\tdemographic trajectory file"<<endl;
	cout<<"-r\talpha ratio file"<<endl;
	cout<<"-o\toutput directory"<<endl;
	cout<<"-p\tprefix,default is PsmcTree"<<endl;
	cout<<"-i\tinterval file"<<endl;
	cout<<"-m\tif output median file"<<endl;
	cout<<"-v\tprogram verstion number"<<endl;
}

static void UsageError(char* ProgName, char* msg,int opt){
	if(msg&&opt) cerr<<"Usage: "<<msg<<" (-"<<PRINTCHAR(opt)<<")\n";
	Usage(ProgName);
	exit(-1);
}

static int CombineArgs(char c){
	switch(c){
		case 't': ARG =ARG|0x1;break;
		case 'd': ARG =ARG|0x2;break;
		case 'r': ARG =ARG|0x4;break;
		case 'o': ARG =ARG|0x8;break;
		case 'p': ARG =ARG|0x10;break;
		case 'i': ARG =ARG|0x20;break;
		case 'm': ARG =ARG|0x40;break;
		case 'v': ARG =ARG|0x80;break;
		default : ARG =0x8F;
				  cerr<<"Invalid arguments"<<endl;
				  exit(-2);
				  break;
	}
}

int main(const int argc,char** argv){	
	/*if(argc<4){
		cerr<<"error for uncorrect input arguments"<<endl;
		exit(-1);
	}
	
	if(argc==4){
		cor psmc_cor(argv[1],argv[2]);
		psmc_cor.print(argv[3],true);
	}
	if(argc==5){
		cor psmc_cor(argv[1],argv[2],argv[3]);
		psmc_cor.print(argv[4]);	
	}*/
	if(argc==1){
		Usage(argv[0]);	
		exit(0);
	}	
	int opt;
	char* pstr[ARGNUM]={NULL};

	while((opt=getopt(argc,argv,":t:d:r:o:p:i:mv"))!=-1){
		switch(opt){
			case 't': CombineArgs('t');pstr[0]=optarg;break;
			case 'd': CombineArgs('d');pstr[1]=optarg;break;
			case 'r': CombineArgs('r');pstr[2]=optarg;break;
			case 'o': CombineArgs('o');pstr[3]=optarg;break;
			case 'p': CombineArgs('p');pstr[4]=optarg;break;
			case 'i': CombineArgs('i');pstr[5]=optarg;break;
			case 'm': CombineArgs('m');break;
			case 'v': CombineArgs('v');break;
			case ':': UsageError(argv[0],"Missing argument",optopt);break;
			case '?': UsageError(argv[0],"Unrecognized option",optopt);break;
			default: cerr<<"fatal:unecpected case of resloving\n";
					 exit(-3);
		}
	}
	CallWhich(pstr,ARG,VERSION_NUM);
	/*switch(ARG){
		case 0x3: cout<<"calling function psmc_cor1, with arguments -t and -d"<<pstr[0]<<" "<<pstr[1]<<endl;break;
		case 0x7: cout<<"calling function psmc_cor2, with arguments -t,-d,-r"<<endl;break;
		case 0xB:cout<<"calling function psmc_cor1, with arguments -t,-d,-o"<<endl;break;
		case 0x13:cout<<"calling function psmc_cor1,with arguments -t,-d,-p"<<nedl;break;
		case 0x1B:cout<<"calling function psmc_cor1,with arguments -t,-d,-o,-p"<<endl;break;
		case 0x0F:cout<<"calling function psmc_cor2,with arguments -t,-d,-r,-o"<<endl;break;
		case 0x1F:cout<<"calling function psmc_cor2,with arguments -t,-d,-r,-o,-p"<<endl;break;
		case 0x21:cout<<"calling function update_tree,with arguments -t,-i"<<endl;break;
		case 0x29:cout<<"calling function update_tree,with arguments -t,-i,-o"<<endl;break;
		case 0x43:cout<<"-t -d -m"<<endl;break;
		case 0x4B:cout<<"-t -d -o -m"<<endl;break;
		case 0x5B:cout<<"-t -d -o -p -m"<<endl;break;
		case 0x80:cout<<"this software is of version : "<<VERSION_NUM<<endl;break;
		case 0x9:cout<<"calling function of decomposing tree intervels from tree, with argument -t,-o"<<endl;break;
		default:
				 cerr<<"Invalid combination of arguments"<<endl;
				 exit(-4);
	}*/ 
	return 0;
}
