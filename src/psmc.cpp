#include "cor.h"
#include "psmc.h"
void Cor3Args(CSTRING arg1,CSTRING arg2,CSTRING arg3,bool if_true){
	cor psmc_cor(arg1,arg2);
	psmc_cor.print(arg3,if_true);
}

void Cor4Args(CSTRING arg1,CSTRING arg2,CSTRING arg3,CSTRING arg4){
	cor psmc_cor(arg1,arg2,arg3);
	psmc_cor.print(arg4);	
}
