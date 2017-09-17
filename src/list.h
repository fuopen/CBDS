#ifndef _LIST_H_
#define _LIST_H_
#include"macro.h"
#include"node.h"

typedef struct list{
	//int id;
	ND* ListNode;
	struct list* next;
}LS;

static void IniList(LS* ls);
static void InsertList(LS** Head,LS* insert);
static void SearchTree(LS** Head,ND* node,int if_root);
static void ErgodicList(LS* head,const FLOAT* nbs,int n);
static void DeleteList(LS* head);
void TreeIntvs(CSTRING tree_format,FLOAT** intvs,int* n);
void CorrectTree(CSTRING tree_format,STRING* new_tree_format,const FLOAT* nbs,int n);

#endif
