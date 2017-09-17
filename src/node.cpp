#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include"node.h"
#include"gen_func.h"

ND* CreateNode(CSTRING strnode){
	ND* cn = (ND*)malloc(sizeof(ND));
	int str_len=(int) strlen(strnode);
	int i=str_len-1;
	while(i>=0){
		if(strnode[i]==':') break;
		else i--;
	}

	STRING bl= (STRING)malloc(SIZECHAR*(str_len-i));
	memcpy(bl,strnode+i+1,SIZECHAR*(str_len-i));
	bl[str_len-i-1]='\0';
	cn->BranchLength =(FLOAT) atof(bl);
	free(bl);
	
	cn->lchild=NULL;
	cn->rchild=NULL;
	cn->subtree_nodes=1;
	cn->TAG =(STRING) malloc(SIZECHAR*((int)strlen(strnode)+1));
	memcpy(cn->TAG,strnode,SIZECHAR*((int)strlen(strnode)+1));
	cn->TreeDepth=0.0;
	return cn;
}

ND* MergeNode(ND* LNode,ND* RNode,CSTRING BL){
	ND* mn = (ND*)malloc(sizeof(ND));
	mn->lchild=LNode;
	mn->rchild=RNode;
	mn->subtree_nodes=LNode->subtree_nodes+RNode->subtree_nodes;
	mn->TreeDepth=LNode->TreeDepth+LNode->BranchLength;
	int ltag_len=(int) strlen(LNode->TAG);
	int rtag_len=(int) strlen(RNode->TAG);
	if(BL){
		int bl_len=(int) strlen(BL);
		mn->BranchLength=(FLOAT) atof(BL+1);
		mn->TAG=(STRING)malloc(SIZECHAR*(ltag_len+rtag_len+bl_len+4));
		mn->TAG[0]='(';
		memcpy(mn->TAG+1,LNode->TAG,SIZECHAR*ltag_len);
		mn->TAG[ltag_len+1]=',';
		memcpy(mn->TAG+ltag_len+2,RNode->TAG,SIZECHAR*rtag_len);
		mn->TAG[ltag_len+rtag_len+2]=')';
		memcpy(mn->TAG+ltag_len+rtag_len+3,BL,SIZECHAR*bl_len);
		mn->TAG[ltag_len+rtag_len+bl_len+3]='\0';
	}
	else{
		mn->BranchLength=0.0;
		mn->TAG=(STRING)malloc(SIZECHAR*(ltag_len+rtag_len+5));
		mn->TAG[0]='(';
		memcpy(mn->TAG+1,LNode->TAG,SIZECHAR*ltag_len);
		mn->TAG[ltag_len+1]=',';
		memcpy(mn->TAG+ltag_len+2,RNode->TAG,SIZECHAR*rtag_len);
		mn->TAG[ltag_len+rtag_len+2]=')';
		mn->TAG[ltag_len+rtag_len+3]=';';
		mn->TAG[ltag_len+rtag_len+4]='\0';
	}
	return mn;
}

