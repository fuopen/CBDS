#include<string.h>
#include<stdlib.h>
#include<iostream>
#include "update.h"
#include "psmc.h"
#include "macro.h"
#include "call.h"
using namespace std;

STRING Paste(CSTRING str1,CSTRING str2){
	int len1=(int) strlen(str1);
	int len2=(int) strlen(str2);
	char sep=str1[len1-1];
	STRING s;
	if(sep!='/'){
		s=(STRING) malloc(sizeof(char)*(len1+len2+2));
		memcpy(s,str1,sizeof(char)*(len1+1));
		s[len1]='/';
		memcpy(s+len1+1,str2,sizeof(char)*(len2+1));
		s[len1+len2+1]='\0';
	}
	else{
		s=(STRING) malloc(sizeof(char)*(len1+len2+1));
		memcpy(s,str1,sizeof(char)*(len1+1));
		memcpy(s+len1,str2,sizeof(char)*len2);
		s[len1+len2]='\0';
	}
	return s;
}

void CallWhich(STRING* args,int call_code,FLOAT version){
	STRING s=NULL;
	CSTRING outdir=args[3];
	CSTRING prefix=args[4];
	switch(call_code){
		case 0x3:
				Cor3Args(args[0],args[1],"PSMC");
				break;
		case 0x7:
				Cor4Args(args[0],args[1],args[2],"PSMC_RATIO");
				break;
		case 0xB:
				s=Paste(outdir,"PSMC");
				Cor3Args(args[0],args[1],s);
				break;
		case 0x13:
				Cor3Args(args[0],args[1],args[4]);
				break;
		case 0x1B:
				outdir=args[3];
			  	prefix=args[4];
				s=Paste(outdir,prefix);
				Cor3Args(args[0],args[1],s);
				break;
		case 0x0F:
				outdir=args[3];
				s=Paste(outdir,"PSMC_RATIO");
				Cor4Args(args[0],args[1],args[2],s);
				break;
		case 0x1F:
				outdir=args[3];
				prefix=args[4];
				s=Paste(outdir,prefix);
				Cor4Args(args[0],args[1],args[2],s);
				break;
		case 0x21:
				UpdateTreeFile(args[0],args[5],"PSMC_UPDATE.tree");
				break;
		case 0x29:
				UpdateTreeFile(args[0],args[5],args[3]);
				break;
		case 0x43:
				Cor3Args(args[0],args[1],"PSMC",true);
				break;
		case 0x4B:
				outdir=args[3];
				s=Paste(outdir,"PSMC");
				Cor3Args(args[0],args[1],s,true);
				break;
		case 0x5B:
				outdir=args[3];
			  	prefix=args[4];
				s=Paste(outdir,prefix);
				Cor3Args(args[0],args[1],s,true);
				break;
		
		case 0x1:
				MakeIntvFile(args[0],"PSMC_TREE_INTV.txt");
				break;
		case 0x9:
				MakeIntvFile(args[0],args[3]);
				break;
		case 0x80:
				cout<<"this software is of version: "<<version<<endl;
				exit(0);
		default :
				cerr<<"Invalid combination of arguments"<<endl;
				exit(-4);
	}
	if(s) free(s);
}
