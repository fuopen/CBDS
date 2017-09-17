#ifndef _FILE_H_
#define _FILE_H_
#include<stdio.h>
#include"macro.h"
#define INT64 long int

typedef struct tables{
	STRING** strs;
	int nrow;
	int ncol;
}TB;

static void IniTables(TB* tb);
static void MakeFile(CSTRING filename,STRING* str);
static void MakeLines(CSTRING filename,STRING** str,int* NR);
static void MakeTable(CSTRING filename,TB* tb);
void MakeFloatTable(CSTRING filename,FLOAT*** fls,int* nrow,int* ncol);
void MakeTreeFormat(CSTRING filename,STRING** intvs,STRING** tree_format,int* nrow);
static void WriteFile(CSTRING filename,TB* tb);
static void DeleteTables(TB* tb);
static void PrintTables(TB* tb);
#endif
