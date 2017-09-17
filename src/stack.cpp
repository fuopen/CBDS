#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"stack.h"

void NStackIni(NStack* ntop){
	*ntop=NULL;
}

int NStackEmpty(NStack* ntop){
	return *ntop==NULL;
}

void NStackPush(NStack* ntop,ND* mcur){
	NS* ns=(NS*)malloc(sizeof(NS));
	ns->curr=mcur;
	ns->next=*ntop;
	*ntop=ns;
}

ND* NStackPop(NStack* ntop){
	if(!*ntop) return NULL;
	NS* tmp=*ntop;
	*ntop = tmp->next;
	ND* x=tmp->curr;
	free(tmp);
	return x;
}

void NStackDelete(NStack* ntop){
	if(!*ntop) return;
	while(*ntop) NStackPop(ntop);
}

ND* NGetTop(NStack* ntop){
	if(!*ntop) return NULL;
	else return (*ntop)->curr;
}

void NStackPrint(NStack* ntop){
	NStack ns=*ntop;
	while(!ns){
		printf("p:%s l:%s r:%s\n",ns->curr->TAG,ns->curr->lchild->TAG,ns->curr->rchild->TAG);
		ns=ns->next;
	}
	printf("\n\n");
}

void SStackIni(SStack* stop){
	*stop=NULL;
}

int SStackEmpty(SStack* stop){
	return *stop==NULL;
}

void SStackPush(SStack* stop,char mcur){//STRING mcur){
	SS* ss=(SS*)malloc(sizeof(SS));
	ss->curr=mcur;
	ss->next=*stop;
	*stop=ss;
}

//STRING 
char SStackPop(SStack* stop){
	if(!*stop) return 0;
	SS* tmp=*stop;
	*stop = tmp->next;
	//STRING 
	char x=tmp->curr;
	free(tmp);
	return x;
}

void SStackDelete(SStack* stop){
	if(!*stop) return;
	while(*stop) SStackPop(stop);
}

//STRING 
char SGetTop(SStack* stop){
	//if(!*stop) return NULL;
	if(!*stop) return '\0';
	return (*stop)->curr;
}

void SStackShow(SStack* stop){
	if(!*stop) return;
	SStack show=*stop;
	while(show){
		printf("%c",show->curr);
		show=show->next;
	}
	printf("\n");
}
