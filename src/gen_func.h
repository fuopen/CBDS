#ifndef _GEN_FUNC_H_
#define _GEN_FUNC_H_

#include"macro.h"

STRING i2a(int dec);
STRING f2a(FLOAT f,int prec);
STRING StrTrunk(CSTRING str,int beg,int end);
STRING c2a(char c);
FLOAT* cumsum(const FLOAT* farray,int n);
#endif
