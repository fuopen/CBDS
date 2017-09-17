#include<stdio.h>
#include<stdlib.h>
#include"gen_func.h"
#include"file.h"
#include"tree.h"
#include"list.h"
#include"update.h"

void UpdateTreeFile(CSTRING arg1,CSTRING arg2,CSTRING arg3){
		FLOAT** flt;
		STRING* itv;
		STRING* tfs;
		int frow=0,fcol=0,trow=0;
		MakeFloatTable(arg2,&flt,&frow,&fcol);
		MakeTreeFormat(arg1,&itv,&tfs,&trow);
		STRING* ntf=(STRING*) malloc(sizeof(STRING)*trow);
		int i;
		FILE* fp=fopen(arg3,"w");
		for(i=0;i!=trow;i++){
			STRING s=tfs[i];
			FLOAT* f=cumsum(flt[i],fcol);
			CorrectTree(tfs[i],&ntf[i],f,fcol);
			fprintf(fp,"%s %s",itv[i],ntf[i]);
			if(i!=trow-1)fprintf(fp,"\n");
			free(f);
			free(itv[i]);
			free(tfs[i]);
			free(flt[i]);
			free(ntf[i]);
		}
		free(tfs);
		free(itv);
		free(flt);
		free(ntf);
		fclose(fp);
}

void MakeIntvFile(CSTRING arg1,CSTRING arg2){
		FLOAT** flt;
		STRING* itv;
		STRING* tfs;
		int trow=0;
		MakeTreeFormat(arg1,&itv,&tfs,&trow);
		int i;
		FILE* fp=fopen(arg2,"w");
		for(i=0;i!=trow;i++){
			STRING s=tfs[i];
			FLOAT* f;
			int n;
			TreeIntvs(s,&f,&n);
			fprintf(fp,"%s ",itv[i]);
			int j;
			for(j=0;j!=n;j++){
				if(j!=n-1) fprintf(fp,"%.3f ",f[j]);
				else fprintf(fp,"%.3f",f[j]);
			}
			if(i!=trow-1)fprintf(fp,"\n");
			if(f) free(f);
			free(itv[i]);
			free(tfs[i]);
		}
		free(tfs);
		free(itv);
		fclose(fp);
}	
