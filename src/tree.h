#ifndef _TREE_H_
#define _TREE_H_
#include"node.h"
#include"stack.h"

static int IfTreeBound(const char* seek);
void ExtractTree(CSTRING tree_format,NStack* ntop);
void ErgodicTree(NStack* ntop);
void DeleteTree(NStack* ntop);
void UpdateTree(ND* root);
#endif
