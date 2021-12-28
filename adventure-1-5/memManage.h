#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define MAX_MEM 100

#define F_FIT	0
#define B_FIT	1
#define	W_FIT	2

#include <iostream>
#include <stdlib.h>

using namespace std;

#ifndef _CHUNK

#define _CHUNK

typedef struct chunk {
	int mem_start = 0;
	int mem_left_size = 0;
	struct chunk *link = NULL;
} Chunk;

#endif

extern Chunk *available;
extern unsigned int tag;

void setTag();

void initMem();
void releaseMem();// 모든 메모리 할당 해제

int myalloc(int req_size);
void myfree(int start_addr, int return_size);
void setNode(Chunk *temp, int start_addr, int return_size);

void memMerge();

void bwfitSort();
void bwfitSwap(Chunk *temp_1, Chunk *temp_2);

void bwfitMerge();

void printMemStat();