void UpdateNodeTag(ND* Node){
	if(!Node||!Node->TAG) return;
	int tag_len=(int) strlen(Node->TAG);
	//FLOAT nb_len=Node->BranchLength;
	//STRING cb_len=f2a(nb_len,2);
	//int clen=(int) strlen(cb_len);
	if(!Node->lchild&&!Node->rchild){
		FLOAT nb_len=Node->BranchLength;
		STRING cb_len=f2a(nb_len,2);
		int clen=(int) strlen(cb_len);
		int i=tag_len-1;
		while(i>=0){
			if(Node->TAG[i-1]==':') break;
			else i--;
		}
		STRING NTAG=(STRING)malloc(SIZECHAR*(i+clen+1));
		memcpy(NTAG,Node->TAG,SIZECHAR*i);
		memcpy(NTAG+i,cb_len,SIZECHAR*clen);
		NTAG[clen+i]='\0';
		free(cb_len);
		free(Node->TAG);
		Node->TAG=NTAG;
	}
	else{
		int ltag_len=(int) strlen(Node->lchild->TAG);
		int rtag_len=(int) strlen(Node->rchild->TAG);
		//free(Node->TAG);

		if(Node->TAG[tag_len-1]!=';'){
			free(Node->TAG);
			FLOAT nb_len=Node->BranchLength;
			STRING cb_len=f2a(nb_len,2);
			int clen=(int) strlen(cb_len);

			Node->TAG=(STRING)malloc(SIZECHAR*(ltag_len+rtag_len+clen+5));
			Node->TAG[0]='(';
			memcpy(Node->TAG+1,Node->lchild->TAG,SIZECHAR*ltag_len);
			Node->TAG[ltag_len+1]=',';
			memcpy(Node->TAG+ltag_len+2,Node->rchild->TAG,SIZECHAR*rtag_len);
			Node->TAG[ltag_len+rtag_len+2]=')';
			Node->TAG[ltag_len+rtag_len+3]=':';
			memcpy(Node->TAG+ltag_len+rtag_len+4,cb_len,SIZECHAR*clen);
			Node->TAG[ltag_len+rtag_len+clen+4]='\0';
			free(cb_len);
		}
		else{
			free(Node->TAG);
			Node->TAG=(STRING)malloc(SIZECHAR*(ltag_len+rtag_len+5));
			Node->TAG[0]='(';
			memcpy(Node->TAG+1,Node->lchild->TAG,SIZECHAR*ltag_len);
			Node->TAG[ltag_len+1]=',';
			memcpy(Node->TAG+ltag_len+2,Node->rchild->TAG,SIZECHAR*rtag_len);
			Node->TAG[ltag_len+rtag_len+2]=')';
			Node->TAG[ltag_len+rtag_len+3]=';';
			Node->TAG[ltag_len+rtag_len+4]='\0';
		}
	}	
}

void UpdateNodeBranch(ND* Node){
	if(!Node->lchild&&!Node->rchild) return;

	Node->lchild->BranchLength=Node->TreeDepth-Node->lchild->TreeDepth;
	Node->rchild->BranchLength=Node->TreeDepth-Node->rchild->TreeDepth;

	UpdateNodeBranch(Node->lchild);
	UpdateNodeBranch(Node->rchild);
}

void UpdateNode(ND* Node){
	if(Node->lchild) UpdateNode(Node->lchild);
	if(Node->rchild) UpdateNode(Node->rchild);
	UpdateNodeTag(Node);
}

void ErgodicNode(ND* NODE){
	if(!NODE) return;
	if(NODE->lchild) ErgodicNode(NODE->lchild);
	if(NODE->rchild) ErgodicNode(NODE->rchild);
	if(NODE->TAG)printf("node.tag: %s\n,tree_depth:%.2f,subnodes:%d,BranchLength:%.2f\n",NODE->TAG,NODE->TreeDepth,NODE->subtree_nodes,NODE->BranchLength);
	//UpdateNode(Node,Node->BranchLength);
}

void DeleteNode(ND* NODE){
	if(!NODE) return;
	if(NODE->lchild) DeleteNode(NODE->lchild);
	if(NODE->rchild) DeleteNode(NODE->rchild);
	if(NODE->TAG){
		//printf("Now delete:%s\n",NODE->TAG);
		free(NODE->TAG);
	}
	free(NODE);
}

/*int main(){
	STRING strs[3]={"abc:9.2","def:9.2","fuck:12.1"};
	ND* n1=CreateNode(strs[0]);
	ND* n2=CreateNode(strs[1]);
	ND* n3=CreateNode(strs[2]);
	STRING bl=":33.89";
	ND* nm=MergeNode(n1,n2,bl);
	ND* nk=MergeNode(nm,n3,NULL);
	printf("n1:%s, n2:%s, nm:%s, nk:%s\n",n1->TAG,n2->TAG,nm->TAG,nk->TAG);
	DeleteNode(nk);
	return 0;
}*/
