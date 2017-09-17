#ifndef _STACK_H_
#define _STACK_H_

#include"macro.h"
#include"node.h"

typedef struct NodeNStack{
	struct NodeNStack* next;
	ND* curr;
}NS;

typedef NS* NStack;

void NStackIni(NStack* ntop);
int NStackEmpty(NStack* ntop);
void NStackPush(NStack* ntop,ND* mcur);
ND* NStackPop(NStack* ntop);
void NStackDelete(NStack* ntop);
ND* NGetTop(NStack* ntop);
void NStackPrint(NStack* ntop);

typedef struct StringStack{
	struct StringStack* next;
	//STRING curr;
	char curr;
}SS;

typedef SS* SStack;

void SStackIni(SStack* stop);
int SStackEmpty(SStack* stop);
void SStackPush(SStack* stop,char mcur);//STRING mcur);
//STRING 
char SStackPop(SStack* stop);
void SStackDelete(SStack* stop);
//STRING 
char SGetTop(SStack* stop);
void SStackShow(SStack* stop);
#endif
