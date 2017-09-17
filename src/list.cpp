#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gen_func.h"
#include"tree.h"
#include"list.h"

void IniList(LS* ls){
	ls->ListNode=NULL;
	ls->next=NULL;
}

void InsertList(LS** Head,LS* insert){
	if(!(*Head)->next){ 
		insert->next=*Head;
		*Head=insert;
		return;
	}
	
	if(insert->ListNode->TreeDepth<=(*Head)->ListNode->TreeDepth){
		insert->next=*Head;
		*Head=insert;
		return;
	}
	LS* search = *Head;
	while(search&&search->next){
		if(insert->ListNode->TreeDepth>search->next->ListNode->TreeDepth){
			search=search->next;
		}
		else{
			insert->next=search->next;
			search->next=insert;
			return;
		}
	}
}

void SearchTree(LS** Head,ND* node,int if_root){
	LS* head=*Head;
	if(!(*Head)||!node||node->subtree_nodes==1) return;
	LS* nt;
	if(if_root){
		(*Head)->ListNode=node;
		(*Head)->next=NULL;
		if_root=0;
	}
	else{
		nt=(LS*) malloc(sizeof(LS));
		IniList(nt);
		nt->ListNode=node;
		InsertList(Head,nt);
	}
	 SearchTree(Head,node->lchild,if_root);
	 SearchTree(Head,node->rchild,if_root);
}

void ErgodicList(LS* head,const FLOAT* nbs,int n){
	if(!head||!nbs) return;
	LS* replace=head;
	int i=0;
	while(replace->next){
		replace->ListNode->TreeDepth=nbs[i];
		replace=replace->next;
		i++;
	}
	replace->ListNode->TreeDepth=nbs[i];
	UpdateTree(replace->ListNode);
}	

void DeleteList(LS* head){
	LS* dt=head;
	while(dt){
		LS* tmp=dt->next;
		free(dt);
		dt=tmp;
	}
}

void CorrectTree(CSTRING tree_format,STRING* new_tree_format,const FLOAT* nbs,int n){
	NStack ntop;
	ExtractTree(tree_format,&ntop);
	ND* nd=NGetTop(&ntop);
	LS* head=(LS*) malloc(sizeof(LS));
	IniList(head);
	SearchTree(&head,nd,1);
	ErgodicList(head,nbs,n);
	int tf_len=(int)strlen(nd->TAG);
	*new_tree_format=(STRING)malloc(SIZECHAR*(tf_len+1));
	memcpy(*new_tree_format,nd->TAG,SIZECHAR*(tf_len+1));
	(*new_tree_format)[tf_len]='\0';
	DeleteList(head);
	DeleteTree(&ntop);
}	

void TreeIntvs(CSTRING tree_format,FLOAT** intvs, int* n){
	NStack ntop;
	//printf("%s\n",tree_format);
	ExtractTree(tree_format,&ntop);
	ND* nd=NGetTop(&ntop);
	LS* head=(LS*) malloc(sizeof(LS));
	IniList(head);
	SearchTree(&head,nd,1);
	int p=0;
	LS* sn=head;
	while(sn){
		sn=sn->next;
		p++;
	}
	*n=p;
	*intvs=(FLOAT*) malloc(sizeof(FLOAT)**n);
	LS* iter=head;
	int i=0;
	while(iter->next){
		(*intvs)[i]=iter->ListNode->TreeDepth;
		iter=iter->next;
		i++;
	}
	(*intvs)[i]=iter->ListNode->TreeDepth;
	FLOAT* f=(FLOAT*) malloc(sizeof(FLOAT)*p);
	memcpy(f,*intvs,sizeof(FLOAT)*p);
	for(i=1;i<p;i++) (*intvs)[i]=f[i]-f[i-1];
	free(f);
	DeleteList(head);
	DeleteTree(&ntop);
}

/*int main(int argc,char** argv){
	STRING s=ReadFile(argv[1]);
	STRING t=ReadFile(argv[2]);
	TB tb;
	MakeTables(t,&tb);
	int nb=tb.ncol;
	FLOAT* fa=(FLOAT*)malloc(sizeof(FLOAT)*nb);
	int i;
	for(i=0;i!=nb;i++) fa[i]=atof(tb.strs[i][0]);
	FLOAT* intv=cumsum(fa,nb);
	free(fa);
	DeleteTables(&tb);

	NStack ntop;
	ExtractTree(s,&ntop);
	ND* nd=NGetTop(&ntop);
	LS* head=(LS*) malloc(sizeof(LS));
	IniList(head);
	SearchTree(&head,nd,1);
	ErgodicList(head,intv,nb);
	DeleteList(head);
	DeleteTree(&ntop);
	free(intv);
	return 0;
}*/
