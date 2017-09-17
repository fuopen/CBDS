#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"gen_func.h"
#include"file.h"

void MakeFile(CSTRING filename,STRING* str){
	FILE* filept=NULL;

	INT64 filesize;
	filept=fopen(filename,"r");
	if(!filept) return ;//NULL;
	fseek(filept,0,SEEK_END);
	filesize=ftell(filept);
	rewind(filept);

	*str=(char*)malloc(SIZECHAR*(filesize+1));
	if(fread(*str,1,filesize,filept)!=filesize) return ;//NULL;
	(*str)[filesize]='\0';
	fclose(filept);
	filept=NULL;
}

void MakeLines(CSTRING filename,STRING** str,int* NR){
	if(!filename) return;
	STRING content;
	MakeFile(filename,&content);
	const char* cseek=content;
	*NR=0;
	while(*cseek!='\0'){
		cseek++;
		if(*cseek=='\n'||(!isspace(*(cseek-1))&&*cseek=='\0'))(*NR)++;
	}

	*str=(STRING*)malloc(sizeof(STRING)*(*NR));
	cseek=content;
	const char* loc=content;
	int id=0,cp_len=0;
	while(*cseek!='\0'){
		loc=cseek;
		while(*cseek!='\n'&&*cseek!='\0')cseek++;
		if(isspace(*(cseek-1))) cp_len=(int) (cseek-loc); 
		else cp_len=(int) (cseek-loc)+1;
		(*str)[id]=(STRING)malloc(SIZECHAR*cp_len);
		memcpy((*str)[id],loc,SIZECHAR*cp_len);
		(*str)[id][cp_len-1]='\0';
		if(*cseek!='\0'){
			cseek++;
			id++;
		}
	}
	free(content);
}

void MakeTable(CSTRING filename,TB* tb){
	STRING* row;
	MakeLines(filename,&row,&tb->nrow);
	const char* fd=row[0];
	tb->ncol=0;
	while(*fd!='\0'){
		fd++;
		if(isspace(*fd)||*fd=='\0') tb->ncol++;
	}
	tb->strs=(STRING**)malloc(sizeof(STRING*)*tb->ncol);
	int i,j;
	for(i=0;i!=tb->ncol;i++) tb->strs[i]=(STRING*)malloc(sizeof(STRING)*tb->nrow);
	int p,q;
	int cp_len=0;
	for(i=0;i!=tb->nrow;i++){
		p=0;
		j=0;
		while(1){
			q=p;
			while(!isspace(row[i][p])&&row[i][p]!='\0')p++;
			cp_len=p-q;
			tb->strs[j][i]=StrTrunk(row[i],q,p-1);
			j++;
			if(row[i][p]!='\0')p++;
			else break;
		}
	}
	for(i=0;i!=tb->nrow;i++) free(row[i]);
	free(row);
}

void IniTables(TB* tb){
	tb->strs=NULL;
	tb->ncol=tb->nrow=0;
}

void WriteFile(CSTRING filename,TB* tb){
	FILE* fp=fopen(filename,"w");
	if(!fp) return;
	if(!tb) return;
	int i,j;
	for(i=0;i!=tb->nrow;i++){
		for(j=0;j!=tb->ncol;j++){
			fprintf(fp,"%s",tb->strs[j][i]);
			if(j!=tb->ncol-1) fprintf(fp," ");
			else fprintf(fp,"\n");
		}
	}
	fclose(fp);
}

void DeleteTables(TB* tb){
	if(!tb) return;
	int i,j;
	for(j=0;j!=tb->ncol;j++){
		for(i=0;i!=tb->nrow;i++) free(tb->strs[j][i]);
		free(tb->strs[j]);
	}
	free(tb->strs);
}

void PrintTables(TB* tb){
	int i,j;
	if(!tb) return;
	for(i=0;i!=tb->nrow;i++){
		for(j=0;j!=tb->ncol;j++){
			printf("%s",tb->strs[j][i]);
			if(j!=tb->ncol-1) printf(" ");
			else printf("\n");
		}
	}
}

void MakeFloatTable(CSTRING filename,FLOAT*** fls,int* nrow,int* ncol){
	TB tb;
	MakeTable(filename,&tb);
	*nrow=tb.nrow;
	*ncol=tb.ncol-1;
	*fls=(FLOAT**) malloc(sizeof(FLOAT*)**nrow);
	int i,j;
	for(i=0;i!=*nrow;i++){
		(*fls)[i]=(FLOAT*)malloc(sizeof(FLOAT)**ncol);
		for(j=0;j!=*ncol;j++){
		   	(*fls)[i][j]=atof(tb.strs[j+1][i]);
		}
	}
	DeleteTables(&tb);
}

void MakeTreeFormat(CSTRING filename,STRING** intvs,STRING** tree_format,int* num){
	TB tb;
	MakeTable(filename,&tb);
	*num=tb.nrow;
	*intvs=(STRING*)malloc(sizeof(STRING)*tb.nrow);
	*tree_format=(STRING*)malloc(sizeof(STRING)*tb.nrow);
	int i;
	for(i=0;i!=tb.nrow;i++){
		int itv_len=(int)strlen(tb.strs[0][i]);
		int tft_len=(int)strlen(tb.strs[1][i]);
		(*intvs)[i]=(STRING)malloc(SIZECHAR*(itv_len+1));
		(*tree_format)[i]=(STRING)malloc(SIZECHAR*(tft_len+1));
		memcpy((*intvs)[i],tb.strs[0][i],SIZECHAR*(itv_len+1));
		memcpy((*tree_format)[i],tb.strs[1][i],SIZECHAR*(tft_len+1));
		(*intvs)[i][itv_len]='\0';
		(*tree_format)[i][tft_len]='\0';
	}
	//PrintTables(&tb);
	DeleteTables(&tb);
}

/*int main(int argc,char** argv){
	FLOAT** flt;
	int frow=0,fcol=0;
	STRING* itv;
	STRING* tfs;
	int num=0;
	
	MakeFloatTable(argv[1],&flt,&frow,&fcol);
	printf("frow: %d fcol: %d\n",frow,fcol);
	int i,j;
	for(i=0;i!=frow;i++){
		for(j=0;j!=fcol;j++){
			if(j!=fcol-1)printf("%.2f ",flt[i][j]);
			else printf("%.2f\n",flt[i][j]);
		}
		free(flt[i]);
	}
	free(flt);
	printf("****************************\n");
	MakeTreeFormat(argv[2],&itv,&tfs,&num);
	for(i=0;i!=num;i++){
		printf("%s %s\n",itv[i],tfs[i]);
		free(itv[i]);
		free(tfs[i]);
	}
	free(itv);
	free(tfs);
	//PrintTables(&tb);
	return 0;
}*/
