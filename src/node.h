#ifndef _NODE_H_
#define _NODE_H_
#include"macro.h"

typedef struct node{
	struct node* lchild;
	struct node* rchild;
	STRING TAG;
	FLOAT BranchLength;
	FLOAT TreeDepth;
	int subtree_nodes;
}ND;

ND* CreateNode(CSTRING strnode);
ND* MergeNode(ND* LNode,ND* RNode,CSTRING BL);
void UpdateNodeTag(ND* Node);
void UpdateNodeBranch(ND* Node);
void UpdateNode(ND* Node);
void ErgodicNode(ND* NODE);
void DeleteNode(ND* NODE);
#endif
