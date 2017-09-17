#include<stdio.h>
#include<stdlib.h>
#include"gen_func.h"
//#include"file.h"
#include"tree.h"

int IfTreeBound(const char* seek){
	int ret;
	switch(*seek){
		case '(':ret=1;break;
		case ',':ret=2;break;
		case ')':ret=3;break;
		case ';':ret=4;break;
		default :ret=0;break;
	}
	return ret;
}

void ExtractTree(CSTRING tree_format,NStack* ntop){
	const char* loc_left;
	const char* loc_right;
	NStackIni(ntop);

	loc_left=loc_right=tree_format;
	int itb=0;
	while(*loc_right!='\0'){
		itb=IfTreeBound(loc_right);
		if(!itb)loc_right++;
		else{
			switch(itb){
				case 1: {	
							loc_left=loc_right;
						}
						break;
				case 2: {
							if(*loc_left=='('){
								STRING strnode=StrTrunk(tree_format,(int) (loc_left-tree_format)+1,(int) (loc_right-tree_format)-1);
								ND* node=CreateNode(strnode);
								free(strnode);
								NStackPush(ntop,node);
								loc_left=loc_right;
							}
							else{
								ND* rnode=NStackPop(ntop);
								ND* lnode=NStackPop(ntop);
								ND* mnode;
								STRING bl=StrTrunk(tree_format,(int) (loc_left-tree_format)+1,(int) (loc_right-tree_format)-1);
								mnode=MergeNode(lnode,rnode,bl);
								free(bl);
								NStackPush(ntop,mnode);
								loc_left=loc_right;

							}
						}	
						break;
				case 3: {
							if(*loc_left==','){
								STRING strnode=StrTrunk(tree_format,(int) (loc_left-tree_format)+1,(int) (loc_right-tree_format)-1);
								ND* rnode=CreateNode(strnode);
								NStackPush(ntop,rnode);
								loc_left=loc_right;
								free(strnode);
							}
							else{
								ND* rnode=NStackPop(ntop);
								ND* lnode=NStackPop(ntop);
								ND* mnode;
								STRING bl=StrTrunk(tree_format,(int) (loc_left-tree_format)+1,(int) (loc_right-tree_format)-1);
								mnode=MergeNode(lnode,rnode,bl);
								free(bl);
								NStackPush(ntop,mnode);
								loc_left=loc_right;
							}
						}	
						break;
				case 4:	{
							ND* rnode=NStackPop(ntop);
							ND* lnode=NStackPop(ntop);
							ND* mnode;
							mnode=MergeNode(lnode,rnode,NULL);
							NStackPush(ntop,mnode);
							loc_left=loc_right;
						}
						break;
				default:break;
			}
			loc_right++;
		}
	}
}

void UpdateTree(ND* root){
	UpdateNodeBranch(root);
	//UpdateNodeTag(root);
	UpdateNode(root);
}	

void ErgodicTree(NStack* ntop){
	ND* root=NGetTop(ntop);
	ErgodicNode(root);
}

void DeleteTree(NStack* ntop){
	ND* root=NGetTop(ntop);
	DeleteNode(root);
	NStackDelete(ntop);
}

/*int main(int argc,char** argv){
	STRING s=ReadFile(argv[1]);
	printf("ReadTree:%s\n",s);

	NStack ntop;
	
	ExtractTree(s,&ntop);
	ErgodicTree(&ntop);
	DeleteTree(&ntop);
	return 0;
}*/
